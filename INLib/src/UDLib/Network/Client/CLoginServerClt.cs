using System;
using System.Threading;

namespace UDLib.Network
{
    public class CLoginServerClt : CSLib.Utility.CSingleton<CLoginServerClt>
    {
        public CLoginServerClt()
        {
        }

        public string IP
        {
            set { m_strIP = value; }
            get { return m_strIP; }
        }

        public UInt16 Port
        {
            set { m_nPort = value; }
            get { return m_nPort; }
        }

        public CTcpClient TcpClient
        {
            get { return m_TcpClient; }
        }

        //
        private string m_strIP = "";
        private UInt16 m_nPort = 0;
        private CTcpClient m_TcpClient = new CTcpClient();
    }
}

