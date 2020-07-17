using System;
using System.Collections.Generic;
using System.IO;

namespace Proto2Code
{
    class CConfigFile : CSLib.Utility.CSingleton<CConfigFile>
    {
        public Queue<string> DescPath
        {
            get { return m_queDescPath; }
        }

        public Queue<string> DataPath
        {
            get { return m_queDataPath; }
        }

        public Queue<string> ProtoPath
        {
            get { return m_queProtoPath; }
        }

        public Queue<string> ProtoXlsx
        {
            get { return m_queProtoXlsx; }
        }

        public bool LoadFile()
        {
            string exeFileName = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
            FileInfo tmpFileInfo = new FileInfo(exeFileName);
            string iniFileName = tmpFileInfo.DirectoryName + "\\LopGeneration.ini";
            string[] fileLines = File.ReadAllLines(iniFileName);

            foreach (var line in fileLines)
            {
                string strLine = line.Trim();
                if (strLine.StartsWith("/")) continue;

                if (strLine.StartsWith("[DescPath]"))
                {
                    _Reset();
                    m_bDescPath = true;
                    continue;
                }
                else if (strLine.StartsWith("[DataPath]"))
                {
                    _Reset();
                    m_bDataPath = true;
                    continue;
                }
                else if (strLine.StartsWith("[ProtoPath]"))
                {
                    _Reset();
                    m_bProtoPath = true;
                    continue;
                }
                else if (strLine.StartsWith("[ProtoXlsx]"))
                {
                    _Reset();
                    m_bProtoXlsx = true;
                    continue;
                }

                if (m_bDescPath)
                {
                    if (strLine.StartsWith("DescPath"))
                    {
                        String[] tmpArr = strLine.Split('=');
                        if (tmpArr.Length != 2) continue;
                        m_queDescPath.Enqueue(tmpArr[1].Trim());
                        continue;
                    }
                }
                else if(m_bDataPath)
                {
                    if (strLine.StartsWith("DataPath"))
                    {
                        String[] tmpArr = strLine.Split('=');
                        if (tmpArr.Length != 2) continue;
                        m_queDataPath.Enqueue(tmpArr[1].Trim());
                        continue;
                    }
                }
                else if (m_bProtoPath)
                {
                    if (strLine.StartsWith("ProtoPath"))
                    {
                        String[] tmpArr = strLine.Split('=');
                        if (tmpArr.Length != 2) continue;
                        m_queProtoPath.Enqueue(tmpArr[1].Trim());
                        continue;
                    }
                }
                else if (m_bProtoXlsx)
                {
                    if (strLine.StartsWith("ProtoXlsx"))
                    {
                        String[] tmpArr = strLine.Split('=');
                        if (tmpArr.Length != 2) continue;
                        m_queProtoXlsx.Enqueue(tmpArr[1].Trim());
                        continue;
                    }
                }
            }

            return true;
        }

        private void _Reset()
        {
            m_bDescPath = false;
            m_bDataPath = false;
            m_bProtoPath = false;
            m_bProtoXlsx = false;
        }

        //
        private bool m_bDescPath = false;
        private bool m_bDataPath = false;
        private bool m_bProtoPath = false;
        private bool m_bProtoXlsx = false;
        private Queue<string> m_queDescPath = new Queue<string>();
        private Queue<string> m_queDataPath = new Queue<string>();
        private Queue<string> m_queProtoPath = new Queue<string>();
        private Queue<string> m_queProtoXlsx = new Queue<string>();
    }
}
