using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace UDLib.Resource
{
    public class CLoadQueue
    {
        List<CLoadObject> loadQueue = new List<CLoadObject>();
        //总共要加载的数量
        private int totalCount = 0;

        public CLoadQueue()
        {

        }

        //加载进度
        public float Progress
        {
            get {
                //每完成一个占多少比例
                float perProgress = 1.0f / totalCount;
                //完成了多少个
                int loadedCount = GetLoadedCount();
                if (loadedCount == totalCount)
                    return 1;
                else
                    return perProgress * loadedCount + perProgress * loadQueue[0].Progress;
            }
        }

        public CLoadObject CacheEffect(int eResourceType, string resource_name)
        {
            CLoadObject load_object = new CLoadObject(eResourceType, resource_name);

            if (load_object.isLoading)
            {
                loadQueue.Add(load_object);
                totalCount++;
            }

            return load_object;
        }

        public CLoadObject Load(int eResourceType, string resource_name)
        {
            CLoadObject load_object = new CLoadObject(eResourceType, resource_name);

            if (load_object.isLoading)
            {
                loadQueue.Add(load_object);
                totalCount++;
            }

            return load_object;
        }

        public CLoadObject Load(int eResourceType, string package_name, string[] resource_names)
        {
            CLoadObject load_object = new CLoadObject(eResourceType, package_name, resource_names);

            if (load_object.isLoading)
            {
                loadQueue.Add(load_object);
                totalCount++;
            } 

            return load_object;
        }

        public bool IsLoading()
        {
            int i = 0;
            while (i <= loadQueue.Count - 1)
            {
                if (loadQueue[i].isLoading == true)
                    return true;
                i++;
               // loadQueue.RemoveAt(i);
            }

            return false;
        }

        public int GetLoadedCount()
        {
            int count = 0;
            int i = 0;
            while (i < loadQueue.Count)
            {
                if (loadQueue[i].isLoading == false || loadQueue[i].Progress == 1)
                    count++;
                i++;
            }
            return count;
        }
    }
}
