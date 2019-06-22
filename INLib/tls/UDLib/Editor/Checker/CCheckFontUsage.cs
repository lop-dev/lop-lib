using System.Collections;
using System.Collections.Generic;
using System.IO;
using UDLib.Editor;
using UnityEditor;
using UnityEngine;
using UnityEngine.UI;

public class CCheckFontUsage : Editor
{
    private static readonly string ROOTDIR = "Assets/App/Pro/GameRes/UIPrefabs";
    private static readonly string[] oldFonts = {"FiraCode-Bold","FiraCode-Light", "font2","font3","FZBeiWeiKaiShu","hanyiW",
        "HuaKan","HuaWen","LanTing","semiBold","simhei", "VeraMono", "VeraMono-Bold", "VeraMono-Bold-Italic", "VeraMono-Italic"};
    private static readonly string defaultFont = "Arial";
    [MenuItem("程序/修改工具/将预设体中淘汰字体替换为默认字体", false, 02020000)]
    public static void CheckFontUsage()
    {
        Font tofont = null;
        UnityEngine.Object[] objArr = Resources.FindObjectsOfTypeAll(typeof(Font));
        foreach (var v in objArr)
        {
            Font f = v as Font;
            if (f.name == defaultFont)
            {
                tofont = f;
                break;
            }
        }
        if (tofont == null)
        {
            Debug.LogError("未找到默认字体："+defaultFont);
            return;
        }
        Debug.Log("默认字体："+defaultFont);
        List<string> oldFontList = new List<string>(oldFonts);
        float curIndex = 0;
        int fileCount;
        string[] files = Directory.GetFiles(ROOTDIR, "*.prefab", SearchOption.AllDirectories);
        fileCount = files.Length;
        foreach(string file in files)
        {
            UDLib.Editor.CCheckTools.DisplayProgressBar("将淘汰字体替换为默认字体中",file,curIndex/fileCount);
            curIndex++;
            GameObject rootGO = AssetDatabase.LoadAssetAtPath<UnityEngine.Object>(file) as GameObject;
            if(null == rootGO)
            {
                Debug.LogError("加载预设体失败:" + file);
                continue;
            }

            bool hasChange = false;
            Text[] txts = rootGO.GetComponentsInChildren<Text>(true);
            foreach(var v in txts)
            {
                if (null == v.font)
                {
                    Debug.LogError("Font Missing :" + file + "-->" + GetChildGoPath(v.transform));
                    v.font = tofont;
                    EditorUtility.SetDirty(v);
                    continue;
                }
                if (oldFontList.Contains(v.font.name))
                {
                    Debug.Log(string.Format("{0}-->替换{1}:{2}", file, GetChildGoPath(v.transform), v.font.name));
                    v.font = tofont;
                    hasChange = true;
                    EditorUtility.SetDirty(v);
                }
            }
            if(hasChange)
            {
                EditorUtility.SetDirty(rootGO);
            }
        }

        AssetDatabase.SaveAssets();

        UDLib.Editor.CCheckTools.ClearProgressBar();
    }

    private static string GetChildGoPath(Transform tran)
    {
        string path = "";
        if (null == tran) return path;
        path += tran.name;
        Transform temptran = tran.parent;
        while(temptran != null)
        {
            path = temptran.name + "/" + path;
            temptran = temptran.parent;
        }
        return path;
    }
}
