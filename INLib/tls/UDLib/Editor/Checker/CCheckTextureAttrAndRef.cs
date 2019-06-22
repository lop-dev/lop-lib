using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using UnityEditor;
using UnityEngine;

class CCheckTextureAttrAndRef : EditorWindow
{
    private static string texturePath;
    private static readonly string TEXTURE_PATH = "Assets/App/Shared/ACGN/SceneTools/Editor/filePath.png";
    private static Texture iconTexture;

    [MenuItem("程序/检查工具/检查图片被引用次数(检查时间长，谨慎使用)", false, 02010061)]
    private static void init()
    {
        iconTexture = AssetDatabase.LoadAssetAtPath<Texture>(TEXTURE_PATH);
        CCheckTextureAttrAndRef window = GetWindowWithRect<CCheckTextureAttrAndRef>(new Rect(340, 420, 800, 750), false, "检查图片工具");
        window.Show();
    }

    private GUIStyle fontStyle = new GUIStyle();
    private void OnGUI()
    {
        EditorGUILayout.Space();
        fontStyle.fontSize = 16;
        fontStyle.fontStyle = FontStyle.Normal;
        EditorGUILayout.BeginHorizontal();
        {
            EditorGUILayout.LabelField("图片路径", fontStyle, GUILayout.Width(100));

            texturePath = EditorGUILayout.TextField(texturePath, GUILayout.Width(500), GUILayout.Height(20));

            if (GUILayout.Button(iconTexture, GUILayout.Width(60), GUILayout.Height(20)))
            {
                texturePath = EditorUtility.OpenFolderPanel("选择图片目录", "Assets/App/Pro/GameRes", "");
                Repaint();
            }

            if (GUILayout.Button("开始检查", GUILayout.Width(60), GUILayout.Height(20)))
            {
                //int i = 32;
                //int j = i & (i - 1);
                //Debug.Log("i = " + i + "  j = " + j);
                //Debug.Log("test + " + texturePath);
                CheckTextureAttrAndRef(texturePath);
            }
        }
        EditorGUILayout.EndHorizontal();

        EditorGUILayout.Space();

    }

    private static List<myTexture> list = new List<myTexture>();
    private static void CheckTextureAttrAndRef(string path)
    {
        string[] files = Directory.GetFiles(path, "*.*", SearchOption.AllDirectories);
        int fileCount = files.Length;
        int currentIndex = 0;
        string file;
        foreach (string f in files)
        {
            if (EditorUtility.DisplayCancelableProgressBar("处理中", currentIndex + "/" + fileCount, currentIndex / (float)fileCount))
            {
                EditorUtility.ClearProgressBar();
                return;
            }
            currentIndex++;
            file = f.Replace('\\', '/');
            //Debug.Log("file1 = " + file + "\n" + "rootpath = " + Application.dataPath);
            file = file.Replace(Application.dataPath, "");
            //Debug.Log("file2 = " + file);
            file = "Assets" + file;
            //Debug.Log("file3 = " + file);
            if (!IsPicture(file))
            {
                continue;
            }
            Texture2D texture = AssetDatabase.LoadAssetAtPath(file, typeof(Texture2D)) as Texture2D;
            if (texture == null)
            {
                Debug.LogWarning("texture is not pic format");
                continue;
            }
            int refCount = GetRefCountByGUID(file);
            //int refCount = GetRefCount(file);
            list.Add(new myTexture(file, texture.width, texture.height, refCount));
        }
        savaInExcel(list, String.Format("检测图片被引用数量表_{0}.csv", DateTime.Now.ToString("yyyy-MM-dd-hh-mm-ss")));
        list.Clear();
        EditorUtility.ClearProgressBar();
    }
    private static bool IsPicture(string path)
    {
        if (!File.Exists(path) || !path.EndsWith(".png") && !path.EndsWith(".psd") && !path.EndsWith(".tga") && !path.EndsWith(".exr"))
        {
            return false;
        }
        return true;
    }

    private static int GetRefCount(string path)
    {
        if (string.IsNullOrEmpty(path))
            return 0;
        string[] files = Directory.GetFiles(Application.dataPath, "*.prefab", SearchOption.AllDirectories);
        string[] scene = Directory.GetFiles(Application.dataPath, "*.unity", SearchOption.AllDirectories);

        int refCount = 0;
        for (int i = 0; i < files.Length; i++)
        {
            if (EditorUtility.DisplayCancelableProgressBar("查找中", i + "/" + files.Length, (float) i / files.Length))
            {
                return 0;
            }
            string[] source = AssetDatabase.GetDependencies(files[i].Replace(Application.dataPath, "Assets"));
            for (int j = 0; j < source.Length; j++)
            {
                if (source[j] == path)
                    refCount++;
            }
        }
        EditorUtility.ClearProgressBar();
        for (int i = 0; i < scene.Length; i++)
        {
            if (EditorUtility.DisplayCancelableProgressBar("查找中", i + "/" + scene.Length, (float) i / scene.Length))
            {
                return 0;
            }
            string[] source = AssetDatabase.GetDependencies(scene[i].Replace(Application.dataPath, "Assets"));
            for (int j = 0; j < source.Length; j++)
            {
                if (source[j] == path)
                    refCount++;
            }
            EditorUtility.ClearProgressBar();
        }

        return refCount;
    }

