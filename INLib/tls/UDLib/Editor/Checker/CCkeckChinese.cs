using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using UnityEditor;
using UnityEngine;

public class CCHeckChinese : Editor {
    [MenuItem ("程序/检查工具/检查lua脚本中的中文", false, 02010081)]
    public static void Scan () {
        string luaPath = Application.dataPath + "/App/Pro/Lua";
        List<string> allFiles = Directory.GetFiles (luaPath, "*.lua", SearchOption.AllDirectories).ToList ();

        EditorUtility.ClearProgressBar ();
        int i = 1;

        var uuidReg = new Regex (@"[\u4e00-\u9fa5]");

        List<string> result = new List<string> ();
        Dictionary<string, List<string>> ret = new Dictionary<string, List<string>>();

        foreach (var file in allFiles) {
            EditorUtility.DisplayProgressBar ("扫描中", Path.GetFileName (file), (float) i / allFiles.Count);
            i++;

            StreamReader sr = new StreamReader (file);
            string line;
            while ((line = sr.ReadLine ()) != null) {
                var m = uuidReg.Match (line);
                if (m.Success) {
                    if (!line.Contains("--"))
                    {
                        string fileName = Path.GetFileName(file);
                        if (!ret.ContainsKey(fileName))
                        {
                            ret[fileName] = new List<string>();
                        }

                        ret[fileName].Add(line.Trim());
                    }
                }
            }
            sr.Close ();
        }

        StringBuilder sb = new StringBuilder ();
        foreach (var keyValuePair in ret)
        {
            sb.Append(keyValuePair.Key).Append(Environment.NewLine);
            foreach (var s in keyValuePair.Value)
            {
                sb.Append("    ").Append(s).Append(Environment.NewLine);
            }
        }

        string retName = "fileList.txt";
        string filePath = Path.Combine (Application.temporaryCachePath, retName);
        if (File.Exists (filePath)) {
            File.Delete (filePath);
        }

        File.WriteAllText (filePath, sb.ToString ());
        System.Diagnostics.Process.Start (filePath);

        EditorUtility.ClearProgressBar ();
    }
}