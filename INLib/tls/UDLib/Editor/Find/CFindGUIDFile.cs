using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;

public class CFindGUIDFile
{
    private static string Root = "Assets";

    private static string[] guids = { "ac0b09e7857660247b1477e93731de29", "0b26ceeb1b57e0b48a447c7e037e7ea1", "95bdc27d98d8f784a815e9f872d77b41", "6ad980451443d70438faac0bc6c235a0", "f4044717213e31446939f7bd49c896ea", "6a9286fce23e449778e5092c8ce78ccd", "1c56f1253c2056d4c865bf47023542bb", "f70555f144d8491a825f0804e09c671c", "6f65812cf22e02048937dd1d6d93f951", "45e653bab7fb20e499bda25e1b646fea", "dab1fdbb510c4cd438177cb66744cffa", "627e8364c60cb7045a040e5b2c6bddcf", "e44a3b362fd9b034cb9b428c1c497d69", "b9a15381e244e834aaa78cb425aa17a9" , "728d485fadef3c74dbeb3e80c3dd1d05" };

    [MenuItem("程序/查找指定GUID文件")]
    public static void FindFileByGUID()
    {
        string[] files = Directory.GetFiles(Root,"*",SearchOption.AllDirectories);
        List<string> guidList = new List<string>();
        guidList.AddRange(guids);
        int index = 0;
        foreach (var v in files)
        {
            EditorUtility.DisplayProgressBar("处理中", v, (float)index++ / files.Length);
            if (v.EndsWith(".meta"))
            {
                continue;
            }
            string guid = AssetDatabase.AssetPathToGUID(v);
            if(string.IsNullOrEmpty(guid))
            {
                continue;
            }
            if(guidList.Contains(guid))
            {
                Debug.LogError(v+":"+guid);
            }
        }
        EditorUtility.ClearProgressBar();
    }
}
