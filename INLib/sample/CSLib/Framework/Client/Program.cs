using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization.Formatters.Binary;

namespace Client
{
class CClientMsg
{
    public CClientMsg(CSLib.Network.CTcpClient tcpClient)
    {
        m_tcpClient = tcpClient;

        // 此处使用简单的消息分发机制来演示
        m_msgFactory.AddMsgObject((UInt16)(Message.EMsgType.EMT_SAMPLE), (UInt16)Message.EMsgID.EMID_SAMPLE_NTF_OK, new Message.CMsgSampleNtfOk());
        m_msgFactory.AddMsgObject((UInt16)(Message.EMsgType.EMT_SAMPLE), (UInt16)Message.EMsgID.EMID_SAMPLE_REQ_SAY, new Message.CMsgSampleReqSay());
        m_msgFactory.AddMsgObject((UInt16)(Message.EMsgType.EMT_SAMPLE), (UInt16)Message.EMsgID.EMID_SAMPLE_RES_SAY, new Message.CMsgSampleResSay());
        m_msgExecute.MsgFactory = m_msgFactory;

        // 此处使用简单的消息处理机制来演示
        m_msgExecute.AddMsgExecFunc((UInt16)(Message.EMsgType.EMT_SAMPLE), (UInt16)Message.EMsgID.EMID_SAMPLE_RES_SAY, _OnMsgTestResSay);
    }

    public bool ParseMsg(byte[] msgBuff, int msgSize)
    {
        CSLib.Framework.CMessageLabel msgLabel = new CSLib.Framework.CMessageLabel();

        if(!m_msgExecute.ExecuteMessage(msgLabel, msgBuff, msgSize)) {
            Console.WriteLine("解析消息失败");
            return false;
        }
        return true;
    }

    public static void TerminateMsg()
    {
        Console.WriteLine("退出 RecvMsg");
    }

    private void _OnMsgTestResSay(CSLib.Framework.CMessageLabel msgLabel, CSLib.Framework.CMessage msg)
    {
        Message.CMsgSampleResSay resSay = (Message.CMsgSampleResSay)msg;
        Console.WriteLine("收到：" + resSay.m_string + resSay.m_bool + "  " + resSay.m_byte + "  " + resSay.m_bytes[0] + "  " + resSay.m_bytes[1] + "  " + resSay.m_bytes[2] + "  " + resSay.m_bytes[3] + "  " + resSay.m_bytes[4] + "  " + resSay.m_double + "  " + resSay.m_float + "  " + resSay.m_int + "  " + resSay.m_long + "  " + resSay.m_short + "  " + resSay.m_uint + "  " + resSay.m_ulong + "  " + resSay.m_ushort);
    }

    //
    private CSLib.Network.CTcpClient m_tcpClient;

    private CSLib.Framework.CMsgFactory m_msgFactory = new CSLib.Framework.CMsgFactory();
    private CSLib.Framework.CMsgExecute m_msgExecute = new CSLib.Framework.CMsgExecute();
}

class Program
{
    static void Main(string[] args)
    {
        CSLib.Network.CTcpClient tcpClient = new CSLib.Network.CTcpClient();

        CClientMsg clientMsg = new CClientMsg(tcpClient);
        tcpClient.CbParsMsg = clientMsg.ParseMsg;
        tcpClient.CbTerminateMsg = CClientMsg.TerminateMsg;

        if (!tcpClient.Connect("127.0.0.1", 9500))
        {
            Console.WriteLine("连接服务器失败");
            return;
        }
        Message.CMsgSampleNtfOk ntfOk = new Message.CMsgSampleNtfOk();
        
        CSLib.Utility.CStream stream = new CSLib.Utility.CStream();
        ntfOk.Serialize(stream);
        if (!tcpClient.SendAsync(stream))
        {
            return;
        }

        stream.Reset();
        Console.WriteLine(tcpClient.LocalIPEndPoint.ToString() + "--->" + tcpClient.PeerIPEndPoint.ToString());
        
        uint i = 1;
        //DateTime dt = new DateTime(2013,3,4);
       // decimal dec = 12.12M;
        while (true)
        {
            stream.Reset();

            Message.CMsgSampleReqSay reqSay = new Message.CMsgSampleReqSay();

            reqSay.m_string = "Client No.[";
            reqSay.m_string += i.ToString();
            reqSay.m_string += "]";
            byte[] vv ;
            vv = new byte[5];
            vv[1] = 1;
            vv[2] = 2;
            vv[3] = 3;
            vv[4] = 4;
            vv[0] = 5;
            reqSay.m_bool = true;
            reqSay.m_byte = (byte)i;
            reqSay.m_bytes = vv;
            reqSay.m_double = 1213.43435;
            reqSay.m_float = 123.234f;
            reqSay.m_int = 34534534;
            reqSay.m_long = 3245675756675L;
            reqSay.m_short = 32000;
            reqSay.m_uint = 2100000000;
            reqSay.m_ulong = 94242424242342L;
            reqSay.m_ushort = 65000;
            reqSay.Serialize(stream);

            i++;

            Console.WriteLine(reqSay.m_string + reqSay.m_bool + "  "+reqSay.m_byte+"  " +reqSay.m_bytes[0]+"  "+reqSay.m_bytes[1]+"  "+reqSay.m_bytes[2]+"  "+reqSay.m_bytes[3]+"  "+reqSay.m_bytes[4]+"  "+reqSay.m_double +"  "+reqSay.m_float+"  "+reqSay.m_int+"  "+reqSay.m_long+"  "+reqSay.m_short+"  "+reqSay.m_uint+"  "+reqSay.m_ulong+"  "+reqSay.m_ushort);

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
