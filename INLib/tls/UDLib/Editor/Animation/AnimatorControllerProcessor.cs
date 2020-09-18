/******************************************************************************
 * DESCRIPTION: AnimatorController处理器
 * fanghy 
 * 
*******************************************************************************/

using System.IO;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEditor.Animations;

public class AnimatorControllerProcessor
{
    public static string animatorPath = Application.dataPath + "/App/Pro/GameRes/Character/Hero/Animator";
    public static string tempAnimatorFolder = Application.dataPath + "/../TempAnimator/";

    [MenuItem("程序/动画工具/动画controller依赖清理", false, 02060000)]
    private static void processAllAnimators()
    {
        // create temp folder
        if (!Directory.Exists(tempAnimatorFolder))
        {
            Directory.CreateDirectory(tempAnimatorFolder);
        }

        // check controller dependency
        string[] files = Directory.GetFiles(animatorPath, "*.controller", SearchOption.AllDirectories);
        int count = files.Length;
        int currentIndex = 0;
        foreach (string file in files)
        {
            currentIndex++;
            EditorUtility.DisplayProgressBar("animator处理", string.Format("处理文件中{0}/{1}", currentIndex, count), (float)currentIndex / count);
            CorrectData(file);
        }

        // delete temp folder
        Directory.Delete(tempAnimatorFolder, true);

        EditorUtility.ClearProgressBar();
    }

    private static void CorrectData(string file)
    {
        string block = null;
        bool hasDenpendency = false;
        bool isDependOtherAnimatorController = false;
        string filePathName = file;
        string tempFilePathName = tempAnimatorFolder + "/" + System.DateTime.Now.Ticks.ToString() + ".controller";
        using (StreamWriter writer = File.CreateText(tempFilePathName))
        {
            using (StreamReader reader = File.OpenText(filePathName))
            {
                string content;
                while (null != (content = reader.ReadLine()))
                {
                    if (content.StartsWith("--- !u"))
                    {
                        if (!string.IsNullOrEmpty(block))
                            writer.Write(block);

                        block = content + System.Environment.NewLine;
                        isDependOtherAnimatorController = false;
                    }
                    else
                    {
                        if (isDependOtherAnimatorController)
                            continue;

                        if (string.IsNullOrEmpty(block))
                            writer.WriteLine(content);
                        else
                        {
                            block += (content + System.Environment.NewLine);

                            // 检测是否依赖其他的Animator Controller
                            if (content.Contains("m_DstState:") && content.Contains("guid"))
                            {
                                block = null;
                                isDependOtherAnimatorController = true;
                                hasDenpendency = true;
                                Debug.Log(string.Format("++++处理动画状态机{0}的游离依赖++++", file));
                            }
                        }
                    }
                }

                // 写入最后的数据
                if (!string.IsNullOrEmpty(block))
                    writer.Write(block);
            }
        }

        if (hasDenpendency)
        {
            FileUtil.ReplaceFile(tempFilePathName, filePathName);
        }
        AssetDatabase.Refresh();
    }

    //[MenuItem("Assets/Artist Tools/Animator Controller/Collect Animator Controller Dependencies")]
    private static void CollectAnimatorControllerDependencies()
    {
        AnimatorController animatorController = Selection.activeObject as AnimatorController;
        string[] dependencyArray = AssetDatabase.GetDependencies(AssetDatabase.GetAssetPath(animatorController));

        Debug.Log("************************* Animator Controller Dependencies (" + animatorController.name + ") *************************");
        foreach (string dependency in dependencyArray)
        {
            if (dependency.EndsWith(".controller"))
                Debug.Log(dependency);
        }
        Debug.Log("************************************************* End *************************************************");
    }

    //[MenuItem("ArtistTools/Check Animator Controller Dependencies")]
    private static void CheckAnimatorControllerDependencies()
    {
        List<string> dependencyCheckNameList = new List<string>();
        string[] filePathNameArray = Directory.GetFiles(Application.dataPath + "/BundleResources/Animator", "*.controller", SearchOption.TopDirectoryOnly);
        foreach (string filePathName in filePathNameArray)
        {
            string[] dependencyArray = AssetDatabase.GetDependencies(filePathName.Substring(filePathName.IndexOf("/Assets/") + 1));
            foreach (string dependency in dependencyArray)
            {
                if (dependency.EndsWith(".controller"))
                {
                    string assetName = Path.GetFileNameWithoutExtension(filePathName);
                    string dependencyName = Path.GetFileNameWithoutExtension(dependency);

                    // A依赖于B，如果"B_A"存在，表示B也依赖于A，则是循环依赖
                    string checkName = dependencyName + "_" + assetName;
                    if (dependencyCheckNameList.Contains(checkName))
                        Debug.Log(Path.GetFileName(filePathName) + " and " + Path.GetFileName(dependency) + " depend each other");

                    dependencyCheckNameList.Add(assetName + "_" + dependencyName);
                }
            }
        }
    }
}