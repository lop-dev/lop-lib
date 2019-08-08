using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;
/// <summary>
/// 导入动画fbx时，自动分离出优化过的anim文件
/// </summary>
/// 

namespace UDLib.Editor
{
    public class AnimationFBXPost : AssetPostprocessor
    {
        static private string ANIM_PATH = "Assets/App/Art/Character/Hero/Animation";
        static void OnPostprocessAllAssets(string[] importedAssets, string[] deletedAssets, string[] movedAssets, string[] movedFromAssetPaths)
        {
            foreach (string assetPath in importedAssets)
            {
                Process(assetPath);
            }

            foreach (string assetPath in movedAssets)
            {
                Process(assetPath);
            }
        }

        private static void Process(string assetPath)
        {
            ModelImporter modelImporter = AssetImporter.GetAtPath(assetPath) as ModelImporter;
            if (modelImporter == null) return;
            if (!assetPath.Contains("AnimationFbx")) return;//只处理这个文件夹下的动作fbx

            var objs = AssetDatabase.LoadAllAssetsAtPath(assetPath);
            var folderPath = string.Empty;
            foreach (var o in objs)
            {
                if (o is AnimationClip)
                {
                    if (o.name.StartsWith("__preview__"))
                        continue;
                    var anim = COptimizeAnim.OptimizeAnimationCurveData(o as AnimationClip, false);

                    var index = assetPath.LastIndexOf("/") + 1;
                    var fbxName = assetPath.Substring(index).Split('.')[0];
                    index = fbxName.LastIndexOf('_');
                    var folderName = fbxName.Substring(0, index);
                    if (folderName.EndsWith("_H"))
                        folderName = folderName.Substring(0, folderName.Length - 2);
                    var destFolderPath = ANIM_PATH + "/" + folderName;
                    var destAnimPath = destFolderPath + "/" + fbxName + ".anim";

                    DirectoryInfo info = new DirectoryInfo(destFolderPath);
                    if (!info.Exists)
                        info.Create();

                    if (File.Exists(destAnimPath))
                    {
                        AnimationClip oldAnim = AssetDatabase.LoadAssetAtPath<AnimationClip>(destAnimPath);
                        var oldImporter = AssetImporter.GetAtPath(destAnimPath) as ModelImporter;
                        EditorUtility.CopySerialized(anim, oldAnim);
                        AssetDatabase.SaveAssets();
                        Debug.Log(1);
                    }
                    else
                    {
                        AssetDatabase.CreateAsset(anim, destAnimPath);
                        AssetDatabase.SaveAssets();
                    }
                    AssetDatabase.Refresh();
                }
            }
        }
    }
}