    private class myTexture
    {
        public string file;
        public int width;
        public int height;
        public int refCount;

        public myTexture(string file, int width, int height, int refCount)
        {
            this.file = file;
            this.width = width;
            this.height = height;
            this.refCount = refCount;
        }
    }

    private static void Save(List<myTexture> saveList, string filename)
    {
        string path = "Assets\\ToolsOutput";
        if (!Directory.Exists(path))
        {
            Debug.LogWarning("文件夹不存在，已自动创建");
            Directory.CreateDirectory(path);
        }
        path = path + "\\" + filename;
        FileStream fileStream = new FileStream(path, FileMode.Create);
        StreamWriter streamWriter = new StreamWriter(fileStream);
        int i = 0;
        foreach (var line in saveList)
        {
            EditorUtility.DisplayProgressBar("保存文件中", i + "/" + saveList.Count, (float)i/saveList.Count);
            streamWriter.WriteLine("path = " + line.file + "    weigth = " + line.width + "    height = " + line.height + 
                                   "    count = " + line.refCount);
        }
        streamWriter.Flush();
        streamWriter.Close();
        fileStream.Close();
        EditorUtility.ClearProgressBar();
    }

    private static void savaInExcel(List<myTexture> saveList, string filename)
    {
        if (saveList == null)
        {
            return;
        }
        string path = "Assets\\ToolsOutput";
        if (!Directory.Exists(path))
        {
            Debug.LogWarning("文件夹不存在，已自动创建");
            Directory.CreateDirectory(path);
        }
        path = path + "\\" + filename;
        StreamWriter sw = new StreamWriter(path, true, Encoding.UTF8);
        var stringBuilderTitle = new StringBuilder("资源路径").Append(",").Append("宽").Append(",").Append("长")
            .Append(",").Append("被引用次数");
        sw.WriteLine(stringBuilderTitle.ToString());
        foreach (var line in saveList)
        {
            StringBuilder stringBuilder = new StringBuilder();
            stringBuilder.Append(line.file).Append(",").Append(line.width).Append(",").Append(line.height)
                .Append(",").Append(line.refCount);
            sw.WriteLine(stringBuilder.ToString());
        }
        sw.Flush();
        sw.Close();
    }

    private static int GetRefCountByGUID(string path)
    {
        EditorSettings.serializationMode = SerializationMode.ForceText;
        if (string.IsNullOrEmpty(path))
        {
            return 0;
        }
        int refCount = 0;
        string guid = AssetDatabase.AssetPathToGUID(path);
        List<string> withoutExtensions = new List<string>() { ".prefab", ".unity"};
        string[] files = Directory.GetFiles(Application.dataPath, "*.*", SearchOption.AllDirectories)
            .Where(s => withoutExtensions.Contains(Path.GetExtension(s).ToLower())).ToArray();

        for (int i = 0; i < files.Length; i++)
        {
            string file = files[i];

            if (EditorUtility.DisplayCancelableProgressBar("匹配资源中", i + "/" + files.Length,
                (float) i / (float) files.Length))
            {
                return 0;
            }
            if (Regex.IsMatch(File.ReadAllText(file), guid))
            {
                refCount++;
            }
        }
        EditorUtility.ClearProgressBar();
        return refCount;
    }
    //private void SaveLoadTimeToCsv()
    //{
    //    if (mDebugLoadTimeList != null)
    //    {
    //        string path = Path.Combine(Application.persistentDataPath, string.Format("加载时间测试_{0}.csv", DateTime.Now.ToString("yyyy-MM-dd-hh-mm-ss")));
    //        StreamWriter sw = new StreamWriter(path, true, Encoding.UTF8);
    //        var sbTitle = new StringBuilder("资源名称").Append(",").Append("加载编号").Append(",").Append("加载AB时间（秒）").Append(",").Append("从AB加载资源时间（秒）").Append(",").Append("资源大小（Kb）").Append(",").Append("加载状态");
    //        sw.WriteLine(sbTitle.ToString());
    //        foreach (KeyValuePair<uint, CABLoadTime> kv in mDebugLoadTimeList)
    //        {
    //            StringBuilder sb = new StringBuilder();
    //            if (kv.Value.isFinishLoad == true)
    //            {
    //                sb.Append(kv.Value.name).Append(",").Append(kv.Key).Append(",").Append(kv.Value.loadABTime).Append(",").Append(kv.Value.loadAssetTime).Append(",").Append(kv.Value.size).Append(",").Append("加载成功");
    //            }
    //            else
    //            {
    //                sb.Append(kv.Value.name).Append(",").Append(kv.Key).Append(",").Append(0).Append(",").Append(0).Append(",").Append(0).Append(",").Append("加载出错");
    //            }

    //            sw.WriteLine(sb.ToString());
    //        }
    //        sw.Close();
    //        mDebugLoadTimeList.Clear();
    //    }
    //}



}
