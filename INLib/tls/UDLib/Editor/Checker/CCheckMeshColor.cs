using System.IO;
using UnityEditor;
using UnityEngine;
namespace UDLib.Editor
{
    public class CCheckMeshColor
    {
        private const string notice1 =
            @"-----------------------------------说明-----------------------------------
                                    检查所有含有Color的Mesh";
        private const string notice2 =
            @"-----------------------------------说明-----------------------------------
                                    检查选中含有Color的Mesh";
        private const string fileName = "含有Color的Mesh";

        [MenuItem("Assets/美术/检查工具/检查所有含有Color的Mesh", false, 03030021)]
        [MenuItem("美术/检查工具/检查所有含有Color的Mesh", false, 03030021)]
        public static void CheckALLModel()
        {
            CCheckTools.ClearLogList();
            string[] files = Directory.GetFiles(CPath.AssetsApp, "*.*", SearchOption.AllDirectories);
            int fileCount = files.Length;
            int curIndex = 0;

            foreach (string f in files)
            {
                CCheckTools.DisplayProgressBar("处理中", f, curIndex / (float)fileCount);
                curIndex++;
                CheckColorByPath(f);
            }
            CCheckTools.ClearProgressBar();
            CCheckTools.LogAll();
            CCheckTools.WriteAllInfoToFile(fileName, notice1);
        }

        [MenuItem("Assets/美术/检查工具/检查选中含有Color的Mesh", false, 03030021)]
        [MenuItem("美术/检查工具/检查选中含有Color的Mesh", false, 03030021)]
        public static void CheckSelectedModel()
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
                    CheckColorByPath(s);
                }
                CCheckTools.ClearProgressBar();
            }
            else if (!string.IsNullOrEmpty(defaultPath))
            {
                CheckColorByPath(defaultPath);
            }
            else
            {
                Debug.LogError("未选择任何文件");
            }
            CCheckTools.LogAll();
            CCheckTools.WriteAllInfoToFile(fileName, notice2);
        }

        public static void CheckColorByPath(string path)
        {
            if (!File.Exists(path) || !path.EndsWith(".prefab")) return;
            path = path.Replace('\\', '/');
            GameObject root = AssetDatabase.LoadAssetAtPath<UnityEngine.Object>(path) as GameObject;
            if (root == null) return;
            CheckColorByRoot(root.transform);
        }
        public static void CheckColorByRoot(Transform root)
        {
            if (root == null) return;
            MeshFilter mf = root.GetComponent<MeshFilter>();
            SkinnedMeshRenderer smr = root.GetComponent<SkinnedMeshRenderer>();
            if (mf != null && mf.sharedMesh != null)
            {
                if(mf.sharedMesh.colors.Length > 0 || mf.sharedMesh.colors32.Length > 0)
                {
                    string logStr = CCheckTools.GetCompletePath(root);
                    CCheckTools.AddLogInfo(logStr);
                }
            }
            for (int i = 0; i < root.transform.childCount; i++)
            {
                CheckColorByRoot(root.transform.GetChild(i));
            }
        }
    }
}
