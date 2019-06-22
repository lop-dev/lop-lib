using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class LanguageTool : EditorWindow {
    private static string WINDOW_NAME = "Language";
    private List<string> m_languageList = new List<string>() { "CN", "TW", "EN" };
    private Vector2 m_scrollTotal;
    private Dictionary<string, bool> m_languageDic = new Dictionary<string, bool>();

    [MenuItem("程序/Language", false, 02000000)]
    static void open()
    {
        LanguageTool editor = GetWindowWithRect<LanguageTool>(new Rect(640, 420, 400, 400), true, WINDOW_NAME);
        editor.Init();
        editor.Show();
    }

    private void Init()
    {
        foreach (string lan in m_languageList)
        {
            m_languageDic.Add(lan, false);
        }
    }

    private void OnGUI()
    {
        m_scrollTotal = EditorGUILayout.BeginScrollView(m_scrollTotal, GUILayout.Width(400), GUILayout.Height(590));

        GUILayout.Label("请选择语言：");
        EditorGUILayout.Space();

        foreach(string lan in m_languageList)
        {
            EditorGUILayout.BeginHorizontal();
            m_languageDic[lan] = EditorGUILayout.Toggle(lan, m_languageDic[lan]);
            EditorGUILayout.EndHorizontal();
        }

        EditorGUILayout.Space();
        if (GUILayout.Button("确定"))
        {
            SetLanguage();
            Close();
        }

        EditorGUILayout.EndScrollView();
    }

    private void SetLanguage()
    {
        float progess = 0;
        float proDelta = 50f / m_languageList.Count;
        // 删除原有语言目录
        foreach (string lan in m_languageList)
        {
            string textFolder = string.Format("{0}/App/Pro/GameRes/i18n/Language_{1}/", Application.dataPath, lan.ToUpper());
            string UIFolder = string.Format("{0}/App/Pro/GameRes/UI/Module_{1}/", Application.dataPath, lan.ToUpper());
            if (System.IO.Directory.Exists(textFolder))
            {
                progess += proDelta / 2;
                EditorUtility.DisplayProgressBar("language", "copy language...", progess);
                FileUtil.DeleteFileOrDirectory(textFolder);
            }
            if (System.IO.Directory.Exists(UIFolder))
            {
                progess += proDelta / 2;
                EditorUtility.DisplayProgressBar("language", "copy language...", progess);
                FileUtil.DeleteFileOrDirectory(UIFolder);
            }
        }

        // 拷贝需要的语言目录
        foreach (string lan in m_languageList)
        {
            progess += proDelta;
            EditorUtility.DisplayProgressBar("language", "copy language...", progess);
            if (m_languageDic[lan] == true)
            {
                CopyLanguage(lan);
            }
        }

        EditorUtility.ClearProgressBar();
    }

    private void CopyLanguage(string Language)
    {
        // 文本
        string sourceFolder = string.Format("{0}/../i18n/Language/Language_{1}/", Application.dataPath, Language.ToUpper());
        string destFolder = string.Format("{0}/App/Pro/GameRes/i18n/Language_{1}/", Application.dataPath, Language.ToUpper());
        string errorMsg = string.Format("多语言文本目录不存在：{0}", sourceFolder);
        CopyFolder(sourceFolder, destFolder, errorMsg);


        // 图片
        sourceFolder = string.Format("{0}/../i18n/UI/Module_{1}/", Application.dataPath, Language.ToUpper());
        destFolder = string.Format("{0}/App/Pro/GameRes/UI/Module_{1}/", Application.dataPath, Language.ToUpper());
        errorMsg = string.Format("多语言图片目录不存在：{0}", sourceFolder);
        CopyFolder(sourceFolder, destFolder, errorMsg);
        Debug.Log(string.Format("语言{0}处理完毕", Language));
    }

    private bool CopyFolder(string sourceFolder, string destFolder, string errorMsg)
    {
        if (!System.IO.Directory.Exists(sourceFolder))
        {
            Debug.LogError(errorMsg);
            return false;
        }

        if (System.IO.Directory.Exists(destFolder))
        {
            FileUtil.DeleteFileOrDirectory(destFolder);
        }

        FileUtil.CopyFileOrDirectory(sourceFolder, destFolder);

        return true;
    }
}
