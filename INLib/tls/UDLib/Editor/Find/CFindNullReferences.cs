using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class CFindNullReferences : EditorWindow
{
    static private string[] result = null;

    [MenuItem("程序/检查工具/查找空挂载脚本",false,02010000)]
    static public void Find()
    {
        result = FindNullReferencePrefabs();
        init();
    }
    static private void  init()
    {
        var window = GetWindow(typeof(CFindNullReferences));
        window.Show();
    }
    private Vector2 scrollPos;
    private void OnGUI()
    {
        if (result ==  null)
        {
            return;
        }

        scrollPos = EditorGUILayout.BeginScrollView(scrollPos,false,true);
        foreach(var s in result)
        {
            EditorGUILayout.SelectableLabel(s);
            
        }
        EditorGUILayout.EndScrollView();
        
    }

    static private string[] FindNullReferencePrefabs()
    {
        string[] prefabs = GetAllPrefab();
        List<string> result = new List<string>();
        int index = 0;
        foreach (var prefab in prefabs)
        {
            UnityEngine.Object o = AssetDatabase.LoadMainAssetAtPath(prefab);
            GameObject gameObject;
            try
            {
                gameObject = (GameObject)o;
                Component[] components = gameObject.GetComponentsInChildren<Component>(true);
                EditorUtility.DisplayProgressBar("检索中", prefabs[index++], (float)index / (float)prefabs.Length);
                foreach (var component in components)
                {
                    if (component == null)
                    {
                        result.Add(prefab);
                    }
                }
            }
            catch
            {
                Debug.Log("预制体 “" + prefab + "“ 没有GameObject");
            }
        }
        EditorUtility.ClearProgressBar();
        return result.ToArray();
    }
    private static string[] GetAllPrefab()
    {
        string[] assets = AssetDatabase.GetAllAssetPaths();
        List<string> prefabs = new List<string>();
        int index = 0;
        foreach(var prefab in assets)
        {
            EditorUtility.DisplayProgressBar("检索中", assets[index++], (float)index / (float)assets.Length);
            if (prefab.Contains(".prefab"))
                prefabs.Add(prefab);
        }
        EditorUtility.ClearProgressBar();
        return prefabs.ToArray();
    }
}
