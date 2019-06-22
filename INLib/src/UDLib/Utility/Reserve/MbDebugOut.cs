using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;

public class MbDebugOut : MonoBehaviour
{
    static List<string> m_listText = new List<string>();
    private string m_strOutPath;
    public bool DebugLog = false;

    protected void OnEnable()
    {
        Application.RegisterLogCallback(_HandleLog);
    }

    protected void OnDisable()
    {
        Application.RegisterLogCallback(null);
    }

    protected void Start()
    {
        m_strOutPath = Application.persistentDataPath + "/" + System.DateTime.Today.ToString("yyyy-MM-dd") + "  outLog.txt";

        //删除7天外的日志文件
        DirectoryInfo direc = new DirectoryInfo(Application.persistentDataPath);
        FileInfo[] info = direc.GetFiles("*outlog.txt", SearchOption.TopDirectoryOnly);

        if (info != null)
        {
            foreach (FileInfo item in info)
            {
                TimeSpan span = DateTime.Now.Subtract(item.LastWriteTime.Date);
                if (span.TotalDays > 7)
                {
                    File.Delete(item.FullName);
                }
            }
        }
    }

    protected void Update()
    {
        if (m_listText.Count > 0)
        {
            string[] temp = m_listText.ToArray();
            foreach (var item in temp)
            {
                using (StreamWriter writer = new StreamWriter(m_strOutPath, true, Encoding.UTF8))
                {
                    writer.WriteLine(item);
                }
                m_listText.Remove(item);
            }
        }
    }

    //
    private void _HandleLog(string logString, string stackTrace, LogType type)
    {
        switch (type)
        {
            case LogType.Error:
            case LogType.Exception:
                m_listText.Add("\"OutputInfo\": " + logString);
                m_listText.Add("\"StackInfo\":  " + stackTrace);
                break;
            case LogType.Assert:
            case LogType.Log:
            case LogType.Warning:
                if (!DebugLog)
                {
                    return;
                }
                m_listText.Add("\"OutputInfo\":" + logString);
                break;
            default:
                break;
        }

        m_listText.Add("===========================");
    }
}
