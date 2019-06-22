namespace UDLib.Resource
{
    /// <summary>
    /// 加载优先级别
    /// </summary>
    public enum CLoadPriority
    {
        LOW,        //优先级低，等其他任务都加载完了，自己再加载吧
        NORMAL,     //普通，按添加顺序加载
        IMPORTANT,  //重要，我有点急，先加载我吧
        SOON        //十万火急，我现在就要用，先给我加载了！
    }

    public enum CReleaseType
    {
        Normal,         //自动释放
        Manual,         //手动释放
    }

    public class CLoadObject
    {
        public delegate void LoadDelegate(bool isOk, CAssetObject[] resultObject, System.Object args);
        public bool isLoading { get; protected set; }
        public CLoadRequest request;
        public CAssetObject loadedObject;
        public CAssetObject[] loadedObjects;

        private LoadDelegate m_callback;
        private System.Object m_args;
        //加载进度
        public float Progress { get { return request.Progress; } }

        public CLoadObject()
        {
        }

        public CLoadObject(
            int eResourceType,
            string resourceName,
            LoadDelegate callback = null,
            CLoadPriority priority = CLoadPriority.NORMAL,
            System.Object args = null,
            CReleaseType releaseType = CReleaseType.Normal,
            bool isAsynMode = true
            )
        {
            _Load(eResourceType, resourceName, callback, priority, args, releaseType, isAsynMode);
        }

        public CLoadObject(
            int eResourceType,
            string packageName,
            string[] resourceNames,
            LoadDelegate callback = null,
            CLoadPriority priority = CLoadPriority.NORMAL,
            System.Object args = null,
            CReleaseType releaseType = CReleaseType.Normal,
            bool isAsynMode = true
            )
        {
            _Load(eResourceType, packageName, resourceNames, callback, priority, args, releaseType, isAsynMode);
        }

        public void _Load(
            int eResourceType,
            string resourceName,
            LoadDelegate callback = null,
            CLoadPriority priority = CLoadPriority.NORMAL,
            System.Object args = null,
            CReleaseType releaseType = CReleaseType.Normal,
            bool isAsynMode = true
            )
        {
            isLoading = false;

            if (string.IsNullOrEmpty(resourceName) == true)
            {
                CAssetBundleLog.LogWarning(string.Format("resource_name should not be null!ResourceType={0}", eResourceType.ToString()));
                return;
            }

            m_args = args;
            m_callback = callback;
            isLoading = true;
            request = MbAssetBundleManager.Instance.Load(eResourceType, resourceName, priority, _OnLoadComplete, OnLoadError, OnLoadCancel, releaseType, isAsynMode);
        }

        public void _Load(
            int eResourceType,
            string packageName,
            string[] resourceNames,
            LoadDelegate callback = null,
            CLoadPriority priority = CLoadPriority.NORMAL,
            System.Object args = null,
            CReleaseType releaseType = CReleaseType.Normal,
            bool isAsynMode = true
            )
        {
            isLoading = false;

            if (resourceNames == null || resourceNames.Length == 0)
            {
                CAssetBundleLog.LogWarning("resource_name should not be null!");
                return;
            }

            m_args = args;
            m_callback = callback;
            isLoading = true;
            request = MbAssetBundleManager.Instance.Load(eResourceType, packageName, resourceNames, priority, _OnLoadComplete, OnLoadError, OnLoadCancel, releaseType, isAsynMode);
        }

        public void LoadScene(int eResourceType, string resourceName, System.Action callback)
        {
            isLoading = false;
            if (string.IsNullOrEmpty(resourceName) == true)
            {
                CAssetBundleLog.LogWarning(string.Format("resource_name should not be null!ResourceType={0}", eResourceType.ToString()));
                return;
            }
            if (!MbAssetBundleManager.Instance.ResourceDefine.IsScene(eResourceType))
            {
                CAssetBundleLog.LogError(string.Format("resource_type should not be SCENE! resource_type = {}", (eResourceType).ToString()));
                return;
            }

            isLoading = true;
            MbAssetBundleManager.Instance.LoadScene(resourceName, eResourceType, callback);
        }

        protected virtual void _OnLoadComplete(CAssetObject[] objs)
        {
            if (objs != null && objs.Length >= 1 && objs[0] != null)
            {
                loadedObject = objs[0];
            }

            loadedObjects = objs;
            isLoading = false;

            if (m_callback != null)
            {
                m_callback(true, loadedObjects, m_args);
            }
        }

        void OnLoadError()
        {
            isLoading = false;
            if (m_callback != null)
            {
                m_callback(false, null, m_args);
            }
        }

        void OnLoadCancel()
        {
            isLoading = false;
            if (m_callback != null)
            {
                m_callback(false, null, m_args);
            }
        }

        public void Dispose()
        {
            if (loadedObjects != null)
            {
                for (int i = 0; i < loadedObjects.Length; i++)
                {
                    loadedObjects[i].Release();
                    loadedObjects[i] = null;
                }
            }

            loadedObjects = null;
            loadedObject = null;
            request = null;
        }
    }
}