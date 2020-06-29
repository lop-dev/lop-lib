using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System;
using System.Text;

namespace UDLib.Resource
{
    public class CABLoadTime
    {
        public string name;
        public float size;
        public float loadABStartTime;
        public float loadAssetStartTime;
        public float loadABTime;
        public float loadAssetTime;
        public bool isFinishLoad = false;
    }

    /// <summary>
    /// 加载管理器
    /// </summary>
    public class MbAssetBundleManager : UDLib.Utility.CMonoSingleton<MbAssetBundleManager>
    {
        //同时最大加载数量
        private const int MAX_LOAD = 5;
        //加载失败后重新加载次数
        private const int MAX_RETRY = 3;
        //等待加载的请求
        private List<CLoadRequest> _waitList;
        //已加载的资源信息  <category,<assetName,AssetObject>>
        public Dictionary<int, Dictionary<string, CAssetObject>> loadedAssetObjectDic { get; private set; }
        //系统永久缓存的资源（shader、字体资源等）
        public Dictionary<string, CAssetBundleObject> systemAssetBundleObject;
        //缓存的UI图集<图集ab，<spritename,sprite>>
        public Dictionary<string, Dictionary<string, Sprite>> UIAtlas;
        //常驻图集，不会销毁
        public Dictionary<string, Dictionary<string, Sprite>> alwaysUIAtlas;
        //已加载的ab文件
        public Dictionary<string, CAssetBundleObject> loadedAssetBundleDic { get; private set; }
        public Dictionary<string, CAssetBundleObject> loadedSceneAssetBundleDic { get; private set; }
        private List<string> mAssetbundleToRemove;
        //正在加载的request列表
        private List<CLoadRequest> loadingRquestList;
        // 资源加载模块打Dll，把Direct_Mode的宏缓存变量
        private bool _isDirectMode = true;
        // 接口，用于获取CResourceDefine中的配置
        private UDLib.Utility.IResourceDefine _iResourceDefine;
        private IExternalResources _iExternalResources;
        private bool _enableLog = false;
        // 已下载的资源清单
        private Dictionary<string, CUpdateObject> mDownloadedAssetDic = new Dictionary<string, CUpdateObject>();
        // 所有AB资源清单
        private Dictionary<string, CUpdateObject> mTotalAssetDic = new Dictionary<string, CUpdateObject>();

        // 用于测试加载时间
        private bool mIsRecordLoadingTime = false;
        private Dictionary<uint, CABLoadTime> mDebugLoadTimeList = new Dictionary<uint, CABLoadTime>();

        private List<CLoadRequest> toCancelList;
        public bool isLoadingManifest
        {
            get;
            private set;
        }

        protected override void Awake()
        {
            base.Awake();
            loadedAssetObjectDic = new Dictionary<int, Dictionary<string, CAssetObject>>();
            loadedAssetBundleDic = new Dictionary<string, CAssetBundleObject>();
            loadedSceneAssetBundleDic = new Dictionary<string, CAssetBundleObject>();
            systemAssetBundleObject = new Dictionary<string, CAssetBundleObject>();
            UIAtlas = new Dictionary<string, Dictionary<string, Sprite>>();
            alwaysUIAtlas = new Dictionary<string, Dictionary<string, Sprite>>();
            loadingRquestList = new List<CLoadRequest>();
            _waitList = new List<CLoadRequest>();
            mAssetbundleToRemove = new List<string>();
        }

        //加载ab资源目录
        private string rootPath = "";
        public string RootPath
        {
            get
            {
                if (rootPath == "")
                    CAssetBundleLog.LogError("root path not set!");
                return rootPath;
            }

            set
            {
                rootPath = value;
            }
        }

        private string cacheDir = "";
        public string CatchDir
        {
            get
            {
                if (cacheDir == "")
                    cacheDir = Path.Combine(Application.persistentDataPath, "assets/AssetBundle");
                return cacheDir;
            }
        }

        private string packageDir = "";
        public string PackageDir
        {
            get
            {
                if (packageDir == "")
                {
                    // 付费分支使用，后续会移除
                    //packageDir = Path.Combine(AppContentPath(), "assets/AssetBundle");
                    // 主干已经把assets目录去掉，由于昱霖反映视频播放加了assets前缀会读取不出
                    packageDir = Path.Combine(AppContentPath(), "AssetBundle");
                }
                return packageDir;
            }
        }

        string _manifestVersion = "0";
        public string manifestVersion
        {
            get
            {
                return _manifestVersion;
            }
            set
            {
                if (_manifestVersion != value)
                {
#if DEBUG
                    CAssetBundleLog.Log(string.Format("manifestVersion : {0} -> {1}", _manifestVersion, value));
#endif
                    _manifestVersion = value;
                }
            }
        }

        private AssetBundleManifest _mainMenifest = null;
        public AssetBundleManifest MainMenifest
        {
            get
            {
                return _mainMenifest;
            }
        }

        public UDLib.Utility.IResourceDefine ResourceDefine
        {
            get
            {
                return _iResourceDefine;
            }

            set
            {
                if (_iResourceDefine != null)
                {
                    CAssetBundleLog.LogError("+++++++++++ICResourceDefine is already set+++++++++++++");
                    return;
                }
                _iResourceDefine = value;
            }
        }

        public bool IsDirectMode
        {
            get
            {
                return _isDirectMode;
            }

            set
            {
                _isDirectMode = value;
            }
        }

        public IExternalResources IExternalResources
        {
            get
            {
                return _iExternalResources;
            }

            set
            {
                _iExternalResources = value;
            }
        }

        public bool EnableLog
        {
            get
            {
                return _enableLog;
            }

            set
            {
                _enableLog = value;
            }
        }

        public Dictionary<string, CUpdateObject> MDownloadedAssetDic
        {
            get { return mDownloadedAssetDic; }
            set { mDownloadedAssetDic = value; }
        }

        public Dictionary<string, CUpdateObject> MTotalAssetDic
        {
            get { return mTotalAssetDic; }
            set { mTotalAssetDic = value; }
        }

        public void LoadManifest(string path, string version = "")
        {
            if (path == null)
            {
                if (string.IsNullOrEmpty(version))
                {
                    version = (UnityEngine.Random.Range(1, 99999)).ToString();
                }
                path = RootPath + "version_" + version + "/AssetBundle" + "?v=" + version;
            }

            StartCoroutine(DoLoadManifest(path));
        }

        IEnumerator DoLoadManifest(string path)
        {

            if (IsDirectMode == true)
            {
                yield return new WaitForEndOfFrame();
            }
            else
            {
                while (isLoadingManifest == true)
                    yield return null;

                isLoadingManifest = true;
#if DEBUG
                CAssetBundleLog.Log("开始加载manifest，path:" + path);
#endif
                using (WWW www = new WWW(path))
                {
                    yield return www;
                    if (string.IsNullOrEmpty(www.error))
                    {
                        byte[] compressedByte = www.bytes;

                        // byte[] bytes = GZipStream.UncompressBuffer(compressedByte);
                        AssetBundle ab = AssetBundle.LoadFromMemory(compressedByte/**bytes*/);
                        _mainMenifest = ab.LoadAsset<AssetBundleManifest>("AssetBundleManifest");
                        ab.Unload(false);
                    }
                    else
                    {
                        CAssetBundleLog.LogError(string.Format("加载manifest文件失败：{0},{1}", www.url, www.error));
                    }
                }
            }
            isLoadingManifest = false;
        }

