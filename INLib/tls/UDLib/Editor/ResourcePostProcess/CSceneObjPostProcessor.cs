using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;

namespace ACGNStudio
{
    public class CSceneObjPostProcessor : UnityEditor.AssetModificationProcessor
    {
        private static string checkPath = "Assets/App/Pro/GameRes/Scenes/SceneObjects/";
        private static Dictionary<string, string> containsFileList = new Dictionary<string, string>();
        static void OnWillSaveAssets(string[] paths)
        {
            bool initContainsFileList = false;
            foreach (string str in paths)
            {
                string fullName = str.Replace('\\', '/');
                if (fullName.StartsWith(checkPath))
                {
                    string[] temp = fullName.Split('/');
                    string name = temp[temp.Length - 1];
                    if (!initContainsFileList)
                    {
                        GetExistingFileList();
                        initContainsFileList = true;
                    }
                    foreach(var v in containsFileList)
                    {
                        if(v.Value == name && v.Key != fullName)
                        {
                            Debug.LogError("存在同名文件:" + fullName + "=="+v.Key);
                        }
                    }
                    if (!containsFileList.ContainsKey(fullName))
                        containsFileList.Add(fullName, name);
                }
            }
            containsFileList.Clear();
        }

        private static void GetExistingFileList()
        {
            string[] files = Directory.GetFiles(checkPath, "*.*",SearchOption.AllDirectories);
            foreach(string f in files)
            {
                if (f.EndsWith(".meta")) continue;
                string fullName = f.Replace("\\", "/");
                string []temp = fullName.Split('/');
                string name = temp[temp.Length - 1];
                containsFileList.Add(fullName, name);
            }
        }
    }
}
