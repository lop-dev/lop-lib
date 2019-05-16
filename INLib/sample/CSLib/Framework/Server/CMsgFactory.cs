using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Server
{
public class CMsgFactory
{
    public static bool MsgCreater(UInt16 type, UInt16 id, ref CSLib.Framework.CMessage msg)
    {
        Message.EMsgType msgType = (Message.EMsgType)type;
        switch(msgType)
        {
            case Message.EMsgType.EMT_SAMPLE:
                {
                    return _SampleMsgCreater(id, ref msg);
                }
        }
        return false;
    }

    private static bool _SampleMsgCreater(UInt16 id, ref CSLib.Framework.CMessage msg)
    {
        Message.EMsgID msgID = (Message.EMsgID)id;
        switch(msgID)
        {
            case Message.EMsgID.EMID_SAMPLE_NTF_OK:
            {
                msg = new Message.CMsgSampleNtfOk();
                return true;
            }
            case Message.EMsgID.EMID_SAMPLE_REQ_SAY:
            {
            msg = new Message.CMsgSampleReqSay();
                return true;
            }
            case Message.EMsgID.EMID_SAMPLE_RES_SAY:
            {
            msg = new Message.CMsgSampleResSay();
                return true;
            }
        }
        return false;
    }
}
}
