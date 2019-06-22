using System.IO;
using UnityEditor;
using UnityEngine;
namespace UDLib.Editor
{
    public class CCheckUVCount
    {
        private const string notice1 =
            @"-----------------------------------说明-----------------------------------
                                     检查所有UV个数大于等于2的物体";
        private const string notice2 =
            @"-----------------------------------说明-----------------------------------
                                     检查选中UV个数大于等于2的物体";
        private const string fileName = "UV个数";

        [MenuItem("Assets/美术/检查工具/检查所有预设体的UV个数", false, 03030201)]
        [MenuItem("美术/检查工具/检查所有预设体的UV个数", false, 03030201)]
        public static void CheckALLUVCount()
        {
            CCheckTools.ClearLogList();
            string[] files = Directory.GetFiles(CPath.AssetsApp, "*.*", SearchOption.AllDirectories);
            int fileCount = files.Length;
            int curIndex = 0;

            foreach (string f in files)
            {
                CCheckTools.DisplayProgressBar("处理中", f, curIndex / (float)fileCount);
                curIndex++;
                CheckUVByPath(f);
            }
            CCheckTools.ClearProgressBar();
            CCheckTools.LogAll();
            CCheckTools.WriteAllInfoToFile(fileName, notice1);
        }

        [MenuItem("Assets/美术/检查工具/检查选中预设体的UV个数", false, 03030201)]
        [MenuItem("美术/检查工具/检查选中预设体的UV个数", false, 03030201)]
        public static void CheckSelectedUVCount()
        {
            CCheckTools.ClearLogList();
            int curIndex = 0;
            int fileCount;
            string[] filePath = null;
            string defaultPath = "";
            CCheckTools.GetSelectedFilePath(ref filePath, ref defaultPath);
            if (filePath != null && filePath.Length > 0)
            {
                fileCount = filePath.Length;
                foreach (string s in filePath)
                {
                    curIndex++;
                    CCheckTools.DisplayProgressBar("检查中", s, curIndex / (float)fileCount);
                    CheckUVByPath(s);
                }
                CCheckTools.ClearProgressBar();
            }
            else if (!string.IsNullOrEmpty(defaultPath))
            {
                CheckUVByPath(defaultPath);
            }
            else
            {
                Debug.LogError("未选择任何文件");
            }
            CCheckTools.LogAll();
            CCheckTools.WriteAllInfoToFile(fileName, notice2);
        }

        public static void CheckUVByPath(string path)
        {
            if (!File.Exists(path) || !path.EndsWith(".prefab")) return;
            path = path.Replace('\\', '/');
            GameObject root = AssetDatabase.LoadAssetAtPath<UnityEngine.Object>(path) as GameObject;
            if (root == null) return;
            CheckUVByRoot(root.transform);
        }
        public static void CheckUVByRoot(Transform root)
        {
            if (root == null) return;
            MeshFilter mf = root.GetComponent<MeshFilter>();
            SkinnedMeshRenderer smr = root.GetComponent<SkinnedMeshRenderer>();
            int count = 0;
            if (mf != null && mf.sharedMesh != null)
            {
                if (mf.sharedMesh.uv.Length > 0) count++;
                if (mf.sharedMesh.uv2.Length > 0) count++;
                if (mf.sharedMesh.uv3.Length > 0) count++;
                if (mf.sharedMesh.uv4.Length > 0) count++;
                if (mf.sharedMesh.uv5.Length > 0) count++;
                if (mf.sharedMesh.uv6.Length > 0) count++;
                if (mf.sharedMesh.uv7.Length > 0) count++;
                if (mf.sharedMesh.uv8.Length > 0) count++;
            }
            if (smr != null && smr.sharedMesh != null)
            {
                if (smr.sharedMesh.uv.Length > 0) count++;
                if (smr.sharedMesh.uv2.Length > 0) count++;
                if (smr.sharedMesh.uv3.Length > 0) count++;
                if (smr.sharedMesh.uv4.Length > 0) count++;
                if (smr.sharedMesh.uv5.Length > 0) count++;
                if (smr.sharedMesh.uv6.Length > 0) count++;
                if (smr.sharedMesh.uv7.Length > 0) count++;
                if (smr.sharedMesh.uv8.Length > 0) count++;
            }
            if (count >= 2)
            {
                string logStr = CCheckTools.GetCompletePath(root);
                CCheckTools.AddLogInfo(logStr + ":uv Count=" + count);
            }
            for (int i = 0; i < root.transform.childCount; i++)
            {
                CheckUVByRoot(root.transform.GetChild(i));
            }
        }
    }
}
