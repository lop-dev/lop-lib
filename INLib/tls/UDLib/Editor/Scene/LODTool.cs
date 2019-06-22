/* ***********************************************
 * TestDealLoD
 * author :  created by asvo
 * function: 
 * history:  created at .
 * ***********************************************/

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEngine.SceneManagement;

public class LODTool : Editor 
{
    [MenuItem("美术/场景工具/烘焙当前场景时的处理/烘焙前关闭场景LOD",false,030101)]
    public static void DiselectLOD()
    {

        Scene scene = SceneManager.GetActiveScene();
        foreach (GameObject go in scene.GetRootGameObjects())
        {
            LODGroup[] compos = go.GetComponentsInChildren<LODGroup>(true);
            foreach (var compo in compos)
            {
                compo.enabled = false;
            }
        }
    }

    [MenuItem("美术/场景工具/烘焙当前场景时的处理/烘焙后打开场景LOD",false,030101)]
    public static void SelectLOD()
    {
        Scene scene = SceneManager.GetActiveScene();
        foreach (GameObject go in scene.GetRootGameObjects())
        {
            LODGroup[] compos = go.GetComponentsInChildren<LODGroup>(true);
            foreach (var compo in compos)
            {
                compo.enabled = true;
            }
        }
    }
}
