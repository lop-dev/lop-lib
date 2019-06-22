/* ***********************************************
 * RawResourceProcess
 * author :  created by asvo
 * function: 一些对原生资源的中间处理操作
 * history:  created at .
 * ***********************************************/

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class RawResourceProcess {

	[MenuItem("Tools/DealRawRes/UiVFX")]
    public static void ChangeToUiVfx()
    {
        GameObject gobj = Selection.activeGameObject;
        if (null == gobj)
            return;
        ParticleSystem[] psArr = gobj.GetComponentsInChildren<ParticleSystem>(true);
        foreach(var ps in psArr)
        {
            Renderer render = ps.GetComponent<Renderer>();
            if (null != render)
                render.sortingLayerName = "UIvfx";
        }
    }
}
