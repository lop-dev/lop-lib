//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/netfw/client/client.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/logFile.h>
#include "client.h"
#include "mainThread.h"
#include "message.h"

CClient::CClient()
{
    ;
}

CClient::~CClient()
{
    ;
}

bool CClient::sendMsg(const BCLib::Framework::CMessage& msg)
{
    BCLib::Utility::CStream stream;
    if(!msg.serializeTo(stream))
    {
        return false;
    }
    return BCLib::Network::CTcpClient::send(stream) > 0;
}

bool CClient::sendMsg(const BCLib::Framework::SMessage* msg, const BCLib::uint32 msgSize)
{
    return BCLib::Network::CTcpClient::send(msg, msgSize) > 0;
}

void CClient::_leavePoll()
{
    BCLib::Network::CTcpClient::_leavePoll();

    CMainThread::singleton().setValid(false);
}

bool CClient::_cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize)
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
    case EMID_SAMPLE_RES_SAY:
        {
            CMsgSampleResSay resSay;
            resSay.serializeFrom(msg, msgSize);
            //BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "收到[%s]", resSay.m_strSay.c_str());
            BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "收到[CMsgSampleResSay]");
            return true;
        }
        break;
    case EMID_SAMPLE_RES_SAY_C:
        {
            CMsgSampleResSayC resSay;
            resSay.serializeFrom(msg, msgSize);

            BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "收到[%s]", resSay.m_strSay.c_str());
            return true;
        }
        break;
    }
    BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "无法解析消息type[%d]id[%d]size[%d]", msg->getType(), msg->getID(), msgSize);
    return false;
}