        internal void LoadScene(string resource_name, int eResourceType, System.Action callback)
        {
            CLoadRequest request = new CLoadRequest();

            request.m_eResourceType = eResourceType;
            request.packageName = resource_name;
            request.resourceNames = new string[] { resource_name };
            request.priority = CLoadPriority.NORMAL;
            request.master = null;
            request.isMaster = true;
            request.releaseType = CReleaseType.Normal;
            request.onSceneLoadComplete.Add(callback);

            request.load_num = request.resourceNames != null ? request.resourceNames.Length : 0;
            request.loaded_num = 0;

            if (request.load_num > 0)
                request.load_objectsThisRequest = new CAssetObject[request.load_num];
            Load(request);
        }

        internal CLoadRequest Load(
            int eResourceType,
            string resourceName,
            CLoadPriority priority,
            System.Action<CAssetObject[]> onLoadComplete,
            System.Action onLoadError,
            System.Action onLoadCancel,
            CReleaseType releaseType = CReleaseType.Normal,
            bool isAsynMode = true
            )
        {
            return Load(eResourceType, resourceName, new string[] { resourceName }, priority, onLoadComplete, onLoadError, onLoadCancel, releaseType, isAsynMode);
        }

        internal CLoadRequest Load(
            int eResourceType,        // 比如：UI_IMAGE
            string packageName,       // 比如：battleui.dat
            string[] resourceNames,   // 比如：fightbutton
            CLoadPriority priority,
            System.Action<CAssetObject[]> onLoadComplete,
            System.Action onLoadError,
            System.Action onLoadCancel,
            CReleaseType releaseType = CReleaseType.Normal,
            bool isAsynMode = true
            )
        {
            CLoadRequest request = new CLoadRequest();

            request.m_eResourceType = eResourceType;
            request.packageName = packageName;
            request.resourceNames = resourceNames;
            request.priority = priority;
            request.master = null;
            request.isMaster = true;
            request.releaseType = releaseType;
            request.onComplete.Add(onLoadComplete);
            request.onError.Add(onLoadError);
            request.onCancel.Add(onLoadCancel);
            request.isAsyncMode = isAsynMode;

            request.load_num = request.resourceNames != null ? request.resourceNames.Length : 0;
            request.loaded_num = 0;

            if (request.load_num > 0)
                request.load_objectsThisRequest = new CAssetObject[request.load_num];

            if (!IsDirectMode)
            {
                if (isUIAtlas(eResourceType))
                {
                    //CAssetBundleLog.LogError("------------------是uiAtlas ----------------------" + request.packageName + ", eResourceType : " + eResourceType);
                    request.Setup();
                    Dictionary<string, Sprite> atlas;

                    if (UIAtlas.TryGetValue(request.packageName, out atlas) || alwaysUIAtlas.TryGetValue(request.packageName, out atlas))
                    {
                        //CAssetBundleLog.LogError("------------------uiAtlas 在缓存，直接加载----------------------" + request.packageName);
                        for (int i = 0, n = request.load_num; i < n; ++i)
                        {
                            if (request.load_objectsThisRequest[i] != null) continue;
                            string res_name = request.resourceNames[i];
                            if (atlas.ContainsKey(res_name))
                                request.load_objectsThisRequest[i] = CAssetObject.Get(request.m_eResourceType, res_name, atlas[res_name]);
                            else
                                CAssetBundleLog.LogError(string.Format("UI资源不存在：{0},packageName={1}", res_name, request.packageName));
                        }
                        if (onLoadComplete != null)
                            onLoadComplete(request.load_objectsThisRequest);
                        RemoveRequest(ref request);
                        return null;
                    }
                    else
                    {
                        //CAssetBundleLog.LogError("------------------uiAtlas 不在缓存，加载----------------------" + request.packageName);
                    }
                }
                else
                {
                    //CAssetBundleLog.LogError("------------------不是uiAtlas ----------------------" + request.packageName + ", eResourceType : " + eResourceType);
                }
            }

            Load(request);
            return request;
        }

        //处理依赖加载
        private void LoadDependency(CLoadRequest request)
        {
            List<CLoadRequest> dependency_requests = null;
            request.hash = this.MainMenifest.GetAssetBundleHash(request.packageName);
            dependency_requests = GetManifestDependencyRequests(request);
            request.depencencyRequests = dependency_requests;

            if (dependency_requests != null)
            {
                for (int i = 0; i < dependency_requests.Count; i++)
                {
                    if (loadingRquestList.IndexOf(dependency_requests[i]) == -1)//依赖列表获取时，在加载中的被引用了，无需再次加载处理
                        Load(dependency_requests[i]);
                }
            }
        }

        //检测相同的加载
        private bool LoadSameRequest(CLoadRequest request)
        {
            CLoadRequest sameLoadingReuqest = null;
            for (int i = 0; i < loadingRquestList.Count; i++)
            {
                //!loadingRquestList[i].isLoadingFromAssetBundle如果已经在读取资源了,回去处理sameRequest,这时候就不要合并过去了
                //如果loadingRquestList[i]从缓存中就满足加载需求了，就不要合并
                // load_num == 0的多级依赖也加入SameLoading判断
                if (loadingRquestList[i].packageName == request.packageName)
                {
                    if (loadingRquestList[i].isLoadingFromAssetBundle == false && (loadingRquestList[i].loaded_num < loadingRquestList[i].load_num || loadingRquestList[i].load_num == 0))
                    {
                        sameLoadingReuqest = loadingRquestList[i];
                        break;
                    }
                }
            }

            if (sameLoadingReuqest == null)
            {
                for (int i = 0; i < _waitList.Count; i++)
                {
                    if (_waitList[i].packageName == request.packageName)
                    {
                        sameLoadingReuqest = _waitList[i];
                        break;
                    }
                }
            }

            if (sameLoadingReuqest != null)
            {
                if (request.master == null)//合并masterRequest,如果相同的request是被其他masterrequest依赖的，直接return
                {
                    if (sameLoadingReuqest.sameMasterRequest == null)
                        sameLoadingReuqest.sameMasterRequest = new List<CLoadRequest>();

                    sameLoadingReuqest.sameMasterRequest.Add(request);
#if DEBUG
                        CAssetBundleLog.Log("合并相同的master加载请求：" + request.packageName);
#endif
                }

                return true;
            }

            return false;
        }

