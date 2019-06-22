using System;
using System.Threading;

namespace UDLib.Network
{
    public class CLoginServerClt : CSLib.Utility.CSingleton<CLoginServerClt>
    {
        public bool isConnectFinish = false;
        public bool isConnected = false;

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

        // 开启多线程connect,否则服务器不可用的时候会卡住Unity21秒
        public void ConnectAsync(string serverIP, int serverPort)
        {
            Thread td = new Thread(()=> {
                isConnected = m_TcpClient.Connect(serverIP, serverPort, 3000);
                isConnectFinish = true;
            });
            isConnectFinish = false;
            td.Start();
        }
    }
}

