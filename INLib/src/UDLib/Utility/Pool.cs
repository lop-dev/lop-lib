using System;
using System.Collections.Generic;
namespace UDLib.Utility
{
    public class Pool<T> where T : class, new()
    {
        public readonly int max;
        readonly Stack<T> freeObjects;

        public int Count { get { return freeObjects.Count; } }
        public int Peak { get; private set; }

        public Pool(int initialCapacity = 16, int max = int.MaxValue)
        {
            freeObjects = new Stack<T>(initialCapacity);
            this.max = max;
        }

        public T Obtain()
        {
            return freeObjects.Count == 0 ? new T() : freeObjects.Pop();
        }

        public void Free(T obj)
        {
            if (obj == null) throw new ArgumentNullException("obj", "obj cannot be null");
            if (freeObjects.Count < max)
            {
                freeObjects.Push(obj);
                Peak = Math.Max(Peak, freeObjects.Count);
            }
            Reset(obj);
        }

        protected void FreeAll(List<T> objects)
        {
            if (objects == null) throw new ArgumentNullException("objects", "objects cannot be null.");
            var freeObjects = this.freeObjects;
            int max = this.max;
            for (int i = 0; i < objects.Count; i++)
            {
                T obj = objects[i];
                if (obj == null) continue;
                if (freeObjects.Count < max) freeObjects.Push(obj);
                Reset(obj);
            }
            Peak = Math.Max(Peak, freeObjects.Count);
        }

        public void Clear()
        {
            freeObjects.Clear();
        }

        protected void Reset(T obj)
        {
            var poolable = obj as IPoolable;
            if (poolable != null) poolable.Reset();
        }

        public interface IPoolable
        {
            void Reset();
        }
    }
}