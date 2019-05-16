//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/p2p/p2pServer/p2pServer.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/logFile.h>
#include <BCLib/network/netInit.h>
#include <BCLib/p2p/p2pServer/p2pServer.h>
#include <BCLib/framework/mainThread.h>
#include <BCLib/framework/sysConfig.h>
#include <BCLib/p2p/p2pCommon/p2pCfg.h>
#include "p2pServer.h"
#include "message.h"

BCLIB_FW_MAIN(CP2pServer::main);

BCLIB_SINGLETON_DEFINE(CP2pServer)

CP2pServer::CP2pServer()
{
    //timeBeginPeriod(1);
}

CP2pServer::~CP2pServer()
{
    ;
}

int CP2pServer::main()
{
    BCLib::Utility::CLogFile::singleton().setModuleON(BCLib::ELOGMODULE_BCLIB_NETWORK);
    BCLib::Utility::CLogFile::singleton().setModuleLevel(BCLib::ELOGMODULE_BCLIB_NETWORK, BCLib::Utility::CLog::ELOGLEVEL_DEBUG);
    BCLib::Utility::CLogFile::singleton().setModuleLevel(BCLib::ELOGMODULE_DEFAULT, BCLib::Utility::CLog::ELOGLEVEL_DEBUG);
    BCLib::Utility::CLogFile::singleton().setOutConsole(true);
    BCLib::Utility::CLogFile::singleton().setOutFile("./p2pServer.log");

    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "*************p2sp 测试服务端***********");
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "***************************************");

    BCLib::P2p::CP2pCfg::setP2pVersion(7777777);
    BCLib::P2p::CP2pCfg::setP2pStepKey0(1);
    BCLib::P2p::CP2pCfg::setP2pStepKey1(2);
    BCLib::P2p::CP2pCfg::setP2pStepKey2(3);

    return CP2pServer::singleton()._main();
}

bool CP2pServer::_init()
{
    if(!BCLib::P2p::P2pServer::CP2pServer::init(false, NULL, NULL))
    {
        return false;
    }

    if(!BCLib::P2p::P2pServer::CP2pServer::addUdpAcceptor("0.0.0.0", 9000))
    {
        return false;
    }

    return BCLib::Framework::CMainThread::_init();
}

int CP2pServer::_final()
{
    BCLib::P2p::P2pServer::CP2pServer::final();
    return BCLib::Framework::CMainThread::_final();
}

void CP2pServer::_cbParseMsgByPeerID(BCLib::P2p::PeerID peerID, const void* msg, BCLib::uint32 msgSize)
{
    BCLib::Framework::SMessage* sMsg = (BCLib::Framework::SMessage*)msg;
    if(sMsg->getType() == ETYPE_P2P_SAMPLE)
    {
        _parseSampleMsgPeerID(peerID, sMsg, msgSize);
        return;
    }
    BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "无法处理的消息type[%d] id[%d]", sMsg->getType(), sMsg->getID());
}

void CP2pServer::_cbParseMsgByConnectID(BCLib::P2p::ConnectID connID, const void* msg, BCLib::uint32 msgSize)
{
    BCLib::Framework::SMessage* sMsg = (BCLib::Framework::SMessage*)msg;
    if(sMsg->getType() == ETYPE_P2P_SAMPLE)
    {
        _parseSampleMsgConnID(connID, sMsg, msgSize);
        return;
    }
    BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "无法处理的消息type[%d] id[%d]", sMsg->getType(), sMsg->getID());
}

void CP2pServer::_cbRequestConnectPeer(BCLib::P2p::PeerID srcPeerID, BCLib::P2p::PeerID dstPeerID)
{
    BCLib::P2p::P2pServer::CP2pServer::allowConnectPeer(srcPeerID, dstPeerID);
}

void CP2pServer::_cbConnectionTerminate(BCLib::P2p::ConnectID connID, BCLib::P2p::PeerID peerID)
{

}

void CP2pServer::_parseSampleMsgConnID(BCLib::P2p::ConnectID connID, BCLib::Framework::SMessage* msg, BCLib::uint32 msgSize)
{
    switch(msg->getID())
    {
    case EMID_PC2PS_REQ_LOGIN:
        {
            SMsgPc2PsReqLogin* loginMsgReq = (SMsgPc2PsReqLogin*)msg;
            BCLib::P2p::PeerID peerID = m_allocPeerID.getID();
            BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "玩家peerID[%ld] user[%s] password[%s]", peerID, loginMsgReq->m_name, loginMsgReq->m_pswd);
            BCLib::P2p::P2pServer::CP2pServer::setRegisterPeer(connID, peerID);
            break;
        }
    default:
        {
            BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "无法处理的消息type[%d] id[%d]", msg->getType(), msg->getID());
        }
    }
}

void CP2pServer::_parseSampleMsgPeerID(BCLib::P2p::PeerID peerID, BCLib::Framework::SMessage* msg, BCLib::uint32 msgSize)
{
    switch(msg->getID())
    {
    case EMID_PC2PS_NTF_SAY:
        {
            SMsgPc2PsNtfSay* sayMsg = (SMsgPc2PsNtfSay*)msg;
            BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "peer[%d] say[%s]", peerID, sayMsg->m_text);
            break;
        }
    default:
        {
            BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "无法处理的消息type[%d] id[%d]", msg->getType(), msg->getID());
        }
    }
}

