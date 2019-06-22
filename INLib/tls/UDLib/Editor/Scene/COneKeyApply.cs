using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;
using UnityEngine.SceneManagement;

public class COneKeyApply
{
   // [MenuItem("美术/场景工具/保存当前场景时的操作/1、对当前场景中所有的预制体进行Apply操作")]
    public static void OnekeyApply()
    {
        //美术误点后transform信息会应用到所有实例上，他们要求去掉！！
        //Scene scene = SceneManager.GetActiveScene();
        //foreach (GameObject go in scene.GetRootGameObjects())
        //{
        //    ApplyByRoot(go);
        //}
        //EditorSceneManager.SaveScene(EditorSceneManager.GetActiveScene());
    }

    private static void ApplyByRoot(GameObject root)
    {
        PrefabType type = PrefabUtility.GetPrefabType(root);
        if (type == PrefabType.PrefabInstance || type == PrefabType.DisconnectedPrefabInstance)
        {
            string path = AssetDatabase.GetAssetPath(PrefabUtility.GetCorrespondingObjectFromSource(root));
            //CreateNew(root,path);
            PrefabUtility.ReplacePrefab(root, PrefabUtility.GetCorrespondingObjectFromSource(root), ReplacePrefabOptions.ConnectToPrefab);
        }
        else
        {
            for(int i =0;i < root.transform.childCount;i++)
            {
                ApplyByRoot(root.transform.GetChild(i).gameObject);
            }
        }
    }

    private static void CreateNew(GameObject obj, string localPath)
    {
        Object prefab = PrefabUtility.CreateEmptyPrefab(localPath);
        PrefabUtility.ReplacePrefab(obj, prefab, ReplacePrefabOptions.ConnectToPrefab);
    }
}
