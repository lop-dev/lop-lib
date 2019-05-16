using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Client
{
class CClientMsg
{
    public static bool ParseMsg(byte[] msgBuff, int msgSize)
    {
        CSLib.Utility.CStream stream = new CSLib.Utility.CStream();
        
        stream.Write(msgBuff, 0, msgSize);

        Console.WriteLine(stream.ReadString());
        return true;
    }

    public static void TerminateMsg()
    {
        Console.WriteLine("退出 RecvMsg");;
    }
}

class Program
{
    static void Main(string[] args)
    {
        CSLib.Network.CTcpClient tcpClient = new CSLib.Network.CTcpClient();

        tcpClient.CbParsMsg = CClientMsg.ParseMsg;
        tcpClient.CbTerminateMsg = CClientMsg.TerminateMsg;

        if (!tcpClient.Connect("127.0.0.1", 9000))
        {
            Console.WriteLine("连接服务器失败");
            return;
        }

        CSLib.Utility.CStream stream = new CSLib.Utility.CStream();

        Console.WriteLine(tcpClient.LocalIPEndPoint.ToString() + "--->" + tcpClient.PeerIPEndPoint.ToString());
        
        uint i = 1;

        while (true)
        {
            string data = "Client No.[";
            data += i.ToString();
            data += "]";

            stream.Write(data);

            i++;

            Console.WriteLine(data.ToString() + "{" + tcpClient.LocalIPEndPoint.ToString() + "--->" + tcpClient.PeerIPEndPoint.ToString() + "}");

            if (!tcpClient.SendAsync(stream))
            {
                break;
            }

            stream.Reset();

            System.Threading.Thread.Sleep(1000);
        }
    }
}
}
