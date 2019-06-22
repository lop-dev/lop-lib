using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;
using UnityEngine.SceneManagement;

/// <summary>
/// 预制生成
/// </summary>
public class PrefabBuilder : Editor
{
	/// <summary>
	/// 对选中的场景物件生成prefab
	/// </summary>
	[MenuItem("美术/场景工具/将当前场景中选中的对象生成Prefab文件",false,030101)]
    private static void Test()
	{
        string prefabOutPath = CreateSceneObjectsFolderIfNeed();

        //
        Dictionary<string, int> nameListToNum = new Dictionary<string, int>();
		foreach (var selectObj in Selection.gameObjects)
		{
			string trimName = selectObj.name.Replace(" ", string.Empty);
			string useName = trimName;
			if (nameListToNum.ContainsKey(trimName))
			{
				nameListToNum[trimName] = nameListToNum[trimName] + 1;
				useName = trimName + "_" + nameListToNum[trimName];
			}
			else
			{
				nameListToNum.Add(trimName, 0);
			}
			GameObject createdGobj = PrefabUtility.CreatePrefab(prefabOutPath + "/" + useName + ".prefab", selectObj);
			PrefabUtility.ConnectGameObjectToPrefab(selectObj, createdGobj);
		}
		AssetDatabase.Refresh();
	}
	
	//[MenuItem("Test/TestScene-Upgrade")]
	private static void Test_Upgrade()
	{
		string prefabOutPath = CreateSceneObjectsFolderIfNeed();
		GameObject parentRoot = new GameObject("TempParentRoot");
		GameObject[] selectGobjs = Selection.gameObjects; 
		for(int i = 0; i < selectGobjs.Length; ++i)
		{
			GameObject selectObj = selectGobjs[i];
			Debug.Log(selectObj.name);
			GameObject gobj = PrefabUtility.InstantiateAttachedAsset(selectObj) as GameObject;
			gobj.name = selectObj.name;
			gobj.transform.SetParent(parentRoot.transform);
			
			//Create prefab to project
			PrefabUtility.CreatePrefab(prefabOutPath + "/" + selectObj.name + ".prefab", gobj);
			if (i == selectGobjs.Length - 1)
			{
				EditorUtility.DisplayProgressBar(string.Empty, string.Empty, 1);
			}
			else
			{
				EditorUtility.DisplayProgressBar("正在生成场景物件预制...", string.Format("{0}/{1}", i, selectGobjs.Length), (i / (float)selectGobjs.Length));	
			}
		}
		GameObject.DestroyImmediate(parentRoot);
		
		AssetDatabase.Refresh();
	}

    private static string CreateSceneObjectsFolderIfNeed()
    {
        string prefabOutPath = "Assets/App/Pro/GameRes/Scenes/SceneObjects/" + SceneManager.GetActiveScene().name;
        if (!Directory.Exists(prefabOutPath))
        {
            Directory.CreateDirectory(prefabOutPath);
        }

        return prefabOutPath;
    }
}


