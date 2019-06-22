using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class CAnimationClipPostProcessor : AssetPostprocessor
{
    static string[] filters = { "idle", "run", "sleep", "coma", "angry01","stand01","loop01","walk01" };//小写
    static void OnPostprocessAllAssets(string[] importedAssets, string[] deletedAssets, string[] movedAssets, string[] movedFromAssetPaths)
    {
        foreach (string str in importedAssets)
        {
            //Debug.Log("Reimported Asset: " + str);
            if (str.EndsWith(".anim"))
            {
                string lowerPath = str.ToLower();
                bool bcontains = false;
                foreach(string filter in filters)
                {
                    if (lowerPath.IndexOf(filter) >= 0)
                    {
                        bcontains = true;
                        break;
                    }
                }
                if (!bcontains) return;
                string newStr = str.Replace('\\', '/');
                AnimationClip aniClip = AssetDatabase.LoadAssetAtPath<AnimationClip>(newStr);
                if (null == aniClip)
                    return;
                AnimationClipSettings clipsetting = AnimationUtility.GetAnimationClipSettings(aniClip);
                if (clipsetting.loopTime)
                    return;
                clipsetting.loopTime = true;
                AnimationUtility.SetAnimationClipSettings(aniClip, clipsetting);
                AssetDatabase.SaveAssets();
            }
        }
        foreach (string str in deletedAssets)
        {
            //Debug.Log("Deleted Asset: " + str);
        }

        for (int i = 0; i < movedAssets.Length; i++)
        {
            //Debug.Log("Moved Asset: " + movedAssets[i] + " from: " + movedFromAssetPaths[i]);
        }
    }
}