        private void Load(CLoadRequest request)
        {
            if (IsDirectMode)
            {
                _waitList.Add(request);
            }
            else
            {
                request.Setup();
#if DEBUG
                CAssetBundleLog.Log(string.Format("请求加载：{0}", request.packageName));
#endif
                //优先处理依赖加载
                LoadDependency(request);

                //检测相同的加载
                bool isLoadingSame = LoadSameRequest(request);
                if (isLoadingSame)
                    return;

                if (loadingRquestList.IndexOf(request) == -1)
                {
                    if (loadingRquestList.Count < MAX_LOAD)
                    {
                        loadingRquestList.Add(request);
                        //CAssetBundleLog.Log(string.Format("----加入加载列表：{0},loading Count:{1}", request.packageName, loadingRquestList.Count));
                        StartCoroutine(DoLoad(request));
                    }
                    else
                    {
                        //  CAssetBundleLog.Log("~~~~加入等待列表：" + request.packageName);
                        _waitList.Add(request);
                    }
                }
            }
        }

        private void Update()
        {
            CAssetBundleObject.CheckGcCollect();
            CAssetObject.CheckGcCollect();
            if (loadingRquestList == null || _waitList == null) return;

            //处理待加载列表
            while (loadingRquestList.Count < MAX_LOAD)
            {
                if (_waitList.Count > 0)
                {
                    CLoadRequest request = _waitList[0];
                    _waitList.Remove(request);
                    loadingRquestList.Add(request);
                    StartCoroutine(DoLoad(request));
                }
                else
                    break;
            }
        }

        // Direct Mode 加载资源（Resource.load）
        private void LoadResourceDirect(CLoadRequest request, string sub_path)
        {
            if (request.loaded_num < request.load_num)
            {
                for (int i = 0, n = request.load_num; i < n; ++i)
                {
                    if (request.load_objectsThisRequest[i] != null) continue;
                    string res_name = sub_path + request.resourceNames[i];
                    UnityEngine.Object uobj = LoadDirect(request.m_eResourceType, res_name);
                    CAssetObject obj = CAssetObject.Get(request.m_eResourceType, res_name, uobj);
                    request.load_objectsThisRequest[i] = obj;
                    request.loaded_num++;
                    //保存读出来的资源到缓存
                    if (!loadedAssetObjectDic.ContainsKey(request.m_eResourceType))
                    {
                        loadedAssetObjectDic.Add(request.m_eResourceType, new Dictionary<string, CAssetObject>());
                    }

                    Dictionary<string, CAssetObject> loadedAssetCategoryDic = null;
                    if (loadedAssetObjectDic.TryGetValue(request.m_eResourceType, out loadedAssetCategoryDic))
                    {
                        if (!loadedAssetCategoryDic.ContainsKey(res_name))
                            loadedAssetCategoryDic.Add(res_name, request.load_objectsThisRequest[i]);
                    }
                }
            }
        }

        /// <summary>
        /// 尝试从缓存读取AB
        /// </summary>
        /// <param name="request"></param>
        /// <param name="asset_bundle"></param>
        /// <returns></returns>
        private AssetBundle TryLoadABFromCache(CLoadRequest request)
        {
            //获取assetBundle
            AssetBundle asset_bundle = null ;
            CAssetBundleObject assetBundleObject = null;
            if (loadedAssetBundleDic.TryGetValue(request.packageName, out assetBundleObject))
            {
                // 只要使用，都加一次引用计数
                if (request.master != null || request.isMaster)//由于依赖的重复性，引用会被增加多次，所以只有主动加载及其依赖的ab，才会addRef
                    assetBundleObject.AddRef();
                asset_bundle = assetBundleObject.assetBundle;
#if DEBUG
                        CAssetBundleLog.Log("从缓存获取ab:" + request.packageName);
#endif
            }
            else if (systemAssetBundleObject.TryGetValue(request.packageName, out assetBundleObject))
            {
                if (request.master != null || request.isMaster)//由于依赖的重复性，引用会被增加多次，所以只有主动加载及其依赖的ab，才会addRef
                    assetBundleObject.AddRef();
                asset_bundle = assetBundleObject.assetBundle;
#if DEBUG
                        CAssetBundleLog.Log("从系统缓存获取ab:" + request.packageName);
#endif
            }
            else
            {
                foreach (var key in CAssetBundleObject.unUsedAssetBundleObjects.Keys)
                {
                    if (key.name.Equals(request.packageName))
                    {
#if DEBUG
                                CAssetBundleLog.Log("从unUsedList获取assetbundle:" + request.packageName);
#endif
                        assetBundleObject = key;
                        CAssetBundleObject.unUsedAssetBundleObjects.Remove(key);
                        loadedAssetBundleDic.Add(request.packageName, key);
                        asset_bundle = key.assetBundle;
                        key.Revive();
                        if (request.master != null || request.isMaster)//由于依赖的重复性，引用会被增加多次，所以只有主动加载及其依赖的ab，才会addRef
                            key.AddRef();
                        break;
                    }
                }
            }

            return asset_bundle;
        }

        /// <summary>
        ///  AB使用完做释放
        /// </summary>
        /// <param name="request"></param>
        private void ReleaseABForRequest(CLoadRequest request)
        {
            // 资源读完了，放入延迟列表，准备释放assetbundle
            // 非常驻AB 才释放
            // releaseType是自动释放的才释放，UI管理的那块用手动释放
            if (request.load_num != 0 && !IsPersistABInGame(request.packageName) && request.releaseType == CReleaseType.Normal)
            {
                if (IsPersistABInScene(request.packageName))
                {
                    loadedSceneAssetBundleDic.Add(request.packageName, loadedAssetBundleDic[request.packageName]);
                }
                else if (loadedAssetBundleDic.ContainsKey(request.packageName))
                {
                    loadedAssetBundleDic[request.packageName].Release();
                }

                if (request.depencencyRequests != null)
                {
                    for (int k = request.depencencyRequests.Count - 1; k >= 0; k--)
                    {
                        CAssetBundleObject depAbObj = null;
                        var depReq = request.depencencyRequests[k];
                        bool hasValue = loadedAssetBundleDic.TryGetValue(depReq.packageName, out depAbObj);
                        if (hasValue)
                        {
                            if (IsPersistABInScene(request.packageName))
                            {
                                loadedSceneAssetBundleDic.Add(request.packageName, depAbObj);
                            }
                            else if (!IsPersistABInGame(depReq.packageName))
                            {
                                depAbObj.Release();
                            }
                        }
                    }
                }
            }
        }

