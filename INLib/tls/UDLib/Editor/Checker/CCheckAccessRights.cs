using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;
namespace UDLib.Editor
{
    public class CCheckAccessRights
    {
        static string[] textureFilter = { "png", "tga", "psd", "exr" };
        static string[] modelFilter = { "FBX" };

        [MenuItem("美术/检查工具/检查所有美术资源的读写选项", false, 03030000)]
        public static void CheckReadWrite()
        {
            CheckAccessRights(false);
        }

        [MenuItem("美术/修改工具/修改所有美术资源的读写选项", false, 03040000)]
        public static void ModifyReadWrite()
        {
            CheckAccessRights(true);
        }

        public static void CheckAccessRights(bool bmodify)
        {
            Debug.Log("找出Read/Write属性为true的png,tga,psd,exr,fbx类型文件");
            CheckTextureFiles(bmodify);
            CheckModelFiles(bmodify);
        }

        public static void CheckTextureFiles(bool bmodify)
        {
            foreach (string type in textureFilter)
            {
                string[] files = Directory.GetFiles(CPath.Assets, "*." + type, SearchOption.AllDirectories);
                int index = 0;
                int totalCount = files.Length;
                foreach (string f in files)
                {
                    CCheckTools.DisplayProgressBar("检查所有" + type + "文件读写权限", f, index / (float)totalCount);
                    string path = f.Replace('\\', '/');
                    TextureImporter imp = AssetImporter.GetAtPath(path) as TextureImporter;
                    if (imp != null && imp.isReadable == true)
                    {
                        Debug.LogError(path);
                        if (bmodify)
                        {
                            imp.isReadable = false;
                            AssetDatabase.ImportAsset(path);
                        }
                    }
                    index++;
                }
                CCheckTools.ClearProgressBar();
            }
        }

        public static void CheckModelFiles(bool bmodify)
        {
            foreach (string type in modelFilter)
            {
                string[] files = Directory.GetFiles(CPath.Assets, "*." + type, SearchOption.AllDirectories);
                int index = 0;
                int totalCount = files.Length;
                foreach (string f in files)
                {
                    CCheckTools.DisplayProgressBar("检查所有" + type + "文件读写权限", f, index / (float)totalCount);
                    string path = f.Replace('\\', '/');
                    ModelImporter imp = AssetImporter.GetAtPath(path) as ModelImporter;
                    if (imp != null && imp.isReadable == true)
                    {
                        Debug.LogError(path);
                        if (bmodify)
                        {
                            imp.isReadable = false;
                            AssetDatabase.ImportAsset(path);
                        }
                    }
                    index++;
                }
                CCheckTools.ClearProgressBar();
            }
        }

        public static void SetModelIsReadable(string path, bool readWrite = false)
        {
            ModelImporter importer = AssetImporter.GetAtPath(path) as ModelImporter;
            if (importer != null)
            {
                //Debug.Log("enter SetModelIsReadable Func");
                importer.isReadable = readWrite;
            }
        }
        public static void SetTextureIsReadable(string path, bool readWrite = false)
        {
            TextureImporter textureImporter = AssetImporter.GetAtPath(path) as TextureImporter;
            if (textureImporter != null)
            {
                //Debug.Log("enter SetTextureIsReadable Func");
                textureImporter.isReadable = readWrite;
            }
        }
    }
}

