using System;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
using System.IO;
using UDLib.Editor;

public class FindeReferencedCount : EditorWindow
{
    private enum EFileType
    {
        fbx,
        png,
        shader,
        mat,
        prefab,
        ttf
    }

    private static readonly string TEXTURE_PATH = "Assets/App/Shared/ACGN/SceneTools/Editor/filePath.png";
    private static readonly string WINDOW_NAME = "查找文件引用次数";

    private GUIStyle m_titleStyle = new GUIStyle();
    private GUIStyle m_iconStyle = new GUIStyle();

    private Texture m_fileTexture = null;
    private string m_dataFilePath = "";
    private string m_assetsPath = "Assets";
    private EFileType m_fileType = EFileType.fbx;

    [MenuItem("程序/检查工具/查找文件引用次数",false,02010101)]
    static void open()
    {
        FindeReferencedCount editor = GetWindowWithRect<FindeReferencedCount>(new Rect(640, 420, 500, 100), true, WINDOW_NAME);
        editor.Init();
        editor.Show();
    }

    private void Init()
    {
        m_titleStyle.fontStyle = FontStyle.Italic;
        m_titleStyle.normal.textColor = new Color(1, 0, 0);
        m_titleStyle.fontSize = 13;

        if (null == m_fileTexture)
        {
            m_iconStyle.fixedWidth = 20;
            m_iconStyle.fixedHeight = 20;
            m_fileTexture = AssetDatabase.LoadAssetAtPath<Texture>(TEXTURE_PATH);
        }
    }

    private void OnGUI()
    {
        EditorGUILayout.LabelField("执行时间较长，请谨慎使用！", m_titleStyle);
        EditorGUILayout.Space();
        EditorGUILayout.BeginHorizontal();
        {
            if (GUILayout.Button(m_fileTexture, m_iconStyle))
            {
                m_dataFilePath = EditorUtility.OpenFolderPanel("Open Folder Dialog", m_assetsPath, "");
                if (string.IsNullOrEmpty(m_dataFilePath))
                {
                    Debug.Log("取消选择文件");
                    return;
                }
                m_assetsPath = m_dataFilePath;
            }
            EditorGUILayout.LabelField("文件夹", GUILayout.Width(40));
            m_dataFilePath = EditorGUILayout.TextField(m_dataFilePath, GUILayout.Width(300));
            EditorGUILayout.LabelField("类型", GUILayout.Width(30));
            m_fileType = (EFileType)EditorGUILayout.EnumPopup(m_fileType);
        }
        EditorGUILayout.EndHorizontal();

        EditorGUILayout.Space();

        EditorGUILayout.BeginHorizontal();
        {
            EditorGUILayout.LabelField("", GUILayout.Width(20));
            if (GUILayout.Button("开始查找"))
            {
                Process();
            }
            EditorGUILayout.LabelField("", GUILayout.Width(20));
        }
        EditorGUILayout.EndHorizontal();

    }

    private int index;
    private void Process()
    {
        if (string.IsNullOrEmpty(m_dataFilePath))
        {
            Debug.LogWarning("没有选择任何文件夹");
            return;
        }
        index = 0;
        string[] file = Directory.GetFiles(m_dataFilePath, "*." + m_fileType.ToString(), SearchOption.AllDirectories);
        if (file.Length == 0)
        {
            Debug.Log("没有该类型文件~");
            return;
        }

        UDLib.Editor.CCheckTools.ClearLogList();
        EditorApplication.update = delegate ()
        {
            bool cancle = EditorUtility.DisplayCancelableProgressBar("匹配资源中", file[index], (float)index / (float)file.Length);
            string unityPath = file[index].Substring(Application.dataPath.Length - 6);
            List<string> list = UDLib.Editor.CFindReference.FindReferenceForFile(unityPath.Replace(@"\", "/"));
            UDLib.Editor.CCheckTools.AddErrorInfo(unityPath + ","+list.Count);
            index++;
            if (cancle || index >= file.Length)
            {
                EditorApplication.update = null;
                UDLib.Editor.CCheckTools.WriteAllInfoToFile(m_fileType.ToString() + "类型文件引用次数", "", UDLib.Editor.CCheckTools.EFileType.csv);
                EditorUtility.ClearProgressBar();
                Debug.Log("匹配结束");
                UDLib.Editor.CCheckTools.LogAll();
            }
        };
        EditorUtility.ClearProgressBar();
    }
}
