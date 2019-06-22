using UnityEngine;

namespace UDLib.Resource
{
    public interface IRefObj
    {
        void Dispose();
    }

    public abstract class CAbstractRefObject<T> : IRefObj where T: IRefObj
    {
        /// <summary>
        /// 引用计数
        /// </summary>
        public int refCount;

        public string name = string.Empty;

        /// <summary>
        /// refCount 为 0，进入预备销毁状态
        /// </summary>
        protected bool isReadyDisposed;
        /// <summary>
        /// AssetObject延迟Dispose的时间
        /// </summary>
        protected const float _DISPOSE_DELAY_TIME = 20;

        /// <summary>
        /// 间隔多少秒做一次GC(在AutoNew时)
        /// </summary>
        public static float GcIntervalTime
        {
            get
            {
                if (Application.platform == RuntimePlatform.WindowsEditor ||
                    Application.platform == RuntimePlatform.OSXEditor)
                    return 1f;

                return Debug.isDebugBuild ? 5f : 10f;
            }
        }

        public virtual void AddRef()
        {
            refCount++;
        }

        internal void Revive()
        {
            isReadyDisposed = false;//重新可用
        }

        public virtual void Dispose()
        {
            throw new System.Exception(string.Format("Please override in sub class!type={0}",this.GetType().FullName));
        }
    }
}