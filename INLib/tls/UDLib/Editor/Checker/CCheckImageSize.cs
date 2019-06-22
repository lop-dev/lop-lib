using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;
using System.Text;

namespace UDLib.Editor
{
    public class ImageInfo
    {
        public string sceneName;
        public string imageName;
        public float height;
        public float width;

        public ImageInfo(string _sceneName, string _name, float _height, float _width)
        {
            sceneName = _sceneName;
            imageName = _name;
            height = _height;
            width = _width;
        }
    }

    public class CCheckImageSize
    {
        private static string notice1 = @"-----------------------------------说明-----------------------------------
检查所有图片资源，输出长或宽大于最大尺寸的图片。最大尺寸：" + MaxSize + "警告尺寸：" + WarningSize;
        private const int MaxSize = 1024;
        private const int WarningSize = 512;
        private const string fileName = "图片尺寸";
        private static List<ImageInfo> mImageList = new List<ImageInfo>();
        private static string mCsvName = "图片检查.csv";
        private static string mSceneFlag = "App/Art/Scenes/";


        [MenuItem("Assets/程序/检查工具/检查所有图片的尺寸(512Warning,1024Error)", false, 03030181)]
        [MenuItem("程序/检查工具/检查所有图片的尺寸(512Warning,1024Error)", false, 03030181)]
        public static void CheckAllImageSize()
        {
            mImageList.Clear();
            CCheckTools.ClearLogList();
            string[] files = Directory.GetFiles(CPath.AssetsApp, "*.*", SearchOption.AllDirectories);
            int fileCount = files.Length;
            int curIndex = 0;
            foreach (string f in files)
            {
                CCheckTools.DisplayProgressBar("处理中", f, curIndex / (float)fileCount);
                curIndex++;
                CheckImageSize(f);
            }
            CCheckTools.ClearProgressBar();
            CCheckTools.LogAll();
            CCheckTools.WriteAllInfoToFile(fileName, notice1);
            //savaInExcel(mImageList, CPath.OutPutPath, csvName);
            savaInExcel(mImageList, Application.dataPath + "/ToolsOutput/", mCsvName);
        }

        [MenuItem("Assets/程序/检查工具/检查选中图片的尺寸(512Warning,1024Error)", false, 03030181)]
        [MenuItem("程序/检查工具/检查选中图片的尺寸(512Warning,1024Error)", false, 03030181)]
        public static void CheckSelectedImageSize()
        {
            mImageList.Clear();
            CCheckTools.ClearLogList();
            int fileCount = 0;
            int curIndex = 0;
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
                    CheckImageSize(s);
                }
                CCheckTools.ClearProgressBar();
            }
            else if (!string.IsNullOrEmpty(defaultPath))
            {
                CheckImageSize(defaultPath);
            }
            else
            {
                Debug.LogError("未选择任何文件");
            }
            CCheckTools.LogAll();
            CCheckTools.WriteAllInfoToFile(fileName, notice1);

            //savaInExcel(mImageList, CPath.OutPutPath, csvName);
            savaInExcel(mImageList, Application.dataPath + "/ToolsOutput/", mCsvName);
        }

        private static void CheckImageSize(string path)
        {
            if (!File.Exists(path) || !path.EndsWith(".png") && !path.EndsWith(".psd") && !path.EndsWith(".tga"))
            {
                return;
            }
            path = path.Replace('\\', '/');
            UnityEngine.Object obj = AssetDatabase.LoadAssetAtPath<UnityEngine.Object>(path);
            if (null == obj)
                return;
            Texture tex = obj as Texture;
            if (null != tex)
            {
                if (tex.width > MaxSize || tex.height > MaxSize)
                {
                    CCheckTools.AddErrorInfo(path + ":" + tex.width + "*" + tex.height);
                }
                else if (tex.width > WarningSize || tex.height > WarningSize)
                {
                    CCheckTools.AddWarningInfo(path + ":" + tex.width + "*" + tex.height);
                }

                if (path.Contains(mSceneFlag))
                {
                    int idx = path.IndexOf(mSceneFlag) + mSceneFlag.Length;
                    path = path.Substring(idx);
                    string[] pathArr = path.Split('/');
                    if (pathArr.Length > 0)
                    {
                        path = pathArr[0];
                    }
                    string sceneName = path;
                    ImageInfo imgInfo = new ImageInfo(sceneName, tex.name, tex.height, tex.width);
                    mImageList.Add(imgInfo);
                }
            }
        }

        private static void savaInExcel(List<ImageInfo> saveList, string path, string filename)
        {
            if (saveList == null)
            {
                return;
            }

            if (!Directory.Exists(path))
            {
                Debug.LogWarning("文件夹不存在，已自动创建");
                Directory.CreateDirectory(path);
            }

            path = path + "\\" + filename;
            StreamWriter sw = new StreamWriter(path, true, Encoding.UTF8);
            var stringBuilderTitle = new StringBuilder("场景名称").Append(",").Append("图片").Append(",").Append("长")
                .Append(",").Append("宽");
            sw.WriteLine(stringBuilderTitle.ToString());
            foreach (var line in saveList)
            {
                StringBuilder stringBuilder = new StringBuilder();
                stringBuilder.Append(line.sceneName).Append(",").Append(line.imageName).Append(",").Append(line.height)
                    .Append(",").Append(line.width);
                sw.WriteLine(stringBuilder.ToString());
            }
            sw.Flush();
            sw.Close();
        }
    }
}

