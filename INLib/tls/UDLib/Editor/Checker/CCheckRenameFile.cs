using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
namespace UDLib.Editor
{
    public class CCheckRenameFile
    {
        public const string notice1 = @"-----------------------------------说明-----------------------------------
检查特定目录下的重名文件，区分类型，不区分大小写。";
        public static int fileCount = 0;
        public static string[] typeFilter = { "prefab" };

        [MenuItem("Assets/程序/检查工具/检查选中目录下的同名文件", false, 02010041)]
        [MenuItem("程序/检查工具/检查选中目录下的同名文件", false, 02010041)]
        public static void CheckRenameFile()
        {
            Debug.Log(notice1);
            string[] filePath = null;
            string defaultPath = "";
            int curIndex = 0;
            CCheckTools.GetSelectedFilePath(ref filePath, ref defaultPath);
            if (filePath != null && filePath.Length > 0)
            {
                fileCount = filePath.Length;
                string[] objArray = new string[filePath.Length];   //筛选文件并获取文件路径
                int count = 0;  //待检查文件总数
                for (int i = 0; i < filePath.Length; i++)
                {
                    if (IsCheck(filePath[i]))
                    {
                        objArray[count++] = filePath[i];
                    }
                }
                fileCount = count * 2;
                curIndex = count;

                CCheckTools.DisplayProgressBar("检查中", "快速排序...", 1 / 2.0f);
                //先排序，再找重名
                QuickSort(ref objArray, 0, count - 1);
                for (int i = 0; i < count; i++)
                {
                    curIndex++;
                    CCheckTools.DisplayProgressBar("检查中", objArray[i], curIndex / (float)fileCount);
                    if (i + 1 < count && GetObjLowerFileName(objArray[i]).Equals(GetObjLowerFileName(objArray[i + 1])))
                    {
                        string log = objArray[i];
                        while (i + 1 < count && GetObjLowerFileName(objArray[i]).Equals(GetObjLowerFileName(objArray[i + 1])))
                        {
                            log = log + "==" + objArray[i + 1];
                            i++;
                            curIndex++;
                        }
                        Debug.LogError(log);
                    }
                }
                CCheckTools.ClearProgressBar();
            }
            else
            {
                Debug.LogError("你没有选择文件夹或只选择了一个文件");
            }
        }

        /// <summary>
        /// 快速排序
        /// </summary>
        /// <param name="objArray"></param>
        /// <param name="left"></param>
        /// <param name="right"></param>
        public static void QuickSort(ref string[] objArray, int left, int right)
        {
            if (left > right)
                return;
            int i, j;
            string t;
            string temp = objArray[left];
            i = left;
            j = right;
            while (i != j)
            {
                while (GetObjLowerFileName(objArray[j]).CompareTo(GetObjLowerFileName(temp)) >= 0 && i < j)
                    j--;
                while (GetObjLowerFileName(objArray[i]).CompareTo(GetObjLowerFileName(temp)) <= 0 && i < j)
                    i++;
                if (i < j)
                {
                    t = objArray[i];
                    objArray[i] = objArray[j];
                    objArray[j] = t;
                }
            }
            objArray[left] = objArray[i];
            objArray[i] = temp;
            QuickSort(ref objArray, left, i - 1);
            QuickSort(ref objArray, i + 1, right);
        }


        public static string GetObjLowerFileName(string obj)
        {
            string[] s = obj.Split('/');
            return s[s.Length - 1].ToLower();
        }

        public static bool IsCheck(string path)
        {
            for (int i = 0; i < typeFilter.Length; i++)
            {
                if (path.EndsWith(typeFilter[i]))
                    return true;
            }
            return false;
        }
    }
}

