using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using UnityEditor;
using UnityEngine;
using UnityEngine.Experimental.UIElements;

public class CCodeGenECSSystem:EditorWindow
{
    private static readonly string WIN_NAME = "添加ECS-System";
    private static readonly string DEST_PATH = "Assets/App/Pro/Scripts/GamePlay/Systems/LogicLayer/";
    private static readonly string []MODEL_PATH = { "Assets/App/Pro/Templates/CSys.txt", "Assets/App/Pro/Templates/CSysProcess.txt" };
    private static string fileName = "文件名";

    [MenuItem("程序/代码生成工具/添加ECS-System",false,02040000)]
    public static void CodeGenECSSystem()
    {
        CCodeGenECSSystem ecsSys = EditorWindow.GetWindowWithRect<CCodeGenECSSystem>(new Rect(640,420,340,50),true, WIN_NAME);
        ecsSys.Show();
    }

    private void OnGUI()
    {
        EditorGUILayout.BeginHorizontal();
        {
            EditorGUILayout.LabelField("Name",GUILayout.Width(40));
            fileName = EditorGUILayout.TextField(fileName, GUILayout.Width(200));

            if(GUILayout.Button("生成",GUILayout.Width(80)))
            {
                StreamReader sr1 = new StreamReader(MODEL_PATH[0],Encoding.UTF8);
                StreamWriter sw1 = new StreamWriter(DEST_PATH + string.Format("C{0}Sys.cs", fileName),false,Encoding.UTF8);
                string newfile1 = sr1.ReadToEnd();
                newfile1 = newfile1.Replace("[NAME]", fileName);
                sw1.Write(newfile1);
                sw1.Flush();
                sr1.Close();
                sw1.Close();

                StreamReader sr2 = new StreamReader(MODEL_PATH[1],Encoding.UTF8);
                StreamWriter sw2 = new StreamWriter(DEST_PATH + string.Format("C{0}SysProcess.cs",fileName),false,Encoding.UTF8);
                string newfile2 = sr2.ReadToEnd();
                newfile2 = newfile2.Replace("[NAME]", fileName);
                sw2.Write(newfile2);
                sw2.Flush();
                sr2.Close();
                sw2.Close();
                AssetDatabase.Refresh();
            }
        }
        EditorGUILayout.EndHorizontal();
    }
}
