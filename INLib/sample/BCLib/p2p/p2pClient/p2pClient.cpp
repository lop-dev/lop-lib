//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/p2p/p2pClient/p2pClient.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <time.h>
#include <BCLib/utility/logFile.h>
#include <BCLib/utility/string.h>
#include <BCLib/framework/mainThread.h>
#include <BCLib/framework/sysConfig.h>
#include <BCLib/p2p/p2pCommon/p2pCfg.h>

#include "p2pClient.h"
#include "message.h"

BCLIB_FW_MAIN(CP2pClient::main);

BCLIB_SINGLETON_DEFINE(CP2pClient)

CP2pClient::CP2pClient()
:m_peerID(0)
{
    //timeBeginPeriod(1);
    m_p2pClientCommand = new CP2pCommand(this);
}

CP2pClient::~CP2pClient()
{
    ;
}

BCLib::ResThread CP2pClient::inputCommand(void* param)
{
    char commandBuff[1024];
    int res = 0;
    while(true)
    {
        printf("\n>");
        gets_s(commandBuff, sizeof(commandBuff));
        if(m_p2pClientCommand != NULL)
        {
            res = m_p2pClientCommand->parseCommand(commandBuff, NULL);
        }
        if(res == 0)
        {
            continue;
        }
        if(res == 1)
        {
            break;
        }
    }
    return BCLIB_THREAD_RES(0);
}

BCLib::int64 CP2pClient::getNowTime()
{
    m_realTime.now();
    return m_realTime.getMilliseconds();
}

int CP2pClient::main()
{
    BCLib::Utility::CLogFile::singleton().setModuleON(BCLib::ELOGMODULE_BCLIB_NETWORK);
    BCLib::Utility::CLogFile::singleton().setModuleLevel(BCLib::ELOGMODULE_BCLIB_NETWORK, BCLib::Utility::CLog::ELOGLEVEL_DEBUG);
    BCLib::Utility::CLogFile::singleton().setModuleLevel(BCLib::ELOGMODULE_DEFAULT, BCLib::Utility::CLog::ELOGLEVEL_DEBUG);
    BCLib::Utility::CLogFile::singleton().setOutConsole(true);
    BCLib::Utility::CLogFile::singleton().setOutFile("./p2pClient.log");

    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "*************p2sp 测试客户端***********");
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "*quit 退出程序");
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "***************************************");

    BCLib::P2p::CP2pCfg::setP2pVersion(7777777);
    BCLib::P2p::CP2pCfg::setP2pStepKey0(1);
    BCLib::P2p::CP2pCfg::setP2pStepKey1(2);
    BCLib::P2p::CP2pCfg::setP2pStepKey2(3);

    return CP2pClient::singleton()._main();
}

bool CP2pClient::_init()
{
    m_p2pClientCommand->init();
    m_inputThread = BCLib::Utility::CHndThread::createThread(&CP2pClient::inputCommand, this);

    if(!CP2pClient::init(NULL, NULL))
    {
        return false;
    }

    return BCLib::Framework::CMainThread::_init();
}

int CP2pClient::_final()
{
    BCLib::P2p::P2pClient::CP2pClient::closeServer();
    return BCLib::Framework::CMainThread::_final();
}

void CP2pClient::_cbParseMsgFromServer(const void* msg, BCLib::uint32 msgSize)
{
    BCLib::Framework::SMessage* sMsg = (BCLib::Framework::SMessage*)msg;
    if(sMsg->getType() == ETYPE_P2P_SAMPLE)
    {
        _parseSampleMsgFromServer(sMsg, msgSize);
        return;
    }
    BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "无法处理的消息type[%d] id[%d]", sMsg->getType(), sMsg->getID());
}

void CP2pClient::_cbParseMsgFromPeer(BCLib::P2p::PeerID peerID, const void* msg, BCLib::uint32 msgSize)
{
    BCLib::Framework::SMessage* sMsg = (BCLib::Framework::SMessage*)msg;
    if(sMsg->getType() == ETYPE_P2P_SAMPLE)
    {
        _parseSampleMsgFromPeer(peerID, sMsg, msgSize);
        return;
    }
    BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "无法处理的消息peer[%d] type[%d] id[%d]", peerID, sMsg->getType(), sMsg->getID());
}

void CP2pClient::_parseSampleMsgFromServer(BCLib::Framework::SMessage* msg, BCLib::uint32 msgSize)
{
    switch(msg->getID())
    {
    default:
        {
            BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "无法处理的消息type[%d] id[%d]", msg->getType(), msg->getID());
        }
    }
}

void CP2pClient::_parseSampleMsgFromPeer(BCLib::P2p::PeerID peerID, BCLib::Framework::SMessage* msg, BCLib::uint32 msgSize)
{
    switch(msg->getID())
    {
    case EMID_PC2PC_NTF_SAY:
        {
            SMsgPc2PcNtfSay* sayMsg = (SMsgPc2PcNtfSay*)msg;
            BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "PeerID[%d] say [%s]", peerID, sayMsg->m_text);
            SMsgPc2PcNtfSayBk sayBk;
            sayBk.m_time = sayMsg->m_time;
            sendMsgToPeer(peerID, &sayBk, sizeof(sayBk));
        }
        break;
    case EMID_PC2PC_NTF_SAY_BK:
        {
            SMsgPc2PcNtfSayBk* sayBkMsg = (SMsgPc2PcNtfSayBk*)msg;
            BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "PeerID[%d] rtt[%I64d]", peerID, getNowTime() - sayBkMsg->m_time);
        }
        break;
    default:
        {
            BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "无法处理的消息type[%d] id[%d]", msg->getType(), msg->getID());
        }
    }
}

