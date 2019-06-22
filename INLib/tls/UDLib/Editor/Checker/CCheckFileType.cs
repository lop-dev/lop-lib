using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;
namespace UDLib.Editor
{
    public class CCheckFileType : MonoBehaviour
    {
        // 工具基本不使用，先注释掉菜单
        //[MenuItem("Assets/程序/检查工具/检查选中目录下包含的文件类型", false, 02010041)]
        //[MenuItem("程序/检查工具/检查选中目录下包含的文件类型", false, 02010041)]
        public static void CheckFileType()
        {
            CCheckTools.ClearLogList();
            int fileCount;
            int curIndex = 0;
            object[] objArray = Selection.GetFiltered(typeof(DefaultAsset), SelectionMode.Assets);
            if (null == objArray || objArray.Length == 0)
            {
                Debug.LogError("没有选中任何文件夹");
                return;
            }
            string path = AssetDatabase.GetAssetPath(objArray[0] as Object);
            string[] files = Directory.GetFiles(path, "*.*", SearchOption.AllDirectories);
            fileCount = files.Length;
            List<string> typeList = new List<string>();
            foreach (string fp in files)
            {
                CCheckTools.DisplayProgressBar("检查中", fp, curIndex / (float)fileCount);
                curIndex++;
                if (fp.IndexOf('.') < 0)
                    continue;
                string type = fp.Split('.')[fp.Split('.').Length - 1];
                if (type.IndexOf('/') >= 0 || type.IndexOf('\\') >= 0)
                    continue;       //排除xxx.xxx/yyy文件
                if (!typeList.Exists(s => s.Equals(type)))
                {
                    typeList.Add(type);
                }
            }
            foreach (string s in typeList)
            {
                CCheckTools.AddLogInfo(s);
            }
            CCheckTools.LogAll();
            CCheckTools.WriteAllInfoToFile("文件类型", "", CCheckTools.EFileType.txt);
            CCheckTools.ClearProgressBar();
        }
    }
}

