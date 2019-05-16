//////////////////////////////////////////////////////////////////////
//  created:    2012/08/01
//  filename:   BCLib/framework/netfw/server/netStub.cpp
//  author:     aaron
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/logFile.h>
#include "netServer.h"
#include "netStub.h"
#include "message.h"

CNetStub::CNetStub(const BCLib::Network::CNetConnectionSPtr& netConn)
:BCLib::Network::CNetStub(netConn)
{
}

CNetStub::~CNetStub()
{
    ;
}

bool CNetStub::sendMsg(const BCLib::Framework::CMessage& msg)
{
    BCLib::Utility::CStream stream;
    if(!msg.serializeTo(stream))
    {
        return false;
    }
    return BCLib::Network::CNetStub::send(stream) > 0;
}

bool CNetStub::sendMsg(const BCLib::Framework::SMessage* msg, const BCLib::uint32 msgSize)
{
    return BCLib::Network::CNetStub::send(msg, msgSize) > 0;
}

bool CNetStub::_cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize)
{
    BCLib::Framework::SMessage* msg = (BCLib::Framework::SMessage*)msgBuff;
    BCLib::uint16 type = msg->getType();

    if(type != EMT_SAMPLE)
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "无法解析消息type[%d]id[%d]size[%d]", msg->getType(), msg->getID(), msgSize);
        return false;
    }
    switch(msg->getID())
    {
    case EMID_SAMPLE_NTF_OK:
        {
            BCLib::Network::CNetStub::setState(BCLib::Network::ESS_OKAY);
            return true;
        }
        break;
    case EMID_SAMPLE_REQ_SAY:
        {
            CMsgSampleReqSay reqSay;
            reqSay.serializeFrom(msg, msgSize);
            //BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "收到[%d  %d  %d  %d  %d  %f  %f  %d  %d  %d  %u  %u  %u]", reqSay.m_bool,reqSay.m_byte,reqSay.m_bytes[0],reqSay.m_bytes[1],reqSay.len,reqSay.m_double,reqSay.m_float,reqSay.m_int,reqSay.m_long,reqSay.m_short,reqSay.m_uint,reqSay.m_ulong,reqSay.m_ushort);

            CMsgSampleResSay resSay;
//             resSay.m_bool = reqSay.m_bool;
//             resSay.m_byte = reqSay.m_byte;
//             resSay.len = reqSay.len;
//             memcpy(resSay.m_bytes,reqSay.m_bytes,reqSay.len);
//             resSay.m_double = reqSay.m_double;
//             resSay.m_float = reqSay.m_float;
//             resSay.m_int = reqSay.m_int;
//             resSay.m_long = reqSay.m_long;
//             resSay.m_short = reqSay.m_short;
            resSay.m_string = reqSay.m_string;
//             resSay.m_uint = reqSay.m_uint;
//             resSay.m_ulong = reqSay.m_ulong;
//             resSay.m_ushort = reqSay.m_ushort;
            if(!sendMsg(resSay))
            {
                BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "发送消息失败");
            }
            return true;
        }
        break;
    case EMID_SAMPLE_REQ_SAY_C:
        {
            CMsgSampleReqSayC reqSay;
            reqSay.serializeFrom(msg, msgSize);

            BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "收到[%s]", reqSay.m_strSay.c_str());

            CMsgSampleResSayC resSay;
            resSay.m_strSay = reqSay.m_strSay;

            if(!sendMsg(resSay))
            {
                BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "发送消息失败");
            }

            return true;
        }
        break;
    }
    BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "无法解析消息type[%d]id[%d]size[%d]", msg->getType(), msg->getID(), msgSize);
    return false;
}

void CNetStub::_cbTerminate()
{
    ;
}