        /// <summary>
        /// 加载资源结束，触发回调
        /// </summary>
        /// <param name="request"></param>
        private void OnLoadABFinish(CLoadRequest request)
        {
            if (request.loaded_num == request.load_num)
            {
                if (toCancelList == null || !toCancelList.Contains(request))
                {
                    if (IsScene(request.m_eResourceType))
                    {
                        foreach (var action in request.onSceneLoadComplete)
                            action();
                    }
                    else
                    {
                        foreach (var action in request.onComplete)
                            action(request.load_objectsThisRequest);
                    }

                    //处理sameMasterRequest
                    if (request.sameMasterRequest != null)
                    {
                        for (int rindex = 0; rindex < request.sameMasterRequest.Count; rindex++)
                        {
                            CLoadRequest currentSame = request.sameMasterRequest[rindex];
                            if (IsScene(request.m_eResourceType))
                            {
                                foreach (var action in currentSame.onSceneLoadComplete)
                                    action();
                            }
                            else
                            {
                                foreach (var action in currentSame.onComplete)
                                    action(currentSame.load_objectsThisRequest);
                            }
                        }
                    }
                }

                //  CAssetBundleLog.Log("!!!加载完成,name={0},loading Count = {1}", request.packageName, loadingRquestList.Count-1);
                if (null != toCancelList)
                    toCancelList.Remove(request);
                RemoveRequest(ref request);
            }
            else
            {
                OnRequestError(request);
                CAssetBundleLog.LogError(string.Format("加载完成数量不匹配{0}/{1} - {2}", request.loaded_num, request.load_num, request.packageName));
            }
        }

        private IEnumerator DoLoad(CLoadRequest request)
        {
            //等待依赖加载
            if (request.depencencyRequests != null)
            {
                while (true)
                {
                    bool wait = false;
                    for (int i = 0; i < request.depencencyRequests.Count; i++)
                    {
                        CLoadRequest dependency_request = request.depencencyRequests[i];
                        for (int j = 0; j < loadingRquestList.Count; j++)
                        {
                            if (loadingRquestList[j] == dependency_request)
                            {
                                wait = true;
                                break;
                            }
                        }

                        if (wait == true)//只要有一个在wait了，其他的引用资源不必继续检测了
                            break;
                    }

                    if (wait == true)
                    {
                        yield return null;
                        continue;
                    }
                    else
                        break;
                }
            }

            //依赖加载完了，进度20%
            request.Progress = 0.2f;
            string sub_path = GetSubPath(request.m_eResourceType, request.packageName).ToString();
#if DEBUG
            CAssetBundleLog.Log(string.Format("!!!开始加载,name={0}，wait count={1}", request.packageName, _waitList.Count));
#endif

            //从缓存中获取资源
            GetObjectFromCache(request, sub_path);
            //去即将销毁的列表里找找看
            GetObjectFromUnUsed(request, sub_path);
            request.Progress = 0.3f;
            AssetBundle asset_bundle = null;

            if (IsDirectMode)
            {
                LoadResourceDirect(request, sub_path);          // Direct Mode 加载资源（Resource.load）
            }
            else
            {
                //如果该request的assetbundle是被依赖的，则一定要加载 || 如果当前request的obj还有没在缓存找到的 || 如果其他request和此request加载地址一样，且需要加载的
                if (request.load_num == 0 || request.loaded_num < request.load_num || !request.isSameRequestComplete())
                {
                    asset_bundle = TryLoadABFromCache(request);
                    if (asset_bundle == null)
                    {
                        //从本地硬盘获取assetbundle
                        string pathPersist = GetPersistCachePath(request.packageName);
                        if (IsVersionCached(pathPersist))
                        {

#if DEBUG
                            // 开始记录加载，硬盘读取
                            StartRecord(request);
                            CAssetBundleLog.Log("从本地硬盘P目录加载ab:" + pathPersist);
#endif
                            if (!request.isAsyncMode)
                            {
                                asset_bundle = AssetBundle.LoadFromFile(pathPersist);
                                SaveToABCache(request.packageName, asset_bundle);
                            }
                            else
                            {
                                AssetBundleCreateRequest req = null;
                                req = AssetBundle.LoadFromFileAsync(pathPersist);
                                while (!req.isDone)
                                {
                                    //加载ab只算6成加载进度
                                    request.Progress = 0.3f + req.progress * 0.6f;
                                    yield return null;
                                }
                                asset_bundle = GetAssetBundleFromRequest(req, request);
                            }

                            if (asset_bundle == null)
                                File.Delete(pathPersist);
                        }
                    }

                    // 如果没有从P目录加载，尝试底包加载
                    if (asset_bundle == null)
                    {
                        string pathPackage = GetPackageCachePath(request.packageName);
                        if (MDownloadedAssetDic.ContainsKey(request.packageName))
                        {
#if DEBUG
                                // 开始记录加载，硬盘读取
                                StartRecord(request);
#endif
                            if (!request.isAsyncMode)
                            {
                                asset_bundle = AssetBundle.LoadFromFile(pathPackage);
                                SaveToABCache(request.packageName, asset_bundle);
                            }
                            else
                            {
#if DEBUG
                                    CAssetBundleLog.Log("P目录没有找到对应资源，从底包加载ab:" + pathPackage);
#endif
                                AssetBundleCreateRequest req = null;
                                req = AssetBundle.LoadFromFileAsync(pathPackage);
                                while (!req.isDone)
                                {
                                    //加载ab只算6成加载进度
                                    request.Progress = 0.3f + req.progress * 0.6f;
                                    yield return null;
                                }

                                asset_bundle = GetAssetBundleFromRequest(req, request);
                            }
                        }
                        else
                        {
                            CAssetBundleLog.Log("P目录没有找到对应资源，底包也没有ab资源:" + pathPackage);
                        }
                    }
                    request.Progress = 0.9f;//ab资源下载好了，90%
#if DEBUG
                    //从服务器下载asstBundle
                    if (asset_bundle == null)
                    {
                        // 开始记录加载，网络读取
                        StartRecord(request);
                    }
#endif
                    while (asset_bundle == null)
                    {
                        if (request.retry < MAX_RETRY)
                        {
                            // 远程资源改成增量目录存储后，这个Url需要增加Version字段
                            if (!mTotalAssetDic.ContainsKey(request.packageName))
                            {
                                CAssetBundleLog.LogError(string.Format("资源清单中没有资源：{0}，无法下载", request.packageName));
                                break;
                            }

                            CUpdateObject obj = mTotalAssetDic[request.packageName];
                            StringBuilder sbUrl = new StringBuilder(RootPath).Append("version_").Append(obj.version).Append("/").Append(request.packageName).Append("?v=").Append(request.hash.ToString());
#if DEBUG
                            CAssetBundleLog.Log("从服务器下载资源：" + sbUrl);
#endif
                            using (WWW www = new WWW(sbUrl.ToString()))
                            {
                                while (!www.isDone)
                                {
                                    //加载ab只算6成加载进度
                                    if (request.retry == 0)
                                        request.Progress = 0.3f + www.progress * 0.6f;
                                    yield return null;
                                }

                                if (String.IsNullOrEmpty(www.error) && www.isDone)
                                {
                                    CAssetBundleLog.Log(www.bytes.Length + "," + www.error + "," + www.isDone + "," + www.url);
                                    asset_bundle = AssetBundle.LoadFromMemory(www.bytes);
#if DEBUG
                                    CAssetBundleLog.Log("从服务器加载ab成功：" + sbUrl);
#endif
                                    //保存asset_bundle到缓存;
                                    if (!loadedAssetBundleDic.ContainsKey(request.packageName))
                                    {
                                        loadedAssetBundleDic.Add(request.packageName, CAssetBundleObject.Get(request.packageName, asset_bundle));
                                    }

                                    //保存asset_bundle到硬盘
                                    SaveAssetBundle(request.packageName, www.bytes);
                                }
                                else
                                {
                                    CAssetBundleLog.LogError(www.error + www.url);
                                }
                            }

                            if (asset_bundle == null)
                            {
                                request.retry++;
#if DEBUG
                                CAssetBundleLog.LogWarning(string.Format("{0}加载资源失败，第{1}次尝试重复加载：", sbUrl, request.retry));
#endif
                            }
                        }
                        else
                        {
                            request.isError = true;
                            break;
                        }
                    }

                    if (request.isError)
                    {
                        //加载错误
                        OnRequestError(request);
                        yield break;
                    }
                }

                //不管怎样，ab不会再处理了，强制90%进度
                request.Progress = 0.9f;

#if DEBUG
                // 记录加载结束
                CheckLoadTime(request);
#endif
                //从asset_bundle获取资源
                if (asset_bundle != null)
                {
                    //保存UIAtlas
                    if (isUIAtlas(request.m_eResourceType))
                    {
                        if (request.load_num == 0)
                            CAssetBundleLog.LogWarning(string.Format("=========================> depended:{0}", request.packageName));
#if DEBUG
                        CAssetBundleLog.Log(string.Format("save ....{0}", request.packageName));
#endif
                        AssetBundleRequest abreq = asset_bundle.LoadAllAssetsAsync<Sprite>();
                        while (!abreq.isDone)
                            yield return null;

                        UnityEngine.Object[] sprites = abreq.allAssets;
                        Dictionary<string, Dictionary<string, Sprite>> whichUIAtlas = null;
                        if (ResourceDefine.IsPersistAtlasInGame(request.packageName))
                            whichUIAtlas = alwaysUIAtlas;
                        else
                            whichUIAtlas = UIAtlas;
                        try
                        {
                            for (int spriteIndex = 0; spriteIndex < sprites.Length; spriteIndex++)
                            {
                                Dictionary<string, Sprite> atlasDic = null;
                                if (!whichUIAtlas.ContainsKey(request.packageName))
                                {
                                    atlasDic = new Dictionary<string, Sprite>();
                                    whichUIAtlas.Add(request.packageName, atlasDic);
                                    //CAssetBundleLog.LogError("------------------AB 加载，并缓存图集---------------------" + request.packageName);
                                }

                                if (whichUIAtlas.TryGetValue(request.packageName, out atlasDic))
                                {
                                    if (!atlasDic.ContainsKey(sprites[spriteIndex].name))
                                        atlasDic.Add(sprites[spriteIndex].name, sprites[spriteIndex] as Sprite);
                                }
                            }
                        }
                        catch (Exception e)
                        {
                            CAssetBundleLog.LogError(e.ToString());
                        }
                    }

                    if (!IsScene(request.m_eResourceType))
                    {
#if DEBUG
                        CAssetBundleLog.Log(string.Format("开始从ab={0}读取资源：request={1}", asset_bundle.name, request.packageName));
#endif
                        request.LoadObjectFromAssetBundle(asset_bundle, sub_path);

                        while (request.isLoadingFromAssetBundle)
                            yield return null;
                    }
                    else
                    {
                        request.loaded_num++;
                    }

                    while (request.isLoadingFromAssetBundle)
                        yield return null;

                    // AB使用完做释放
                    ReleaseABForRequest(request);
                }
            }
            //加载完成
            request.Progress = 1.0f;
#if DEBUG
            // 记录从AB实例化资源时间
            CheckInstanceTime(request);
#endif
            // 加载资源结束，触发回调
            OnLoadABFinish(request);
        }

