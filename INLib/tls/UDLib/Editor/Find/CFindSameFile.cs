/* ***********************************************
* author : fanghy
* history : created at 10/23/2018 1:59:06 PM
* description :
* ***********************************************/

using System.Collections.Generic;
using System.IO;
using System.Text;
using UnityEditor;
using UnityEngine;

namespace ACGNStudio
{
    public class CFindSameFile
    {
        private static string mFileName = "CheckSameFile_";
        private static Dictionary<string, List<string>> fileList = new Dictionary<string, List<string>>();

        [MenuItem("程序/检查工具/检查选中目录下的重复资源", false, 02010041)]
        public static void SetFormatForUISprite()
        {
            FindFiles();
            SaveResult();
        }

        private static void FindFiles()
        {
            // 收集前清空数据
            fileList.Clear();

            Object[] selectedAsset = Selection.GetFiltered(typeof(Object), SelectionMode.DeepAssets);
            foreach (Object obj in selectedAsset)
            {
                string file = AssetDatabase.GetAssetPath(obj);
                FileAttributes attr = File.GetAttributes(file);
                if((attr & FileAttributes.Directory) == FileAttributes.Directory)
                {
                    continue;
                }

                string md5 = CSLib.Security.CMd5.EncodeFile(file);
                if (!fileList.ContainsKey(md5))
                {
                    fileList.Add(md5, new List<string>());
                }

                var _list = fileList[md5];
                if (!_list.Contains(file))
                {
                    _list.Add(file);
                }
            }
        }

        private static void SaveResult()
        {
            //保存数据
            if(fileList != null && fileList.Count > 0)
            {
                string fileName = string.Format("{0}_{1}.txt", mFileName, GetTimestamp());
                StringBuilder sb = new StringBuilder();
                foreach(var kvp in fileList)
                {
                    string hint = string.Empty;
                    if(kvp.Value.Count > 1)
                    {
                        hint = string.Format(" 重复 X{0}", kvp.Value.Count);
                    }
                    sb.Append(kvp.Key).Append(":").Append(hint).Append("\r\n");
                    foreach(var value in kvp.Value)
                    {
                        string guid = AssetDatabase.AssetPathToGUID(value);
                        sb.Append("path:  ").Append(value).Append("   guid:  ").Append(guid).Append("\r\n");
                    }
                    sb.Append("\r\n\n");
                }

                string filePath = Path.Combine("Assets", fileName);
                File.WriteAllText(filePath, sb.ToString(),Encoding.UTF8);
                Debug.Log(string.Format("++++++++++++++++save file to {0}+++++++++++++++++++++", filePath));
            }


            // 回收清空数据
            fileList.Clear();
        }

        public static string GetTimestamp()
        {
            return System.DateTime.Now.ToString("yyyy-MM-dd-HH-mm-ss");
        }
    }
}
