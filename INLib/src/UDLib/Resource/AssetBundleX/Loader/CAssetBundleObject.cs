using UnityEngine;
using System.Collections.Generic;

namespace UDLib.Resource
{
    public class CAssetBundleObject : CAbstractRefObject<CAssetBundleObject>
    {
        private static List<CAssetBundleObject> pool = new List<CAssetBundleObject>();
        public AssetBundle assetBundle;
        /// <summary>
        /// 上次做GC的时间
        /// </summary>
        protected static float _lastGcTime = -1;

        /// <summary>
        /// 取一个实例
        /// </summary>
        public static CAssetBundleObject Get(string name, AssetBundle assetBundle)
        {
            CAssetBundleObject instance = null;
            if (pool.Count > 0)
            {
                instance = pool[0];
                pool.RemoveAt(0);
            }
            else
                instance = new CAssetBundleObject();

            instance.assetBundle = assetBundle;
            instance.name = name;
            instance.AddRef();

            return instance;
        }

        public CAssetBundleObject()
        {

        }

        public override void AddRef()
        {
            base.AddRef();
#if DEBUG
            CAssetBundleLog.Log(string.Format("addref,abName={0},refCount={1}", assetBundle.name, refCount));
#endif
        }

        /// <summary>
        /// 外部GameObject销毁时调用该方法
        /// </summary>
        public void Release()
        {
            refCount--;
#if DEBUG
            CAssetBundleLog.Log(string.Format("release,abName={0},refCount={1}", assetBundle.name, refCount));
#endif
            if (refCount <= 0 && !MbAssetBundleManager.Instance.systemAssetBundleObject.ContainsKey(this.name))
            {
#if DEBUG
                UnityEngine.Assertions.Assert.AreEqual(0, refCount, string.Format("refCount should equals 0,refCount={0},name={1}", refCount,this.name));
#endif
                //即将被销毁
                MbAssetBundleManager.Instance.ReleaseAssetBundleObject(this.name);
                unUsedAssetBundleObjects[this] = Time.time;
                isReadyDisposed = true;
            }
        }

        public override void Dispose()
        {
            //CAssetBundleLog.LogWarning("CAssetBundleObject 销毁：" + this.name);
            assetBundle.Unload(false);
            assetBundle = null;
            refCount = 0;
            //用完了回收进池
            CAssetBundleObject.pool.Add(this);
        }

        /// <summary>
        /// 准备进行垃圾回收的AB资源
        /// </summary>
        public static readonly Dictionary<CAssetBundleObject, float> unUsedAssetBundleObjects = new Dictionary<CAssetBundleObject, float>();
        /// <summary>
        /// 缓存起来要删掉的，供DoGarbageCollect函数用, 避免重复的new List
        /// </summary>
        public static readonly List<CAssetBundleObject> cacheToRemoveFromUnUsed = new List<CAssetBundleObject>();

        ///// <summary>
        ///// 是否进行垃圾收集,在Update里调用
        ///// </summary>
        public static void CheckGcCollect()
        {
            if (_lastGcTime == -1 || (Time.time - _lastGcTime) >= GcIntervalTime)
            {
                DoGarbageCollect(false);
                _lastGcTime = Time.time;
            }
        }

        /// <summary>
        /// 进行垃圾回收
        /// </summary>
        internal static void DoGarbageCollect(bool forceClear)
        {
            cacheToRemoveFromUnUsed.Clear();

            foreach (var kv in unUsedAssetBundleObjects)
            {
                var assetObj = kv.Key;
                var time = kv.Value;
                if ((Time.time - time) >= _DISPOSE_DELAY_TIME || forceClear)
                {
                    cacheToRemoveFromUnUsed.Add(assetObj);
                }
            }

            for (var i = cacheToRemoveFromUnUsed.Count - 1; i >= 0; i--)
            {
                try
                {
                    var assetObj = cacheToRemoveFromUnUsed[i];
                    unUsedAssetBundleObjects.Remove(assetObj);
                    cacheToRemoveFromUnUsed.RemoveAt(i);
                    assetObj.Dispose();
                }
                catch (System.Exception e)
                {
                    CAssetBundleLog.LogError(e.ToString());
                }
            }

            if (cacheToRemoveFromUnUsed.Count > 0)
            {
                CAssetBundleLog.LogError("[DoGarbageCollect]CacheAssetObjectToRemoveFromUnUsed should be empty!!");
            }
        }
    }
}