        private void OnRequestError(CLoadRequest request)
        {
            CAssetBundleLog.LogError("加载资源出错：" + request.packageName);
            foreach (var onError in request.onError)
                onError();

            //处理sameMasterRequest
            if (request.sameMasterRequest != null)
            {
                for (int rindex = 0; rindex < request.sameMasterRequest.Count; rindex++)
                {
                    CLoadRequest currentSame = request.sameMasterRequest[rindex];
                    foreach (var onError in currentSame.onError)
                        onError();
                }
            }

            RemoveRequest(ref request);
        }

        private AssetBundle GetAssetBundleFromRequest(AssetBundleCreateRequest req, CLoadRequest request)
        {
            AssetBundle asset_bundle = null;
            try
            {
                asset_bundle = req.assetBundle;
            }
            catch (Exception e)
            {
                CAssetBundleLog.LogError(e.Message);
            }
            //保存asset_bundle到缓存;
            SaveToABCache(request.packageName, asset_bundle);

            return asset_bundle;
        }

        private void SaveToABCache(string packageName, AssetBundle asset_bundle)
        {
            //保存asset_bundle到缓存;
            if (asset_bundle != null)
            {
                if (!loadedAssetBundleDic.ContainsKey(packageName))
                {
                    loadedAssetBundleDic.Add(packageName, CAssetBundleObject.Get(packageName, asset_bundle));
                }
            }
        }

        private void GetObjectFromCache(CLoadRequest request, string sub_path)
        {
            //CAssetBundleLog.LogError("request : " + request.packageName + ", sub_path: " + sub_path);
            var dic = loadedAssetObjectDic;
            if (request.loaded_num < request.load_num)
            {
                Dictionary<string, CAssetObject> loadedAssetCategoryDic;

                if (loadedAssetObjectDic.TryGetValue(request.m_eResourceType, out loadedAssetCategoryDic))
                {
                    for (int i = 0, n = request.load_num; i < n; ++i)
                    {
                        if (request.load_objectsThisRequest[i] != null) continue;
                        string res_name = request.resourceNames[i];
                        if (loadedAssetCategoryDic.TryGetValue(sub_path + res_name, out request.load_objectsThisRequest[i]))
                        {
#if DEBUG
                            CAssetBundleLog.Log("从缓存获取资源:" + sub_path + res_name);
#endif
                            request.load_objectsThisRequest[i].AddRef();//引用+1
                            request.loaded_num++;
                        }
                    }
                }
            }

            if (request.sameMasterRequest != null)
            {
                foreach (CLoadRequest r in request.sameMasterRequest)
                {
                    if (r == request)
                        continue;
                    GetObjectFromCache(r, sub_path);
                }
            }
        }

        private void GetObjectFromUnUsed(CLoadRequest request, string sub_path)
        {
            if (request.loaded_num < request.load_num)
            {
                for (int i = 0, n = request.load_num; i < n; ++i)
                {
                    if (request.load_objectsThisRequest[i] != null) continue;
                    string res_name = sub_path + request.resourceNames[i];

                    foreach (var v in CAssetObject.unUsedAssetObjects.Keys)
                    {
                        if (v.name == res_name)
                        {
#if DEBUG
                            CAssetBundleLog.Log("从unUsedList获取资源:" + res_name);
#endif
                            request.load_objectsThisRequest[i] = v;
                            CAssetObject.unUsedAssetObjects.Remove(v);
                            loadedAssetObjectDic[v.eResourceType].Add(res_name, v);
                            v.Revive();
                            v.AddRef();
                            request.loaded_num++;
                            break;
                        }
                    }
                }
            }

            if (request.sameMasterRequest != null)
            {
                for (int i = 0; i < request.sameMasterRequest.Count; i++)
                {
                    CLoadRequest r = request.sameMasterRequest[i];
                    if (r == request)
                        continue;
                    GetObjectFromUnUsed(r, sub_path);
                }
            }
        }

