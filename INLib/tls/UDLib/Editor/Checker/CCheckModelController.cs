using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;

public class CCheckModelController
{
    private static string root = "Assets/App/Pro/GameRes/Character/Hero/Model/";

    [MenuItem("美术/检查工具/检查角色模型动画控制器", false, 0)]
    public static void CheckCharacterController()
    {
        string []files = Directory.GetFiles(root,"*.prefab",SearchOption.AllDirectories);
        foreach(var v in files)
        {
            GameObject asset = AssetDatabase.LoadAssetAtPath<GameObject>(v);
            if(!asset)
            {
                Debug.LogError(string.Format("加载prefab失败：{0}",v));
                continue;
            }
            Animator []anis = asset.GetComponentsInChildren<Animator>();
            if(anis == null)
            {
                continue;
            }
            foreach(var ani in anis)
            {
                if(ani.runtimeAnimatorController != null)
                {
                    Debug.LogError(v);
                }
            }
        }
    }
}
