using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Linq.Expressions;
using System.Runtime.Serialization.Formatters.Binary;

namespace Server
{
class CTcpStubMsg
{
    CTcpStubMsg(CSLib.Network.CTcpStub netStub)
    {
        m_netStub = netStub;

        // 此处使用繁琐的消息分发机制来演示
        m_msgFactory.MsgCreater = CMsgFactory.MsgCreater;
        m_msgExecute.MsgFactory = m_msgFactory;

        // 此处使用繁琐的消息处理机制来演示
        m_msgExecFuncFactory.SetMsgExecFun(_CreateMsgExecFun);
        m_msgExecute.MsgExecFuncFuctory = m_msgExecFuncFactory;
    }

    //
    private bool _CreateMsgExecFun(UInt16 type, UInt16 id, ref CSLib.Framework.DMsgExecFunc msgExecut)
    {
        Message.EMsgType msgType = (Message.EMsgType)type;
        switch (msgType)
        {
            case Message.EMsgType.EMT_SAMPLE:
                {
                    return _SampleMsgExecFun(id, ref msgExecut);
                }
        }
        Console.WriteLine("无法解析的消息类型{0}", type);
        return false;
    }

    private bool _SampleMsgExecFun(ushort id, ref CSLib.Framework.DMsgExecFunc msgExecut)
    {
        Message.EMsgID msgID = (Message.EMsgID)id;
        switch (msgID)
        {
            case Message.EMsgID.EMID_SAMPLE_NTF_OK:
                {
                    msgExecut = _OnMsgTestNtfOk;
                    return true;
                }
            case Message.EMsgID.EMID_SAMPLE_REQ_SAY:
                {
                    msgExecut = _OnMsgTestReqSay;
                    return true;
                }
        }
        Console.WriteLine("无法解析的消息ID{0}", id);
        return false;
    }

    //
    private void _OnMsgTestReqSay(CSLib.Framework.CMessageLabel msgLabel, CSLib.Framework.CMessage msg)
    {
        Message.CMsgSampleReqSay reqSay = (Message.CMsgSampleReqSay)msg;
        Console.WriteLine("收到：" + reqSay.m_string + "{" + m_netStub.LocalIPEndPoint.ToString() + "<---" + m_netStub.PeerIPEndPoint.ToString() + "}");

        Message.CMsgSampleResSay resSay = new Message.CMsgSampleResSay();
        resSay.m_string = "Sever No.[";
        resSay.m_string += m_number.ToString();
        resSay.m_string += "]";
        resSay.m_bool = reqSay.m_bool;
        resSay.m_byte = reqSay.m_byte;
        resSay.m_bytes = reqSay.m_bytes;
        resSay.m_double = reqSay.m_double;
        resSay.m_float = reqSay.m_float;
        resSay.m_int = reqSay.m_int;
        resSay.m_long = reqSay.m_long;
        resSay.m_short = reqSay.m_short;
        resSay.m_uint = reqSay.m_uint;
        resSay.m_ulong = reqSay.m_ulong;
        resSay.m_ushort = reqSay.m_ushort;
        m_number++;

        CSLib.Utility.CStream streamRes = new CSLib.Utility.CStream();

        resSay.Serialize(streamRes);

        m_netStub.SendAsync(streamRes);
    }

    private void _OnMsgTestNtfOk(CSLib.Framework.CMessageLabel msgLabel, CSLib.Framework.CMessage msg)
    {
        ;
    }

    //
    public bool ParseMsgCallback(byte[] msgBuff, int msgSize)
    {
        CSLib.Framework.CMessageLabel msgLabel = new CSLib.Framework.CMessageLabel();

        if(!m_msgExecute.ExecuteMessage(msgLabel, msgBuff, msgSize)) {
            Console.WriteLine("解析消息失败");
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

    //
    private int m_number = 1;
    private CSLib.Network.CTcpStub m_netStub;

    private CSLib.Framework.CMsgFactory m_msgFactory = new CSLib.Framework.CMsgFactory();
    private CSLib.Framework.CMsgExecFuncFactory m_msgExecFuncFactory = new CSLib.Framework.CMsgExecFuncFactory();
    private CSLib.Framework.CMsgExecute m_msgExecute = new CSLib.Framework.CMsgExecute();
}

class Program
{
    static void Main(string[] args)
    {
        CSLib.Network.CTcpServer netServer = new CSLib.Network.CTcpServer();

        netServer.AddTcpAcceptor("0.0.0.0", 9500, null);

        netServer.CbNewTcpStub = CTcpStubMsg.CbNewTcpStub;

        netServer.Start();

        while (true)
        {
            System.Threading.Thread.Sleep(10);
        }
    }
}
}
