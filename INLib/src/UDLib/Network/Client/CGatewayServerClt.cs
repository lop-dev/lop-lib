using System;
using System.Collections.Generic;
using System.Text;

namespace UDLib.Network
{
    public class CGatewayServerClt : CSLib.Utility.CSingleton<CGatewayServerClt>
    {
        public CGatewayServerClt()
        {
            m_TcpClient.CbTerminate = _CbTerminateMsg;
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

        private void _CbTerminateMsg()
        {
            ;
        }

        //
        private string m_strIP = "";
        private UInt16 m_nPort = 0;
        private CTcpClient m_TcpClient = new CTcpClient();

        // 保存一个最新的response 序列号，用于断线重连的时候告诉服务器从哪里开始重发
        private ushort resLatestIndex = 0;

        public ushort ResLatestIndex
        {
            set
            {
                if (value > resLatestIndex || resLatestIndex == ushort.MaxValue)
                {
                    resLatestIndex = value;
                }
            }
            get { return resLatestIndex; }
        }
    }
}
