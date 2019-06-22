using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;
using UnityEngine.SceneManagement;
namespace UDLib.Editor
{
    public class CCheckMaterials
    {
        private const string notice1 = @"-----------------------------------说明-----------------------------------
检查所有物体包含的材质数量，输出数量大于4个的物体。检查范围包括所有场景，所有预设体";
        private const string notice2 = @"-----------------------------------说明-----------------------------------
检查选中物体包含的材质数量，输出数量大于4个的物体。";
        private const string fileName = "物体材质数量";

        private static int SceneOrPrefab;
        private static List<string> countStr = new List<string>();
        private static List<string> repeatStr = new List<string>();

        [MenuItem("Assets/美术/检查工具/检查所有预设体的材质数", false, 03030141)]
        [MenuItem("美术/检查工具/检查所有预设体的材质数", false, 03030141)]
        public static void CheckAllMaterials()
        {
            CCheckTools.ClearLogList();
            countStr.Clear();
            repeatStr.Clear();
            string[] files = Directory.GetFiles(CPath.AssetsApp, "*.*", SearchOption.AllDirectories);
            int fileCount = files.Length;
            int curIndex = 0;

            foreach (string f in files)
            {
                CCheckTools.DisplayProgressBar("处理中", f, curIndex / (float)fileCount);
                curIndex++;
                CheckMaterials(f);
            }
            foreach (string s in countStr)
            {
                CCheckTools.AddErrorInfo(s);
            }
            foreach (string s in repeatStr)
            {
                CCheckTools.AddErrorInfo(s);
            }
            CCheckTools.ClearProgressBar();
            CCheckTools.LogAll();
            CCheckTools.WriteAllInfoToFile(fileName, notice1);
        }

        [MenuItem("Assets/美术/检查工具/检查选中预设体的材质数", false, 03030141)]
        [MenuItem("美术/检查工具/检查选中预设体的材质数", false, 03030141)]
        public static void CheckSelectedMaterials()
        {
            countStr.Clear();
            repeatStr.Clear();
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
                    CheckMaterials(s);
                }
                CCheckTools.ClearProgressBar();
            }
            else if (!string.IsNullOrEmpty(defaultPath))
            {
                CheckMaterials(defaultPath);
            }
            else
            {
                Debug.LogError("未选择任何文件");
            }
            foreach (string s in countStr)
            {
                CCheckTools.AddErrorInfo(s);
            }
            foreach (string s in repeatStr)
            {
                CCheckTools.AddErrorInfo(s);
            }
            CCheckTools.LogAll();
            CCheckTools.WriteAllInfoToFile(fileName, notice2);
        }


        private static void CheckMaterials(string path)
        {
            if (!File.Exists(path) || (!path.EndsWith(".unity") && !path.EndsWith(".prefab")))
                return;

            path = path.Replace('\\', '/');
            if (path.EndsWith(".unity"))
            {
                SceneOrPrefab = 1;
                Scene scene = EditorSceneManager.OpenScene(path, OpenSceneMode.Single);
                foreach (GameObject root in scene.GetRootGameObjects())
                {
                    CheckMaterialsByRoot(root);
                }
            }
            if (path.EndsWith(".prefab"))
            {
                SceneOrPrefab = 2;
                GameObject rootGO = AssetDatabase.LoadAssetAtPath<UnityEngine.Object>(path) as GameObject;
                CheckMaterialsByRoot(rootGO);
            }
        }


        private static void CheckMaterialsByRoot(GameObject root)
        {
            if (root == null)
                return;
            Renderer[] render = root.GetComponentsInChildren<Renderer>();
            string logStr = "";
            foreach (Renderer r in render)
            {
                if (r is ParticleSystemRenderer)
                    continue;
                logStr = CCheckTools.GetCompletePath(r.transform);
                if (SceneOrPrefab == 1)
                {
                    logStr = "Scene:" + SceneManager.GetActiveScene().name + "->" + logStr;
                }
                if (r.sharedMaterials != null)
                {
                    if (r.sharedMaterials.Length > 4)
                    {
                        countStr.Add(logStr + ":material count=" + r.sharedMaterials.Length);
                    }
                    if (MaterialsRepeat(r.sharedMaterials))
                    {
                        repeatStr.Add(logStr + ":material repeat");
                    }
                }
            }
        }

        private static bool MaterialsRepeat(Material[] material)
        {
            for (int i = 0; i < material.Length; i++)
            {
                for (int j = i + 1; j < material.Length; j++)
                {
                    if (material[i] != null && material[j] != null && material[i].name == material[j].name)
                    {
                        return true;
                    }
                }
            }
            return false;
        }
    }
}

