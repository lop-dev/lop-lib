using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace UDLib.Resource
{
    public class CLoadRequest
    {
        public static uint UID = 0;

        internal int m_eResourceType;
        internal string packageName;
        internal string[] resourceNames;
        internal CLoadPriority priority;
        //该request是master的依赖，并且master是开发者主动调用的加载
        internal CLoadRequest master;
        //自己是Master(因为引用的重复性，master.dep.dep的master也是null哦)
        internal bool isMaster;
        //合并相同的加载，需要执行完成回调
        internal List<CLoadRequest> sameMasterRequest;

        //需要加载的资源数量
        public int load_num = 0;
        //已加载数量
        public int loaded_num = 0;
        public CReleaseType releaseType = CReleaseType.Normal;
        public uint id = 0;
        //是否异步从assetbundle里加载资源
        public bool isAsyncMode;

        private float _progress;
        internal float Progress
        {
            get { return _progress; }
            set
            {

                _progress = value;

                if (sameMasterRequest != null)
                {
                    for (int i = 0; i < sameMasterRequest.Count; i++)
                    {
                        sameMasterRequest[i]._progress = _progress;
                    }
                }
            }
        }

        internal List<System.Action<CAssetObject[]>> onComplete;
        internal List<System.Action> onError;
        internal List<System.Action> onCancel;
        internal List<System.Action> onSceneLoadComplete;

        public int retry = 0;
        //#if !DIRECT_MODE
        public Hash128 hash;

        //是否发生了错误
        public bool isError;
        //#endif
        /// <summary>
        /// 依赖加载
        /// </summary>
        internal List<CLoadRequest> depencencyRequests;
        /// <summary>
        /// 已加载好的物体
        /// </summary>
        internal CAssetObject[] load_objectsThisRequest;

        internal bool isLoadingFromAssetBundle;

        public CLoadRequest()
        {
            CreateId();
            onComplete = new List<System.Action<CAssetObject[]>>();
            onError = new List<System.Action>();
            onCancel = new List<System.Action>();
            onSceneLoadComplete = new List<System.Action>();
        }

        private bool isSetuped = false;
        public void Setup()
        {
            if (isSetuped) return;
            isSetuped = true;
            packageName = MbAssetBundleManager.Instance.ConvertPackgeName(m_eResourceType, packageName);
        }

        private void CreateId()
        {
            id = UID++;
        }

        //是否所有的sameRequest加载完了
        public bool isSameRequestComplete()
        {
            if (sameMasterRequest == null)
                return true;

            for (int i = 0; i < sameMasterRequest.Count; i++)
            {
                CLoadRequest r = sameMasterRequest[i];
                if (r == this)
                    continue;
                if (r.load_num != r.loaded_num)
                    return false;
            }
            return true;
        }

        public void Cancel()
        {
            onComplete = null;
            onError = null;
            onCancel = null;
            retry = 0;
            Progress = 1;
            depencencyRequests = null;
            master = null;
            isAsyncMode = true;
        }

        internal void LoadObjectFromAssetBundle(AssetBundle asset_bundle, string sub_path, bool isAsyncMode = true)
        {
            isLoadingFromAssetBundle = true;
            MbAssetBundleManager.Instance.StartCoroutine(DoLoadObjectFromAssetBundle(asset_bundle, sub_path,isAsyncMode));
        }

        private IEnumerator DoLoadObjectFromAssetBundle(AssetBundle asset_bundle, string sub_path,bool isAsyncMode = true)
        {
            for (int i = 0, n = load_num; i < n; ++i)
            {
                if (load_objectsThisRequest[i] == null)
                {
                    
                    UnityEngine.Object asset = null;

                    if (isAsyncMode)
                    {
                        AssetBundleRequest asset_bundle_load_asset = null;
                        if (MbAssetBundleManager.Instance.ResourceDefine.IsSprite(m_eResourceType))
                            asset_bundle_load_asset = asset_bundle.LoadAssetAsync<Sprite>(resourceNames[i]);
                        else
                        {
                            asset_bundle_load_asset = asset_bundle.LoadAssetAsync(resourceNames[i]);
                        } 
                        while (asset_bundle_load_asset.isDone == false)
                            yield return null;

                        asset = asset_bundle_load_asset.asset;
                    }
                    else
                    {
                        if (MbAssetBundleManager.Instance.ResourceDefine.IsSprite(m_eResourceType))
                            asset = asset_bundle.LoadAsset<Sprite>(resourceNames[i]);
                        else
                            asset = asset_bundle.LoadAsset(resourceNames[i]);
                    }

                    if (asset == null && !resourceNames[i].ToLower().Equals("shader"))
                    {
                        CAssetBundleLog.LogError(string.Format("Load {0} from assetbundle {1} failed", resourceNames[i], packageName));
                    }
                    load_objectsThisRequest[i] = CAssetObject.Get(m_eResourceType,
                        sub_path + resourceNames[i], asset);//refCount++
                    loaded_num++;
#if DEBUG
                    CAssetBundleLog.Log("读取资源：" + resourceNames[i]);
#endif
                    //保存读出来的资源到缓存
                    if (!MbAssetBundleManager.Instance.loadedAssetObjectDic.ContainsKey(m_eResourceType))
                    {
                        MbAssetBundleManager.Instance.loadedAssetObjectDic.Add(m_eResourceType, new Dictionary<string, CAssetObject>());
                    }

                    Dictionary<string, CAssetObject> loadedAssetCategoryDic = null;
                    if (MbAssetBundleManager.Instance.loadedAssetObjectDic.TryGetValue(m_eResourceType, out loadedAssetCategoryDic))
                    {
                        if (!loadedAssetCategoryDic.ContainsKey(sub_path + resourceNames[i]))
                            loadedAssetCategoryDic.Add(sub_path + resourceNames[i], load_objectsThisRequest[i]);
                    }
                }
            }

            if (sameMasterRequest != null)
            {
                for (int i = 0; i < sameMasterRequest.Count; i++)
                {
                    CLoadRequest r = sameMasterRequest[i];
                    if (r == this) continue;
                    r.LoadObjectFromAssetBundle(asset_bundle, sub_path,isAsyncMode);
                    //  while (r.isLoadingFromAssetBundle)
                    //     yield return null;
#if DEBUG
                    CAssetBundleLog.Log(string.Format("sameMasterRequest loaded.{0}", r.packageName));
#endif
                }
            }

            while (isLoadingFromAssetBundle_sameMasterRequest())
            {
                yield return null;
            }

            if (load_objectsThisRequest != null)
            {
                for (int j = 0; j < load_objectsThisRequest.Length; j++)
                {
                    if (load_objectsThisRequest[j] == null)
                    {
                        CAssetBundleLog.LogError(string.Format("error !{0},{1}", packageName, j));
                    }
                }
            }

            isLoadingFromAssetBundle = false;
        }

        private bool isLoadingFromAssetBundle_sameMasterRequest()
        {
            if (sameMasterRequest == null)
                return false;

            for (int i = 0; i < sameMasterRequest.Count; i++)
            {
                var r = sameMasterRequest[i];
                if (r == this)
                    continue;

                if (r.isLoadingFromAssetBundle)
                    return true;
            }
            return false;
        }
    }
}