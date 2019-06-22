using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEditor;
using UnityEngine;

public class TextureChecker : EditorWindow
{

    private static readonly string TEXTURE_PATH = "Assets/App/Shared/ACGN/SceneTools/Editor/filePath.png";

    private static Texture iconTexture;

    private static string prefabPath;
    private static string UIModulePath;

    [MenuItem("程序/UI工具/UI资源检查",false, 02050001)]
    static void Init()
    {
        prefabPath = Application.dataPath + "/App/Pro/GameRes/UIPrefabs";
        UIModulePath = Application.dataPath + "/App/Pro/GameRes/UI/Module";

        iconTexture = AssetDatabase.LoadAssetAtPath<Texture>(TEXTURE_PATH);

        TextureChecker window = GetWindowWithRect<TextureChecker>(new Rect(340, 420, 800, 750), true, "UI资源检查");

        window.Show();
    }

    void OnGUI()
    {
        EditorGUILayout.Space();

        EditorGUILayout.BeginHorizontal();
        {
            EditorGUILayout.LabelField("Prefab路径", GUILayout.Width(100));

            prefabPath = EditorGUILayout.TextField(prefabPath, GUILayout.Width(500));

            if (GUILayout.Button(iconTexture, GUILayout.Width(60), GUILayout.Height(20)))
            {
                prefabPath = EditorUtility.OpenFolderPanel("选择UIPrefab目录", "Assets/App/Pro/GameRes", "UIPrefabs");
                Repaint();
            }
        }
        EditorGUILayout.EndHorizontal();

        EditorGUILayout.Space();

        EditorGUILayout.BeginHorizontal();
        {
            EditorGUILayout.LabelField("UI Module路径", GUILayout.Width(100));

            UIModulePath = EditorGUILayout.TextField(UIModulePath, GUILayout.Width(500));

            if (GUILayout.Button(iconTexture, GUILayout.Width(60), GUILayout.Height(20)))
            {
                UIModulePath = EditorUtility.OpenFolderPanel("选择UIImage目录", "Assets/App/Pro/GameRes/UI", "Module");
                Repaint();
            }
        }
        EditorGUILayout.EndHorizontal();



        EditorGUILayout.Space();

        if (GUILayout.Button("查看common纹理使用", GUILayout.Width(200), GUILayout.Height(50)))
        {
            if (string.IsNullOrEmpty(UIModulePath) || string.IsNullOrEmpty(prefabPath))
            {
                EditorUtility.DisplayDialog("Error", "路径为空", "确定");
                return;
            }
                
            UITextureChecker.CheckCommonFiles();
        }

        EditorGUILayout.Space();

        if (GUILayout.Button("查找未使用纹理", GUILayout.Width(200), GUILayout.Height(50)))
        {
            if (string.IsNullOrEmpty(UIModulePath) || string.IsNullOrEmpty(prefabPath))
            {
                EditorUtility.DisplayDialog("Error", "路径为空", "确定");
                return;
            }
            UITextureChecker.CheckUnusedTexture(UIModulePath, prefabPath);
        }

        EditorGUILayout.Space();

        if (GUILayout.Button("查找重复使用纹理", GUILayout.Width(200), GUILayout.Height(50)))
        {
            if (string.IsNullOrEmpty(UIModulePath) || string.IsNullOrEmpty(prefabPath))
            {
                EditorUtility.DisplayDialog("Error", "路径为空", "确定");
                return;
            }
            UITextureChecker.CheckRepeatUsedTexture(UIModulePath, prefabPath);
        }
    }
}