        /// <summary>
        /// Resources直接加载
        /// </summary>
        /// <param name="eResourceType"></param>
        /// <param name="resource_name"></param>
        /// <returns></returns>
        public UnityEngine.Object LoadDirect(int eResourceType, string resource_name)
        {
            if (!IsDirectMode)
            {
                return null;
            }
            // CAssetBundleLog.Log(SysLogType.Other, "直接加载：" + resource_name);
            UnityEngine.Object load_object = null;

            string[] paths = ResourceDefine.GetABSubPaths()[(int)eResourceType];
            for (int i = 0, n = paths.Length; i < n; ++i)
            {
                string path = paths[i] + resource_name;
                //load_object = Resources.Load<UnityEngine.Object>(path);
                load_object = IExternalResources.LoadAsset(path);

                if (load_object != null)
                    break;
            }

            if (load_object == null)
            {
                if (isUIAtlas(eResourceType))
                {
                    int pos = resource_name.IndexOf("/", StringComparison.Ordinal);
                    if (pos != -1)
                    {
                        string cfgName = "config";
                        string fileName = resource_name.Substring(pos + 1);
                        string moduleName = resource_name.Remove(pos);

                        fileName = cfgName + Path.AltDirectorySeparatorChar + fileName;
                        string newFileName = moduleName + Path.AltDirectorySeparatorChar + fileName;
                        for (int i = 0, n = paths.Length; i < n; ++i)
                        {
                            load_object = IExternalResources.LoadAsset(paths[i] + newFileName);
                            if (load_object != null)
                                break;
                        }
                    }
                }
            }

            if (load_object == null)
                CAssetBundleLog.LogError(string.Format("项目中不存在文件ResourceType：{0},name:{1}", eResourceType.ToString(), resource_name));
            return load_object;
        }

        public void CancelAll()
        {
            for (int i = 0; i < _waitList.Count; i++)
            {
                foreach (var cancel in _waitList[i].onCancel)
                    cancel();
                //处理sameMasterRequest
                if (_waitList[i].sameMasterRequest != null)
                {
                    for (int rindex = 0; rindex < _waitList[i].sameMasterRequest.Count; rindex++)
                    {
                        CLoadRequest currentSame = _waitList[i].sameMasterRequest[rindex];
                        foreach (var onCancel in currentSame.onCancel)
                            onCancel();
                    }
                }
                _waitList[i].Cancel();
            }
            _waitList.Clear();

            if (toCancelList == null)
                toCancelList = new List<CLoadRequest>();
            else
                toCancelList.Clear();

            toCancelList.AddRange(loadingRquestList);
        }

        // 释放指定的Assetbundle
        public void ReleaseAssetbundle(int type, string pName)
        {
            string packageName = ConvertPackgeName(type, pName);
            if (loadedAssetBundleDic.ContainsKey(packageName))
            {
                var ab = loadedAssetBundleDic[packageName];
                string[] deps = MainMenifest.GetAllDependencies(packageName);
                foreach (string dep in deps)
                {
                    if (loadedAssetBundleDic.ContainsKey(dep))
                    {
                        if (IsPersistABInGame(dep) || IsPersistABInScene(dep))
                        {
                            //CAssetBundleLog.Log(string.Format("++++++++++++ReleaseAssetbundle 跳过持久化AB: {0} ++++++++++++++", dep));
                            continue;
                        }
                        var depAB = loadedAssetBundleDic[dep];
                        depAB.Release();
                        //CDebugOut.LogWarning(string.Format("++++++++++++++++主动释放依赖AB : {0}+++++++++++++++++++", dep));
                    }
                }

                //CDebugOut.LogWarning(string.Format("++++++++++++++++主动释放主AB : {0}+++++++++++++++++++", packageName));
                ab.Release();
                ReleaseAssetForAB(type, pName);
            }
        }

        private void ReleaseAssetForAB(int type, string pName)
        {
            // 释放AB相应的资源资源,只有主动加载的AB才缓存了AssetObject
            StringBuilder subPath = GetSubPath(type, pName);
            Dictionary<string, CAssetObject> loadedAssetCategoryDic = null;
            if (loadedAssetObjectDic.TryGetValue(type, out loadedAssetCategoryDic))
            {
                CAssetObject assetObject = null;
                string assetName = subPath.Append(pName).ToString();
                if (loadedAssetCategoryDic.TryGetValue(assetName, out assetObject))
                {
                    //CDebugOut.LogWarning(string.Format("++++++++++++++++主动释资源 : {0}+++++++++++++++++++", assetName));
                    assetObject.Release();
                }
            }
        }

        // 释放场景生命周期的常驻AB
        public void ReleaseScenePersistAB()
        {
            foreach (KeyValuePair<string, CAssetBundleObject> kv in loadedSceneAssetBundleDic)
            {
                MbAssetBundleManager.Instance.ReleaseAssetBundleObject(kv.Key);
                CAssetBundleObject.unUsedAssetBundleObjects[kv.Value] = Time.time;
            }
        }

        //释放缓存的所有Assetbundle
        public void ReleaseAssetBundles()
        {
            //mAssetbundleToRemove.Clear();
            //CDebugOut.LogWarning(string.Format("++++++++++++++++切换场景时释放所有的AB+++++++++++++++++++"));
            //foreach (KeyValuePair<string, CAssetBundleObject> kv in loadedAssetBundleDic)
            //{
            //    if (IsPersistAB(kv.Value.name))
            //    {
            //        CAssetBundleLog.Log(string.Format("++++++++++++ReleaseAssetBundles 跳过持久化AB: {0} ++++++++++++++", kv.Value.name));
            //        continue;
            //    }

            //    kv.Value.Dispose();
            //    mAssetbundleToRemove.Add(kv.Key);
            //}

            //foreach (var key in mAssetbundleToRemove)
            //{
            //    loadedAssetBundleDic.Remove(key);
            //}
            //mAssetbundleToRemove.Clear();

            CAssetBundleObject.DoGarbageCollect(true);
        }

        //释放所有加载的assetObject
        public void ReleaseAssetObjects()
        {
            //foreach (var catogory in loadedAssetObjectDic.Values)
            //{
            //    foreach (var obj in catogory.Values)
            //    {
            //        obj.Dispose();
            //    }
            //    catogory.Clear();
            //}

            foreach (var catogory in loadedAssetObjectDic.Values)
            {
                catogory.Clear();
            }
            loadedAssetObjectDic.Clear();
            CAssetObject.DoGarbageCollect(true);
        }

