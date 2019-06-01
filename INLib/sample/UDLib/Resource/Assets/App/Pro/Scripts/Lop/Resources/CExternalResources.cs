using System;
using UnityEngine;
using System.Collections;
using System.IO;

namespace UDLib.Resource
{
    /// <summary>
    /// 从Resrouces或ExternalResources加载资源
    /// 注意:"Assets/AssetBundleX/ExternalResources"这个路径在具体项目里用的时候需要改一下
    /// </summary>
    public class CExternalResources : UDLib.Resource.IExternalResources
    {
        /// <summary>
        /// 如果Resources和ExternalResources都没有资源，它将返回null。
        /// </summary>
        public static T Load<T>(string path) where T : UnityEngine.Object
        {
            var asset = Resources.Load<T>(path);
            if (asset != null)
            {
                return asset;
            }
#if UNITY_EDITOR
            asset = ExternalLoad<T>(path);
            if (asset != null)
                return asset;
#endif
            return null;
        }

        public static IEnumerator LoadAsync<T>(string path, Action<ResourceRequest> progress, Action<T> complete) where T : UnityEngine.Object
        {
            var request = Resources.LoadAsync<T>(path);
            if (!request.isDone)
            {
                progress(request);
                yield return null;
            }

            if (request.asset != null)
            {
                complete((T)request.asset);
                yield break;
            }
#if UNITY_EDITOR
            complete(ExternalLoad<T>(path));
#endif
        }

#if UNITY_EDITOR
        /// <summary>
        /// 从ExternalResource加载资源
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="path"></param>
        /// <returns></returns>
        static T ExternalLoad<T>(string path) where T : UnityEngine.Object
        {
            //CAssetBundleLog.LogWarning(string.Format("++++++++++++++++load direct : {0}", path));
            var basename = Path.GetFileName(path);
            var dirs = path.Split('/');
            var parentDir = string.Join("/", dirs, 0, dirs.Length - 1);
            //if (parentDir.IndexOf("Assets/App/Pro/GameRes", StringComparison.OrdinalIgnoreCase) == -1)
            //{
            //    parentDir = Path.Combine("Assets/App/Pro/GameRes", string.Join("/", dirs, 0, dirs.Length - 1));
            //}
            if (!Directory.Exists(parentDir))
            {
                return null;
            }
            var files = Directory.GetFiles(parentDir, basename + ".*", SearchOption.TopDirectoryOnly);
            var targetPath = "";
            for (int i = 0, il = files.Length; i < il; ++i)
            {
                if (Path.GetExtension(files[i]) == ".meta")
                {
                    continue;
                }
                targetPath = files[i];
                break;
            }
            if (string.IsNullOrEmpty(targetPath))
            {
                return null;
            }

            if(targetPath.ToLower().Contains("/ui/"))
                return UnityEditor.AssetDatabase.LoadAssetAtPath<UnityEngine.Sprite>(targetPath) as T;

            return UnityEditor.AssetDatabase.LoadAssetAtPath<T>(targetPath);
        }

#endif
        public UnityEngine.Object LoadAsset(string path)
        {
            UnityEngine.Object obj = null;
            try
            {
                obj = Load<UnityEngine.Object>(path);
            }
            catch (Exception e)
            {
                UDLib.Resource.CAssetBundleLog.LogError(string.Format("加载资源出错，path=：{0},reason={1}", path,e.Message));
            }
            return obj;
        }
    }
}