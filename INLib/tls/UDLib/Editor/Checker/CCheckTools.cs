using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;
using UnityEditor;
using UnityEngine;
namespace UDLib.Editor
{
    public class CCheckTools
    {
        private static List<string> logList = new List<string>();
        private static List<string> errorList = new List<string>();
        private static List<string> warningList = new List<string>();

        public enum EFileType
        {
            txt,
            csv
        }

        public static void ClearLogList()
        {
            if (null == logList)
                logList = new List<string>();
            else
                logList.Clear();

            if (null == errorList)
                errorList = new List<string>();
            else
                errorList.Clear();

            if (null == warningList)
                warningList = new List<string>();
            else
                warningList.Clear();
        }

        public static void AddLogInfo(string info)
        {
            if (null == logList)
                logList = new List<string>();
            logList.Add(info);
        }
        public static void AddWarningInfo(string info)
        {
            if (null == warningList)
                warningList = new List<string>();
            warningList.Add(info);
        }
        public static void AddErrorInfo(string info)
        {
            if (null == errorList)
                errorList = new List<string>();
            errorList.Add(info);
        }

        public static void LogAll()
        {
            foreach (string log in logList)
            {
                Debug.Log(log);
            }
            foreach (string warning in warningList)
            {
                Debug.LogWarning(warning);
            }
            foreach (string error in errorList)
            {
                Debug.LogError(error);
            }
        }


        public static void WriteAllInfoToFile(string name, string notice = "", EFileType type = EFileType.txt)
        {
            if (!IsNameLegal(name))
            {
                Debug.LogError("文件名非法");
                return;
            }
            string fileType;
            switch (type)
            {
                case EFileType.txt:
                    fileType = ".txt";
                    break;
                case EFileType.csv:
                    fileType = ".csv";
                    break;
                default:
                    fileType = ".txt";
                    break;
            }
            string fullPath = CPath.InstallPackLog + name + fileType;
            StreamWriter sw = new StreamWriter(fullPath);
            if (!string.IsNullOrEmpty(notice))
                sw.WriteLine(notice);
            if (logList.Count > 0 && type == EFileType.txt)
                sw.WriteLine("Log:");
            foreach (string log in logList)
            {
                sw.WriteLine(log);
            }
            if (logList.Count > 0)
                sw.WriteLine();

            if (warningList.Count > 0 && type == EFileType.txt)
                sw.WriteLine("Warning:");
            foreach (string warning in warningList)
            {
                sw.WriteLine(warning);
            }
            if (warningList.Count > 0)
                sw.WriteLine();

            if (errorList.Count > 0 && type == EFileType.txt)
                sw.WriteLine("Error:");
            foreach (string error in errorList)
            {
                sw.WriteLine(error);
            }
            if (errorList.Count > 0)
                sw.WriteLine();
            sw.Close();
            AssetDatabase.Refresh();
        }

        public static void DisplayProgressBar(string title, string info, float percent)
        {
            EditorUtility.DisplayProgressBar(title, info, percent);
        }

        public static void ClearProgressBar()
        {
            EditorUtility.ClearProgressBar();
        }

        //获取给定文件夹下的文件数量（包括子文件夹内文件）
        public static int GetDirectionFilesCount(string path)
        {
            if (!Directory.Exists(path))
                return 0;
            string[] file = Directory.GetFiles(path, "*.*", SearchOption.AllDirectories);
            return file.Length;
        }

        public static string GetCompletePath(Transform root)
        {
            if (null == root)
                return "";
            string path = root.name;
            Transform pr = root;
            while (pr.parent != null)
            {
                pr = pr.parent;
                path = pr.name + "/" + path;
            }
            return path;
        }

        //将路径转为Unity相对路径
        public static string ConvertFullPathToUnityPath(string fullPath)
        {
            int startIndex = fullPath.IndexOf("Assets");
            string path = fullPath.Substring(startIndex, fullPath.Length - startIndex);
            return path.Replace(@"\", "/");
        }

        //检查文件名是否合法
        public static bool IsNameLegal(string name)
        {
            //是否为空
            if (string.IsNullOrEmpty(name))
                return false;

            //是否包含非法字符
            Regex regex = new Regex(@"[\/:*?<>|]");
            if (regex.Match(name).Success)
                return false;

            //长度是否大于251（包含.txt不大于255）
            if (name.Length > 251)
                return false;

            return true;
        }

        /// <summary>
        /// 获取当前选定文件夹下的T类型文件，返回文件路径数组。如果选中文件，则返回文件路径
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="path">类型文件路径数组</param>
        /// <param name="defaultPath">选中文件路径</param>
        public static void GetSelectedFilePath(ref string[] path, ref string defaultPath, string searchPattern = "*.*")
        {
            object[] objArray = Selection.GetFiltered(typeof(DefaultAsset), SelectionMode.Assets);
            if (null != objArray && objArray.Length > 0)
            {
                string path1 = AssetDatabase.GetAssetPath(objArray[0] as Object);
                path = Directory.GetFiles(path1, searchPattern, SearchOption.AllDirectories);
                if (path == null)
                    return;
                for (int i = 0; i < path.Length; i++)
                {
                    path[i] = path[i].Replace('\\', '/');
                }
            }
            else
            {
                Object[] obj = Selection.GetFiltered(typeof(Object), SelectionMode.Assets);
                if (obj != null && obj.Length > 0)
                {
                    defaultPath = AssetDatabase.GetAssetPath(obj[0]);
                    defaultPath = defaultPath.Replace("\\", "/");
                }
            }
        }
    }
}

