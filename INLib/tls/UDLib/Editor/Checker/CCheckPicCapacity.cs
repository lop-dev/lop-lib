using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using UnityEditor;
using UnityEngine;
using UnityEngine.Profiling;

public class CCheckPicCapacity : Editor {

    private const int MaxCapacity = 10 * 1024 * 1024;

    [MenuItem("程序/检查工具/检查所有图片的容量(>10M)",false,02010061)]
	public static void CheckPicCapacity()
    {
        string[] files = Directory.GetFiles(UDLib.Editor.CPath.AssetsApp, "*.*", SearchOption.AllDirectories);
        int fileCount = files.Length;
        int currentIndex = 0;
        foreach (string f in files)
        {
            UDLib.Editor.CCheckTools.DisplayProgressBar("处理中", f, currentIndex / (float)fileCount);
            currentIndex++;
            if (IsPicture(f) &&IsOverCapacity(f))
            {
                Debug.LogError(f);
            }
        }
        UDLib.Editor.CCheckTools.ClearProgressBar();
    }
    private static bool IsPicture(string path)
    {
        if (!File.Exists(path) || !path.EndsWith(".png") && !path.EndsWith(".psd") && !path.EndsWith(".tga"))
        {
            return false; 
        }
        return true;
    }
    private static bool IsOverCapacity(string path)
    {
        FileInfo file = new FileInfo(path);
        if(file.Length >= MaxCapacity)
        {
            Debug.LogError(EditorUtility.FormatBytes(file.Length));
            return true;
        }
        return false;
    }
    #region 没用的函数
    /// <summary>
    /// unity profiller中的检测的占用硬盘大小
    /// </summary>
    /// <param name="texture"></param>
    /// <returns></returns>
    private static bool CheckPicCapacityIsOverMax(Texture texture)
    {
        //Texture target = Selection.activeObject as Texture;

        var type = System.Reflection.Assembly.Load("UnityEditor.dll").GetType("UnityEditor.TextureUtil");
        MethodInfo methodInfo = type.GetMethod("GetStorageMemorySize", BindingFlags.Static | BindingFlags.Instance | BindingFlags.Public);

        if((int)methodInfo.Invoke(null, new object[] { texture }) >= MaxCapacity){
            Debug.Log("chaochuneicun");
            
            Debug.LogError(EditorUtility.FormatBytes((int)methodInfo.Invoke(null, new object[] { texture })));
            return true;
        }
        //Debug.Log("内存占用：" + EditorUtility.FormatBytes(Profiler.GetRuntimeMemorySizeLong(Selection.activeObject)));
        Debug.Log("硬盘占用：" + EditorUtility.FormatBytes((int)methodInfo.Invoke(null, new object[] { texture })));
        return false;
    }
    #endregion
}
