using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;
using UnityEngine.SceneManagement;
namespace UDLib.Editor
{
    public class CCheckRenameComponent
    {
        public const string notice1 = @"-----------------------------------说明-----------------------------------
检查所有场景中的同名物体";
        public const string notice2 = @"-----------------------------------说明-----------------------------------
检查选中场景中的同名物体";

        public static Scene curScene;
        private static List<GameObject> list;

        //[MenuItem("Assets/检查工具/检查所有场景的同名物体", false, 340)]
        //[MenuItem("检查工具/检查所有场景的同名物体", false, 340)]
        public static void CheckRenameComponentInAllScenes()
        {
            int fileCount;
            int index = 0;
            string[] files = Directory.GetFiles(CPath.AssetsAppArtScenes, "*.unity", SearchOption.AllDirectories);
            fileCount = files.Length;
            list = new List<GameObject>();

            foreach (string f in files)
            {
                CCheckTools.DisplayProgressBar("处理中", f, index / (float)fileCount);
                index++;
                CheckRenameComponent(f);
            }
            CCheckTools.ClearProgressBar();
        }

        //[MenuItem("Assets/检查工具/检查选中场景的同名物体", false, 340)]
        //[MenuItem("检查工具/检查选中场景的同名物体", false, 340)]
        public static void CheckRenameComponentInSelectedScenes()
        {
            int fileCount; ;
            int curIndex = 0;
            list = new List<GameObject>();

            string[] filePath = null;
            string defaultPath = "";
            CCheckTools.GetSelectedFilePath(ref filePath, ref defaultPath, "*.unity");
            if (filePath != null && filePath.Length > 0)
            {
                fileCount = filePath.Length;
                foreach (string s in filePath)
                {
                    curIndex++;
                    CCheckTools.DisplayProgressBar("检查中", s, curIndex / (float)fileCount);
                    CheckRenameComponent(s);
                }
                CCheckTools.ClearProgressBar();
            }
        }

        private static void CheckRenameComponent(string path)
        {
            if (!File.Exists(path) || !path.EndsWith(".unity"))
            {
                return;
            }

            list.Clear();
            path = path.Replace('\\', '/');
            curScene = EditorSceneManager.OpenScene(path, OpenSceneMode.Single);
            Debug.Log("检查场景：" + path);
            foreach (GameObject root in curScene.GetRootGameObjects())
            {
                AppendGO(ref list, root);
            }
            int[] flag = new int[list.Count];
            string info;
            for (int i = 0; i < list.Count; i++)
            {
                if (flag[i] != default(int))
                    continue;
                flag[i] = default(int) + 1;
                info = GetGoPath(list[i]);
                for (int j = 0; j < list.Count; j++)
                {
                    if (flag[j] != default(int))
                        continue;
                    if (list[i].name == list[j].name)
                    {
                        info = info + "==" + GetGoPath(list[j]);
                        flag[j] = default(int) + 1;
                    }
                }
                if (info != GetGoPath(list[i]))
                {
                    Debug.Log(info);
                }
            }
        }

        private static void AppendGO(ref List<GameObject> list, GameObject root)
        {
            list.Add(root);
            for (int i = 0; i < root.transform.childCount; i++)
            {
                AppendGO(ref list, root.transform.GetChild(i).gameObject);
            }
        }

        private static string GetGoPath(GameObject go)
        {
            string p = go.transform.name;
            while (go != null && go.transform.parent != null)
            {
                p = go.transform.parent.name + "/" + p;
                go = go.transform.parent.gameObject;
            }
            return p;
        }
    }
}

