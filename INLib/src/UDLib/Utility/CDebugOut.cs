using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using UnityEngine;

namespace UDLib.Utility
{
    class CDebugFile
    {
        public CDebugFile(string strDirectory, string strPrefixName, string strSuffixName)
        {
            m_strDirectory = strDirectory;
            m_strPrefixName = strPrefixName;
            m_strSuffixName = strSuffixName;
        }

        public int LineCnt
        {
            set
            {
                m_iLineCnt = value;
            }
        }

        private bool _Create()
        {
            Close();

            if (!Directory.Exists(m_strDirectory))
            {
                try
                {
                    Debug.Log("DirectoryInfo: " + m_strDirectory);
                    if (Application.platform != RuntimePlatform.Android)
                    {
                        Directory.CreateDirectory(m_strDirectory);
                    }

                }
                catch (IOException ex)
                {
                    CConsole.WriteLine(ex);
                }
            }

            //DirectoryInfo tmpDirectoryInfo = new DirectoryInfo(m_strDirectory);

            //if (!tmpDirectoryInfo.Exists)
            //{
            //    try
            //    {
            //        Debug.Log("DirectoryInfo: " + tmpDirectoryInfo.FullName);
            //        tmpDirectoryInfo.Create();
            //    }
            //    catch (IOException ex)
            //    {
            //        CConsole.WriteLine(ex);
            //    }
            //}

            //
            string strDateTime = DateTime.Now.ToString("yy-MM-dd HH:mm:ss");
            strDateTime = strDateTime.Replace("-", "");
            strDateTime = strDateTime.Replace(" ", "_");
            strDateTime = strDateTime.Replace(":", "");

            string strFileName = m_strPrefixName + "_" + strDateTime + "_" + m_strSuffixName;

            strFileName = System.IO.Path.Combine(m_strDirectory, strFileName + ".Log");

            if (Application.platform == RuntimePlatform.Android)
            {
                var dirName = "log";
                var fileName = Path.GetFileName(strFileName);

                string fullPath = string.Empty;
                try
                {
                    AndroidJavaClass jc = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
                    AndroidJavaObject jo = jc.GetStatic<AndroidJavaObject>("currentActivity");
                    fullPath = jo.CallStatic<string>("createFile", dirName, fileName);
                }
                catch (Exception e)
                {
                    Debug.LogException(e);
                }

                if (!string.IsNullOrEmpty(fullPath))
                {
                    strFileName = fullPath;
                }
            }

            try
            {
                FileStream fs = File.Open(strFileName, FileMode.Create, FileAccess.Write, FileShare.ReadWrite);
                if (fs == null)
                {
                    return false;
                }
                //FileInfo tmpFileInfo = new FileInfo(strFileName);
                //m_StreamWriter = tmpFileInfo.CreateText();
                m_StreamWriter = new StreamWriter(fs);
                CConsole.WriteLine("CDebugFile : " + strFileName);
            }
            catch (System.Exception ex)
            {
                m_StreamWriter = null;
                CConsole.WriteLine(ex);
            }

            m_iLineNum = 0;

            return true;
        }

        public void WriteLine(string strLine)
        {
            if (m_StreamWriter == null)
            {
                if (!_Create())
                {
                    return;
                }
            }

            try
            {
                m_StreamWriter.WriteLine(DateTime.Now.ToString(CSLib.Utility.CBaseDateTime.Format) + " " + strLine);
                m_StreamWriter.Flush();
            }
            catch (System.Exception ex)
            {
                CConsole.WriteLine(ex);
            }

            m_iLineNum++;
            if (m_iLineNum >= m_iLineCnt)
            {
                _Create();
            }
        }

        public void Close()
        {
            if (m_StreamWriter != null)
            {
                m_StreamWriter.Close();
            }
        }

        private string m_strDirectory = "";
        private string m_strPrefixName = "";
        private string m_strSuffixName = "";
        private StreamWriter m_StreamWriter = null;
        private int m_iLineNum = 0;
        private int m_iLineCnt = 100000;
    }

    //////////////////////////////////////////////////////////////////////////

    public class CDebugOut : CSLib.Utility.CDebugOut
    {
        public CDebugOut()
        {
            WriteFile = false;
            // 为解决 AOT 问题，需要切换注释下面1行
            //CurDirectory = "..\\Log\\";
            CurDirectory = System.IO.Path.Combine(Application.persistentDataPath, m_dirName);
            Debug.Log(CurDirectory);
            _DeleteOverdueFiles();
        }

        public CDebugOut(string strFileName) : base(strFileName)
        {
            WriteFile = false;
            // 为解决 AOT 问题，需要切换注释下面1行
            //CurDirectory = "..\\Log\\";
            CurDirectory = System.IO.Path.Combine(Application.persistentDataPath, m_dirName);
            Debug.Log(CurDirectory);
            _DeleteOverdueFiles();
        }

        //
        public new static CSLib.Utility.CDebugOut Default
        {
            get
            {
                if (m_Singleton == null)
                {
                    m_Singleton = new CDebugOut();
                    CSLib.Utility.CDebugOut.Default = m_Singleton;
                }

                return m_Singleton;
            }
        }

        public override void SetLineCnt(int nCount)
        {
            m_dfNormal.LineCnt = nCount;
            m_dfWarning.LineCnt = nCount;
            m_dfError.LineCnt = nCount;
        }

        public override void DisplayMsg(bool bPopUp, CSLib.Utility.CDebugOut.LEVEL iLevel, string strTitle, object strFormat)
        {
            CConsole.WriteLine(iLevel, strTitle + " : " + strFormat);
            _WriteDebugFile(iLevel, strTitle, strFormat.ToString());

            if (bPopUp)
            {
                //ShowMessageBox(iLevel, null, strTitle, strFormat.ToString());
            }
        }

        protected override void _WriteDebugFile(CSLib.Utility.CDebugOut.LEVEL iLevel, string strTitle, string strFormat)
        {
            if (WriteFile == false)
            {
                return;
            }

            //
            if (m_dfNormal == null)
            {
                m_dfNormal = new CDebugFile(CurDirectory, FileName, "N");
            }
            m_dfNormal.WriteLine(strTitle + " : " + strFormat);

            //
            if (iLevel == CSLib.Utility.CDebugOut.LEVEL.WARNING)
            {
                if (m_dfWarning == null)
                {
                    m_dfWarning = new CDebugFile(CurDirectory, FileName, "W");
                }
                m_dfWarning.WriteLine(strTitle + " : " + strFormat);
            }
            else if (iLevel == CSLib.Utility.CDebugOut.LEVEL.ERROR)
            {
                if (m_dfError == null)
                {
                    m_dfError = new CDebugFile(CurDirectory, FileName, "E");
                }
                m_dfError.WriteLine(strTitle + " : " + strFormat);
            }
        }

        //
        private static CSLib.Utility.CDebugOut m_Singleton;

        private string m_dirName = "Log";
        private CDebugFile m_dfNormal = null;
        private CDebugFile m_dfWarning = null;
        private CDebugFile m_dfError = null;
    }
}