        //释放所有加载的UI图集
        public void ReleaseAllUIAtlas()
        {
            foreach (var atlasName in UIAtlas.Keys)
            {
                UIAtlas[atlasName].Clear();
            }
            UIAtlas.Clear();
        }

        //释放指定加载的UI图集
        public void ReleaseUIAtlas(string packageName)
        {
            if (!string.IsNullOrEmpty(packageName) && UIAtlas.ContainsKey(packageName))
            {
                UIAtlas[packageName].Clear();
                UIAtlas.Remove(packageName);
            }
        }

        public void ViewStaticCache()
        {
            var myloadedAssetObjectDic = loadedAssetObjectDic;
            var myloadedAssetBundleDic = loadedAssetBundleDic;
            var mysystemAssetBundleObject = systemAssetBundleObject;
            var myUIAtlas = UIAtlas;
            var myalwaysUIAtlas = alwaysUIAtlas;
            var myloadingRquestList = loadingRquestList;
            var my_waitList = _waitList;
            var mymAssetbundleToRemove = mAssetbundleToRemove;

            var ABunUsedAssetBundleObjects = CAssetBundleObject.unUsedAssetBundleObjects;
            var ABcacheToRemoveFromUnUsed = CAssetBundleObject.cacheToRemoveFromUnUsed;

            var ASunUsedAssetBundleObjects = CAssetObject.unUsedAssetObjects;
            var AScacheToRemoveFromUnUsed = CAssetBundleObject.cacheToRemoveFromUnUsed;
        }

        public void ReleaseAssetObject(int type, string packageName)
        {
            Dictionary<string, CAssetObject> loadedAssetCategoryDic = null;
            if (loadedAssetObjectDic.TryGetValue(type, out loadedAssetCategoryDic))
            {
                if (!loadedAssetCategoryDic.Remove(packageName))
                    CAssetBundleLog.LogError(string.Format("要移除的assetObject不在缓存列表中,packageName={0}", packageName));
            }
        }

        public void ReleaseAssetBundleObject(string packageName)
        {
#if DEBUG
            CAssetBundleLog.Log("移除assetbundle:" + packageName);
#endif
            if (!loadedAssetBundleDic.Remove(packageName))
                CAssetBundleLog.LogError(string.Format("要移除的assetBundleObject不在缓存列表中,packageName={0}", packageName));
        }

        private void RemoveRequest(ref CLoadRequest request)
        {
            loadingRquestList.Remove(request);
            request.Cancel();
            request = null;
        }

        public void MaskAsSystemAssetBundle(string packageName)
        {
            CAssetBundleObject assetBundleObj = null;
            if (loadedAssetBundleDic.TryGetValue(packageName, out assetBundleObj))
            {
                loadedAssetBundleDic.Remove(packageName);
                systemAssetBundleObject.Add(packageName, assetBundleObj);

            }
            else
            {
                foreach (var i in CAssetBundleObject.unUsedAssetBundleObjects.Keys)
                {
                    if (i.name.Equals(packageName))
                    {
                        assetBundleObj = i;
                        CAssetBundleObject.unUsedAssetBundleObjects.Remove(i);
                        systemAssetBundleObject.Add(packageName, assetBundleObj);

                        //if (packageName.Contains("shader"))
                        //{
                        //    Shader[] shaders = assetBundleObj.assetBundle.LoadAllAssets<Shader>();
                        //    Shader.WarmupAllShaders();
                        //}

                        break;
                    }

                }
            }

            if (assetBundleObj == null)
                CAssetBundleLog.LogError(string.Format("packageName = {0} is not found!", packageName));
        }

        //获取ab的依赖列表
        List<CLoadRequest> GetManifestDependencyRequests(CLoadRequest request)
        {
            List<CLoadRequest> dependency_requests = null;
            string[] deps = MainMenifest.GetAllDependencies(request.packageName);
            // CAssetBundleLog.Log("检测dependency:" + request.packageName);
            for (int i = 0, n = deps.Length; i < n; ++i)
            {
                string dep = deps[i];

                if (dependency_requests == null)
                    dependency_requests = new List<CLoadRequest>();

                CLoadRequest dependency_request = null;
                //是否有相同的request在加载了
                for (int j = 0, m = loadingRquestList.Count; j < m; ++j)
                {
                    if (loadingRquestList[j].packageName == dep)
                    {
                        dependency_request = loadingRquestList[j];
                        //  CAssetBundleLog.Log(SysLogType.Other, "Dependency Request 已经加入: {0} ({1})", dep, request.packageName);
                        break;
                    }
                }

                if (dependency_request == null)
                {
                    for (int j = 0, m = _waitList.Count; j < m; ++j)
                    {
                        if (_waitList[j].packageName == dep)
                        {
                            dependency_request = _waitList[j];
                            //  CAssetBundleLog.Log(SysLogType.Other, "Dependency Request 已经加入: {0} ({1})", dep, request.packageName);
                            break;
                        }
                    }

                }

                if (dependency_request == null)
                {
#if DEBUG
                    CAssetBundleLog.Log(string.Format("Dependency Request 创建: {0} ({1})", dep, request.packageName));
#endif

                    dependency_request = new CLoadRequest();

                    dependency_request.m_eResourceType = MbAssetBundleManager.Instance.ResourceDefine.GetResourceTypeMaxValue();
                    dependency_request.packageName = deps[i];
                    dependency_request.releaseType = request.releaseType;
                    dependency_request.isAsyncMode = false;
                }
                if (request.master == null)
                    dependency_request.master = request;

                dependency_requests.Add(dependency_request);
            }

            return dependency_requests;
        }

        bool IsVersionCached(string path)
        {
            return File.Exists(path);
        }

        bool IsVersionCached(string fileName, Hash128 hash)
        {
            return MDownloadedAssetDic.ContainsKey(fileName) && MDownloadedAssetDic[fileName].HashOrMD5 == hash.ToString();
        }

        // 获取Persist目录中资源路径
        string GetPersistCachePath(string filename)
        {
            var path = Path.Combine(CatchDir, filename);
            return path;
        }

        // 获取底包中资源目录路径
        string GetPackageCachePath(string filename)
        {
            var path = Path.Combine(PackageDir, filename);
            return path;
        }

        bool isUIAtlas(int t)
        {
            return MbAssetBundleManager.Instance.ResourceDefine.IsSprite(t);
        }

        bool IsScene(int t)
        {
            return MbAssetBundleManager.Instance.ResourceDefine.IsScene(t);
        }

        private bool IsPersistABInGame(string packgeName)
        {
            return MbAssetBundleManager.Instance.ResourceDefine.IsPersistABInGame(packgeName);
        }

        private bool IsPersistABInScene(string packgeName)
        {
            return MbAssetBundleManager.Instance.ResourceDefine.IsPersistABInScene(packgeName);
        }

