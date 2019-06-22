using UnityEngine;
using System.Collections.Generic;

namespace UDLib.Resource
{
    /// <summary>
    /// 从Ab文件里Load出来的资源信息
    /// </summary>
    public class CAssetObject : CAbstractRefObject<CAssetObject>
    {
        private static List<CAssetObject> pool = new List<CAssetObject>();
        /// <summary>
        /// _ASSET_BNDLE_.txt第一行的分类
        /// </summary>
        public int eResourceType;
        /// <summary>
        /// 从Ab里load出来的资源
        /// </summary>
        public UnityEngine.Object obj;

        /// <summary>
        /// 上次做GC的时间
        /// </summary>
        protected static float _lastGcTime = -1;

        /// <summary>
        /// 取一个实例
        /// </summary>
        public static CAssetObject Get(int eResourceType, string name, UnityEngine.Object obj)
        {
            CAssetObject instance = null;
            if (pool.Count > 0)
            {
                instance = pool[0];
                pool.RemoveAt(0);
            }
            else
                instance = new CAssetObject();

            instance.eResourceType = eResourceType;
            instance.obj = obj;
            instance.name = name;
            instance.AddRef();

            return instance;
        }

        public CAssetObject()
        {
            Reset();
            //对于这些数据用二叉树比哈希表要快30%
            //这里count+1是因为和服务器约定好的
        }

        private void Reset()
        {
            //TODO：这里还可以xxx处理下，但是我没有做
            this.eResourceType = MbAssetBundleManager.Instance.ResourceDefine.GetResourceTypeMaxValue();
            this.obj = null;
            this.name = string.Empty;
        }

        /// <summary>
        /// 外部GameObject销毁时调用该方法
        /// </summary>
        public void Release()
        {
            refCount--;
            if (refCount <= 0)
            {
                UnityEngine.Assertions.Assert.AreEqual(0, refCount, string.Format("refCount should equals 0,refCount={0}", refCount));
                //即将被销毁
                unUsedAssetObjects[this] = Time.time;
                MbAssetBundleManager.Instance.ReleaseAssetObject(eResourceType, name);
                isReadyDisposed = true;
            }
        }

        public override void Dispose()
        {
            //CAssetBundleLog.LogWarning("CAssetObject 销毁：" + this.name);
            //Resources.UnloadAsset(obj);
            Reset();
            //用完了回收进池
            CAssetObject.pool.Add(this);
        }

        /// <summary>
        /// 准备进行垃圾回收的AB资源
        /// </summary>
        public static readonly Dictionary<CAssetObject, float> unUsedAssetObjects = new Dictionary<CAssetObject, float>();
        /// <summary>
        /// 缓存起来要删掉的，供DoGarbageCollect函数用, 避免重复的new List
        /// </summary>
        public static readonly List<CAssetObject> cacheToRemoveFromUnUsed = new List<CAssetObject>();

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

            foreach (var kv in unUsedAssetObjects)
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
                    unUsedAssetObjects.Remove(assetObj);
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
