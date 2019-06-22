using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UDLib.Utility;

namespace UDLib.Editor
{
    public class CFindReferencesInProject
    {
        private static Dictionary<string, List<string>> referenceCache = new Dictionary<string, List<string>>();
        private static ReferenceAsset referenceAsset;

        public static void Clear()
        {
            referenceCache.Clear();
        }

        public static ReferenceAsset GetReference()
        {
            return referenceAsset;
        }

        //查找引用关系
        public static void Find(string assetPath)
        {
            if (string.IsNullOrEmpty(assetPath))
            {
                CDebugOut.LogError("assetPath is null or empty!");
                return;
            }

            string[] dependencies = AssetDatabase.GetDependencies(assetPath, false);

            if (referenceCache.ContainsKey(assetPath) == false)
                referenceCache[assetPath] = new List<string>() { };

            foreach (var dependency in dependencies)
            {
                if (referenceCache.ContainsKey(dependency))
                {
                    if (!referenceCache[dependency].Contains(assetPath))
                    {
                        referenceCache[dependency].Add(assetPath);
                    }
                }
                else
                {
                    referenceCache[dependency] = new List<string>() { assetPath };
                }
            }
        }

        //生成引用关系文件，保存asset
        public static void CreateReferenceAsset()
        {
            referenceAsset = ScriptableObject.CreateInstance<ReferenceAsset>();
            referenceAsset.infoList = new List<ReferenceInfo>();

            foreach (string assetPath in referenceCache.Keys)
            {
                ReferenceInfo info = new ReferenceInfo();
                info.path = assetPath;
                info.guid = AssetDatabase.AssetPathToGUID(info.path);
                info.parents = new List<string>();
                foreach (string referencerPath in referenceCache[assetPath])
                {
                    ReferenceInfo referencerInfo = new ReferenceInfo();
                    referencerInfo.path = referencerPath;
                    referencerInfo.guid = AssetDatabase.AssetPathToGUID(referencerInfo.path);
                    info.parents.Add(referencerPath);
                }
                referenceAsset.infoList.Add(info);
            }

            AssetDatabase.CreateAsset(referenceAsset, "Assets/ReferenceAsset.asset");
            //AssetDatabase.SaveAssets();
            //AssetDatabase.Refresh();
        }

        private static List<string> allreadyFoundPaths;

        //查找根节点
        public static List<string> FindRoots()
        {
            allreadyFoundPaths = new List<string>();
            List<string> roots = new List<string>();

            foreach (ReferenceInfo info in referenceAsset.infoList)
            {
                string root = FindRoot(info);
                if (!string.IsNullOrEmpty(root))
                    roots.Add(root);
            }

            return roots;
        }

        private static string FindRoot(ReferenceInfo info)
        {
            if (allreadyFoundPaths.Contains(info.path)) return null;

            allreadyFoundPaths.Add(info.path);

            string path = info.path;
            while (info.parents != null && info.parents.Count > 0)
            {
                foreach (string parentPath in info.parents)
                {
                    ReferenceInfo parentInfo = getReferenceInfoByPath(parentPath);
                    return FindRoot(parentInfo);
                }
            }
            return path;
        }

        public static ReferenceInfo getReferenceInfoByPath(string path)
        {
            foreach (ReferenceInfo info in referenceAsset.infoList)
            {
                if (info.path == path)
                    return info;
            }
            return null;
        }

    }
}