using System;
using System.Diagnostics;
using System.IO;
using System.Text;
using UnityEditor;
using UnityEngine;
namespace UDLib.Editor
{
    public static class CCheckLuaScripts
    {
        [MenuItem("程序/检查工具/检查Lua脚本", false, 02010081)]
        public static void Scan()
        {

            EditorUtility.ClearProgressBar();
            var luaFiles = Directory.GetFiles(CPath.InstallPackAppProluaGameModule, "*.lua", SearchOption.AllDirectories);

            var sb = new StringBuilder();
            luaFiles.ForEachI((f, i) =>
            {
                var fileName = Path.GetFileNameWithoutExtension(f);
                EditorUtility.DisplayProgressBar("扫描 " + fileName, f, (float)i / luaFiles.Length);

                var file = new StreamReader(f);
                string line;
                var lineNum = 0;
                while ((line = file.ReadLine()) != null)
                {
                    var s = line.Trim();
                    if (!s.StartsWith("--"))
                    {
                        if (s.Contains(".super:"))
                            sb.Append("Error: " + fileName + " line " + lineNum + " super : should be super." +
                                      Environment.NewLine);

                        if (s.Contains("Vector3.New"))
                            sb.Append("Warning: " + fileName + " line " + lineNum + " use Vector3.New. Too Expensive!" +
                                      Environment.NewLine);

                        if (s.Contains("localPosition"))
                            sb.Append("Warning: " + fileName + " line " + lineNum +
                                      " use CGameObjectHelper.SetLocalPosition instead of .localPosition=" +
                                      Environment.NewLine);

                        if (s.Contains("localScale"))
                            sb.Append("Warning: " + fileName + " line " + lineNum +
                                      " use CGameObjectHelper.SetLocalScale instead of .localScale=" +
                                      Environment.NewLine);

                        if (s.Contains(".color"))
                            sb.Append("Warning: " + fileName + " line " + lineNum +
                                      " use CGameObjectHelper.SetTextColor instead of .color=" +
                                      Environment.NewLine);

                        if (s.Contains(".anchoredPosition"))
                            sb.Append("Warning: " + fileName + " line " + lineNum +
                                      " use CGameObjectHelper.SetAnchoredPosition instead of .anchoredPosition=" +
                                      Environment.NewLine);
                    }

                    lineNum++;
                }
            });

            if (File.Exists("Assets/LuaCheck.txt")) File.Delete("Assets/LuaCheck.txt");

            File.WriteAllText("Assets/LuaCheck.txt", sb.ToString(), Encoding.UTF8);

            Process.Start(Application.dataPath + "/LuaCheck.txt");
            EditorUtility.ClearProgressBar();
        }
    }
}
