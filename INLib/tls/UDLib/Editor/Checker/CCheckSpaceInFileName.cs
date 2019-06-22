using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;

namespace UDLib.Editor
{
    public class CCheckSpaceInFileName
    {
        public static string defaultPathRoot = "Assets/";
        public static string fileName = "文件名包含空格文件列表";

        [MenuItem("Assets/美术/检查工具/检查所有预设体名称带空格或中文", false, 03030101)]
        [MenuItem("美术/检查工具/检查所有预设体名称带空格或中文", false, 03030101)]
        public static void CheckAllPrefab()
        {
            CheckAllFileByType("*.prefab");
        }

        [MenuItem("Assets/美术/检查工具/检查选中预设体名称带空格或中文", false, 03030101)]
        [MenuItem("美术/检查工具/检查选中预设体名称带空格或中文", false, 03030101)]
        public static void CheckSelectedPrefab()
        {
            CheckSelectedFileByType("*.prefab");
        }

        [MenuItem("Assets/美术/检查工具/检查所有文件名称带空格或中文", false, 03030121)]
        [MenuItem("美术/检查工具/检查所有文件名称带空格或中文", false, 03030121)]
        public static void CheckAllFiles()
        {
            CheckAllFileByType("*");
        }

        [MenuItem("Assets/美术/检查工具/检查选中文件名称带空格或中文", false, 03030121)]
        [MenuItem("美术/检查工具/检查选中文件名称带空格或中文", false, 03030121)]
        public static void CheckSelectedFiles()
        {
            CheckSelectedFileByType("*");
        }

        private static void CheckAllFileByType(string searchOption)
        {
            CCheckTools.ClearLogList();
            string[] files = Directory.GetFiles(defaultPathRoot, searchOption, SearchOption.AllDirectories);
            int fileCount = files.Length;
            int index = 0;

            foreach (string f in files)
            {
                CCheckTools.DisplayProgressBar("检查中", index + "/" + fileCount + " " + f, index / (float)fileCount);
                index++;
                //CheckFile(f);
                CheckChineseCharOrSpaceInFlieName(f);
            }
            CCheckTools.ClearProgressBar();
            CCheckTools.LogAll();
            CCheckTools.WriteAllInfoToFile(fileName,"所有类型为"+searchOption+"且文件名包含空格或汉字的文件：");
        }

        private static void CheckSelectedFileByType(string searchOption)
        {
            CCheckTools.ClearLogList();
            int index = 0;
            string[] filePath = null;
            string defaultPath = "";
            CCheckTools.GetSelectedFilePath(ref filePath, ref defaultPath, searchOption);
            if (filePath != null && filePath.Length > 0)
            {
                int fileCount = filePath.Length;
                foreach (string s in filePath)
                {
                    index++;
                    CCheckTools.DisplayProgressBar("检查中", index + "/" + fileCount + " " + s, index / (float)fileCount);
                    //CheckFile(s);
                    CheckChineseCharOrSpaceInFlieName(s);
                }
                CCheckTools.ClearProgressBar();
            }
            else if (!string.IsNullOrEmpty(defaultPath))
            {
                CheckFile(defaultPath);
            }
            else
            {
                Debug.LogError("未选择任何文件");
            }
            CCheckTools.LogAll();
            CCheckTools.WriteAllInfoToFile(fileName, "选中目录下，所有类型为" + searchOption + "且文件名包含空格的文件：");
        }

        private static void CheckFile(string path)
        {
            if (!File.Exists(path) || path.EndsWith("meta"))
            {
                return;
            }
            path = path.Replace('\\', '/');
            string[] temp = path.Split('/');
            string name =temp[temp.Length - 1];
            foreach (char c in name)
            {
                if (c.Equals(' '))
                {
                    CCheckTools.AddLogInfo(path);
                    break;
                }
            }
        }

        private static void CheckChineseCharOrSpaceInFlieName(string path)
        {
            if (!File.Exists(path) || path.EndsWith("meta"))
            {
                return;
            }
            path = path.Replace('\\', '/');
            string[] temp = path.Split('/');
            string name = temp[temp.Length - 1];
            foreach (char c in name)
            {
                if ((int)c > 127 || c.Equals(' '))
                {
                    CCheckTools.AddLogInfo(path);
                    break;
                }
            }
        }
    }
}