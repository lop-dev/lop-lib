using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;

class CCheckTextureSize : EditorWindow
{
    private string texturePath;
    private static readonly string TEXTURE_PATH = "Assets/App/Shared/ACGN/SceneTools/Editor/filePath.png";
    private static Texture iconTexture;
    [MenuItem("程序/检查工具/检查图片长宽是否为2的次幂", false, 02010061)]
    private static void Init()
    {
        iconTexture = AssetDatabase.LoadAssetAtPath<Texture>(TEXTURE_PATH);
        CCheckTextureSize window = GetWindowWithRect<CCheckTextureSize>(new Rect(340, 420, 800, 750), false, "检查图片长宽是否为2的次幂");
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
                texturePath = EditorUtility.OpenFolderPanel("选择图片目录", "Assets/App/Pro/GameRes" , "");
                Repaint();
            }

            if (GUILayout.Button("开始检查", GUILayout.Width(60), GUILayout.Height(20)))
            {
                int i = 32;
                int j = i & (i - 1);
                //Debug.Log("i = " + i + "  j = " + j);
                //Debug.Log("test + " + texturePath);
                CheckTextureSize(texturePath);
            }
        }
        EditorGUILayout.EndHorizontal();

        EditorGUILayout.Space();

    }

    private static List<myTexture> list = new List<myTexture>();
    private static void CheckTextureSize(string path)
    {
        string[] files = Directory.GetFiles(path, "*.*", SearchOption.AllDirectories);
        int fileCount = files.Length;
        int currentIndex = 0;
        string file;
        foreach (string f in files)
        {
            if(EditorUtility.DisplayCancelableProgressBar("处理中", f, currentIndex / (float) fileCount))
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
            if (!IsAPowerOf2(texture.width) && !IsAPowerOf2(texture.height))
            {
                list.Add(new myTexture(f, texture.width, texture.height));
            }
        }
        Save(list, "长宽不是2的幂的图片表.txt");
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

    private static bool IsAPowerOf2(int length)
    {
        length = length & (length - 1);
        if (length == 0)
        {
            return true;
        }
        return false;
    }

    private class myTexture
    {
        public string file;
        public int width;
        public int height;

        public myTexture(string file, int width, int height)
        {
            this.file = file;
            this.width = width;
            this.height = height;
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
        foreach (var line in saveList)
        {
            streamWriter.WriteLine("path = " + line.file + "    weigth = " + line.width + "    height = " + line.height);
        }
        streamWriter.Flush();
        streamWriter.Close();
        fileStream.Close();
    }
}

