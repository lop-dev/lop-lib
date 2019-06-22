using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;
using UnityEngine.SceneManagement;

public class CSceneShadowManager
{
    private static bool open;

    [MenuItem("美术/场景工具/烘焙当前场景时的处理/烘焙前打开所有阴影",false,030101)]
    public static void OpenSceneShadow()
    {
        CheckSceneShadow(true);
        COneKeyApply.OnekeyApply();
    }

    [MenuItem("美术/场景工具/烘焙当前场景时的处理/烘焙后关闭所有阴影",false,030101)]
    public static void CloseSceneShadow()
    {
        CheckSceneShadow(false);
        COneKeyApply.OnekeyApply();
    }

    private static void CheckSceneShadow(bool b)
    {
        open = b;
        Scene scene = SceneManager.GetActiveScene();
        foreach (GameObject root in scene.GetRootGameObjects())
        {
            DealSingleGO(root);
        }
    }


    private static void DealSingleGO(GameObject root)
    {
        MeshRenderer mr = root.GetComponent<MeshRenderer>();
        
        if (mr != null)
        {
            mr.receiveShadows = open;
            mr.shadowCastingMode = open? UnityEngine.Rendering.ShadowCastingMode.On:UnityEngine.Rendering.ShadowCastingMode.Off;
        }
        for (int i = 0; i < root.transform.childCount; i++)
        {
            DealSingleGO(root.transform.GetChild(i).gameObject);
        }
    }
}
