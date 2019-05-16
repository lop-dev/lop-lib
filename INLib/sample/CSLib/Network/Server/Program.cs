using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Linq.Expressions;


namespace Server
{
class CTcpStubMsg
{
    CTcpStubMsg(CSLib.Network.CTcpStub netStub)
    {
        m_netStub = netStub;
    }

    public bool ParseMsgCallback(byte[] msgBuff, int msgSize)
    { 
        m_stream.Write(msgBuff, 0, msgSize);

        Console.WriteLine(m_stream.ReadString() + "{" + m_netStub.LocalIPEndPoint.ToString() + "<---" + m_netStub.PeerIPEndPoint.ToString() + "}");

        string data = "Sever No.[";
        data += m_number.ToString();
        data += "]";

        m_stream.Write(data);

        m_number++;

        Console.WriteLine(data.ToString() + "{" + m_netStub.LocalIPEndPoint.ToString() + "--->" + m_netStub.PeerIPEndPoint.ToString() + "}");

        if (!m_netStub.SendAsync(m_stream))
        {
            return false;
        }

        return true;
    }

    public void TerminateMsgCallback()
    {
        Console.WriteLine("退出 TerminateMsg");
    }

    public static CSLib.Network.CTcpStub CbNewTcpStub(CSLib.Network.CNetConnection netConnection, object tmpData)
    {
        CSLib.Network.CTcpStub netStub = new CSLib.Network.CTcpStub(netConnection);
        CTcpStubMsg tcpStubMsg = new CTcpStubMsg(netStub);

        netStub.CbParsMsg = tcpStubMsg.ParseMsgCallback;
        netStub.CbTerminateMsg = tcpStubMsg.TerminateMsgCallback;

        return netStub;
    }

    private int m_number = 1;
    private CSLib.Network.CTcpStub m_netStub;
    private CSLib.Utility.CStream m_stream = new CSLib.Utility.CStream();
}

class Program
{
    static void Main(string[] args)
    {
        CSLib.Network.CTcpServer netServer = new CSLib.Network.CTcpServer();

        netServer.AddTcpAcceptor("0.0.0.0", 9000, null);

        netServer.CbNewTcpStub = CTcpStubMsg.CbNewTcpStub;

        netServer.Start();

        while (true)
        {
            System.Threading.Thread.Sleep(10);
        }
    }
}
}