        public void SaveAssetBundle(string filename, byte[] bytes)
        {
            if (bytes.Length == 0)
                return;

            string dir = Path.Combine(CatchDir, filename);
            FileInfo info = new FileInfo(dir);

            if (info.Directory.Exists)
            {
                try
                {
                    string dirPath = info.Directory.ToString().Replace("\\", "/");
                    string[] filePathArray = filename.Split('/');
                    var files = Directory.GetFiles(dirPath, filePathArray[filePathArray.Length - 1] + "*");
                    if (files != null)
                    {
                        foreach (var file in files)
                        {
#if DEBUG
                            CAssetBundleLog.Log("Delete cache file: " + file);
#endif
                            File.Delete(file);
                        }
                    }
                }
                catch
                {
                    CAssetBundleLog.LogError("空的缓存文件夹:" + dir);
                }
            }
            else
            {
                info.Directory.Create();
            }

            var path = GetPersistCachePath(filename);
#if DEBUG
            CAssetBundleLog.Log("Save cache file: " + path);
#endif
            try
            {
                File.WriteAllBytes(path, bytes);
            }
            catch (Exception e)
            {
                CAssetBundleLog.LogError("保存ab文件失败:" + filename + ",原因：" + e.ToString());
            }
        }

        // 转换AB的名称
        public string ConvertPackgeName(int resourceType, string packageName)
        {
            string packageNameConverted;
            if (resourceType != MbAssetBundleManager.Instance.ResourceDefine.GetResourceTypeMaxValue())
                packageNameConverted = MbAssetBundleManager.Instance.ResourceDefine.ToAssetBundleName(resourceType, packageName);//m_eResourceType.ToAssetBundleName(packageName);
            else
                packageNameConverted = packageName.ToLower();

            return packageNameConverted;
        }

        // 获取AB的subPath
        public StringBuilder GetSubPath(int eResourceType, string packageName)
        {
            StringBuilder sub_path = new StringBuilder();
            if (eResourceType != MbAssetBundleManager.Instance.ResourceDefine.GetResourceTypeMaxValue())//被依赖的ab request类型为max
            {
                switch (ResourceDefine.GetABBuildTypes()[(int)eResourceType])
                {
                    case UDLib.Utility.EBUILD_TYPE.PACK:
                    case UDLib.Utility.EBUILD_TYPE.SINGLE:
                    case UDLib.Utility.EBUILD_TYPE.HASH256:
                        break;
                    default:
                        sub_path = new StringBuilder(packageName.ToLower()).Append("/");
                        break;
                }
            }

            return sub_path;
        }

        #region DEBUG 加载时间
        public void StartRecordLoadTime()
        {
#if DEBUG
            if (IsDirectMode)
            {
                CAssetBundleLog.LogError("记录加载时间只能在AB模式使用");
                return;
            }

            mIsRecordLoadingTime = true;
#endif
        }

        public void StopRecordLoadTime()
        {
#if DEBUG
            if (IsDirectMode)
            {
                CAssetBundleLog.LogError("记录加载时间只能在AB模式使用");
                return;
            }

            if (mIsRecordLoadingTime == false)
                return;

            mIsRecordLoadingTime = false;
            SaveLoadTimeToCsv();
#endif
        }

        private void SaveLoadTimeToCsv()
        {
#if DEBUG
            if (mDebugLoadTimeList != null)
            {
                string path = Path.Combine(Application.persistentDataPath, string.Format("加载时间测试_{0}.csv", DateTime.Now.ToString("yyyy-MM-dd-hh-mm-ss")));
                StreamWriter sw = new StreamWriter(path, true, Encoding.UTF8);
                var sbTitle = new StringBuilder("资源名称").Append(",").Append("加载编号").Append(",").Append("加载AB时间（秒）").Append(",").Append("从AB加载资源时间（秒）").Append(",").Append("资源大小（Kb）").Append(",").Append("加载状态");
                sw.WriteLine(sbTitle.ToString());
                foreach (KeyValuePair<uint, CABLoadTime> kv in mDebugLoadTimeList)
                {
                    StringBuilder sb = new StringBuilder();
                    if (kv.Value.isFinishLoad == true)
                    {
                        sb.Append(kv.Value.name).Append(",").Append(kv.Key).Append(",").Append(kv.Value.loadABTime).Append(",").Append(kv.Value.loadAssetTime).Append(",").Append(kv.Value.size).Append(",").Append("加载成功");
                    }
                    else
                    {
                        sb.Append(kv.Value.name).Append(",").Append(kv.Key).Append(",").Append(0).Append(",").Append(0).Append(",").Append(0).Append(",").Append("加载出错");
                    }

                    sw.WriteLine(sb.ToString());
                }
                sw.Close();
                mDebugLoadTimeList.Clear();
            }
#endif
        }

        // 开始记录时间
        private void StartRecord(CLoadRequest request)
        {
#if DEBUG
            if (IsDirectMode)
                return;

            if (mIsRecordLoadingTime == true)
            {
                if (!mDebugLoadTimeList.ContainsKey(request.id))
                {
                    var loadTime = new CABLoadTime();
                    loadTime.name = request.packageName;
                    loadTime.loadABStartTime = Time.realtimeSinceStartup;
                    mDebugLoadTimeList.Add(request.id, loadTime);
                }
            }
#endif
        }

        // 记录加载时间
        private void CheckLoadTime(CLoadRequest request)
        {
#if DEBUG
            if (IsDirectMode)
                return;

            if (mIsRecordLoadingTime == true)
            {
                if (mDebugLoadTimeList.ContainsKey(request.id))
                {
                    var loadTime = mDebugLoadTimeList[request.id];
                    loadTime.loadABTime = Time.realtimeSinceStartup - loadTime.loadABStartTime;
                    loadTime.loadAssetStartTime = Time.realtimeSinceStartup;
                }
            }
#endif
        }

        // 记录实例化时间
        private void CheckInstanceTime(CLoadRequest request)
        {
#if DEBUG
            if (IsDirectMode)
                return;

            if (mIsRecordLoadingTime == true)
            {
                if (mDebugLoadTimeList.ContainsKey(request.id))
                {
                    var loadTime = mDebugLoadTimeList[request.id];
                    loadTime.loadAssetTime = Time.realtimeSinceStartup - loadTime.loadAssetStartTime;
                    string dir = GetPersistCachePath(request.packageName);
                    var f = new FileInfo(dir);
                    if (f != null)
                    {
                        loadTime.size = f.Length / 1024;
                    }
                    loadTime.isFinishLoad = true;
                }
            }
#endif
        }
        #endregion

        //--------------------------------------------------------------------------------
        //应用程序内容路径
        private string AppContentPath()
        {
            string path = string.Empty;

            switch (Application.platform)
            {
                case RuntimePlatform.Android:
                    path = "jar:file://" + Application.dataPath + "!/assets/";
                    break;
                case RuntimePlatform.IPhonePlayer:
                    path = Application.dataPath + "/Raw/";
                    break;
                default:
                    path = "file://" + Application.dataPath + "/StreamingAssets/";
                    break;
            }

            return path;
        }
    }
}