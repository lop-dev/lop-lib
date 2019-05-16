//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/message/peerMsg.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_MESSAGE_PEERMSG_H__
#define __SFLIB_MESSAGE_PEERMSG_H__

#include <SFLib/message/gameFrame.h>
#include <BCLib/network/baseDef.h>

namespace SFLib
{

namespace Message
{

#ifdef WIN32
#pragma pack (push, 1)
#else
#pragma pack (1)
#endif

//////////////////////////////////////////////////////////////////////////
// Peer登录流程消息（通过 Master 中转、但是只允许在网关上发起请求）
//////////////////////////////////////////////////////////////////////////
SFLIB_MSGDEBUG(ESERVER_MASTER, EFUNC_GAMEFRAME, EMID_XS2MS_REQ_APPLY_FOR_PEERID);
struct SMsgXS2MSReqApplyForPeerID : public SNetMessage
{
public:
    ServerID m_gatewayServerID;
    BCLib::Network::TcpStubID m_gameClientStubID;  //MS把发起请求的Server当做Gateway
    ServerType m_serverType;
    PeerID m_peerID;
    
    SMsgXS2MSReqApplyForPeerID()
        :SNetMessage(ESERVER_MASTER, EFUNC_GAMEFRAME, EMID_XS2MS_REQ_APPLY_FOR_PEERID)
    {
        m_gatewayServerID = INVALID_SERVER_ID;
        m_gameClientStubID = 0;
        m_serverType = (BCLib::uint8)ESERVER_UNKNOW;
        m_peerID = INVALID_PEER_ID;
    }
};

SFLIB_MSGDEBUG(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_MS2XS_RES_APPLY_FOR_PEERID);
struct SMsgMS2XSResApplyForPeerID : public SNetMessage
{
    enum EError { EERROR_UNKNOWN, EERROR_OK, EERROR_HAVE_EXIST, EERROR_FAIL_ASSIGN, EERROR_CREATE_PEER, EERROR_CREATE_PEERCREATE, };

public:
    ServerID m_gatewayServerID; // 作为延时申请的临时变量
    BCLib::Network::TcpStubID m_gameClientStubID;
	ServerType m_serverType; // 作为延时申请的临时变量
    PeerID m_peerID;
    BCLib::uint8 m_errorID;
    BCLib::uint32 m_uRandKey;

    SMsgMS2XSResApplyForPeerID()
        :SNetMessage(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_MS2XS_RES_APPLY_FOR_PEERID)
    {
        m_gatewayServerID = 0;
        m_gameClientStubID = 0;
		m_serverType = 0;
        m_peerID = INVALID_PEER_ID;
        m_errorID = EERROR_UNKNOWN;
        m_uRandKey = 0;
    }
};

SFLIB_MSGDEBUG(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_MS2XS_REQ_CREATE_PEERID);
//通知 Center 或 Dababase 创建Peer
struct SMsgMS2XSReqCreatePeerID : public SNetMessage
{
public:
    ServerID m_gatewayServerID;
    BCLib::uint32 m_gameClientStubID;
    PeerID m_peerID;

    SMsgMS2XSReqCreatePeerID() : SNetMessage(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_MS2XS_REQ_CREATE_PEERID)
    {
        m_gatewayServerID = 0;
        m_gameClientStubID = 0;
        m_peerID = 0;
    }
};

SFLIB_MSGDEBUG(ESERVER_MASTER, EFUNC_GAMEFRAME, EMID_XS2MS_RES_CREATE_PEERID);
struct SMsgXS2MSResCreatePeerID : public SNetMessage
{
public:
    ServerID m_serverID;
    ServerType m_serverType;
    PeerID m_peerID;
    BCLib::int32 m_errorID;

    SMsgXS2MSResCreatePeerID() : SNetMessage(ESERVER_MASTER, EFUNC_GAMEFRAME, EMID_XS2MS_RES_CREATE_PEERID)
    {
        m_serverID = INVALID_SERVER_ID;
        m_serverType = (BCLib::uint8)ESERVER_UNKNOW;
        m_peerID = 0;
        m_errorID = 0;
    }
};

struct SPeerServerInfo
{
    ServerType m_serverType;
    ServerID m_activeServerID;
};

SFLIB_MSGDEBUG(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_MS2XS_NTF_INIT_PEERID);
struct SMsgMS2XSNtfInitPeerID : public SNetMessage
{
public:
    PeerID m_peerID;
    BCLib::uint8 m_count;
    SPeerServerInfo m_peerServerInfo[1];

    SMsgMS2XSNtfInitPeerID() : SNetMessage(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_MS2XS_NTF_INIT_PEERID)
    {
        m_peerID = 0;
        m_count = 0;
    }

    int getSize() { return sizeof(SMsgMS2XSNtfInitPeerID) - sizeof(SPeerServerInfo) + m_count * sizeof(SPeerServerInfo); }
    int getSize(int count) { return sizeof(SMsgMS2XSNtfInitPeerID) - sizeof(SPeerServerInfo) + count * sizeof(SPeerServerInfo); }
};

SFLIB_MSGDEBUG(ESERVER_GAMECLIENT, EFUNC_GAMEFRAME, EMID_XS2GC_NTF_SELF_PEER_ID);
struct SMsgXS2GCNtfSelfPeerID : public SNetMessage
{
public:
    PeerID m_peerID;

    SMsgXS2GCNtfSelfPeerID() : SNetMessage(ESERVER_GAMECLIENT, EFUNC_GAMEFRAME, EMID_XS2GC_NTF_SELF_PEER_ID)
    {
        m_peerID = 0;
    }
};

//////////////////////////////////////////////////////////////////////////
// Peer进入逻辑服的流程消息（通过 Master 中转）
//////////////////////////////////////////////////////////////////////////
SFLIB_MSGDEBUG(ESERVER_MASTER, EFUNC_GAMEFRAME, EMID_XS2MS_REQ_ENTER_SERVER);
struct SMsgXS2MSReqEnterServer : public SNetMessage
{
public:
    PeerID m_peerID;
    ServerID m_serverID;

    SMsgXS2MSReqEnterServer() : SNetMessage(ESERVER_MASTER, EFUNC_GAMEFRAME, EMID_XS2MS_REQ_ENTER_SERVER)
    {
        m_peerID = INVALID_PEER_ID;
        m_serverID = INVALID_SERVER_ID;
    }
};

SFLIB_MSGDEBUG(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_MS2XS_RES_ENTER_SERVER);
struct SMsgMS2XSResEnterServer : public SNetMessage
{
public:
    EErrorType m_errorID;
    PeerID m_peerID;
    ServerID m_serverID;
    ServerType m_serverType;

    SMsgMS2XSResEnterServer() : SNetMessage(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_MS2XS_RES_ENTER_SERVER)
    {
        m_errorID = EERROR_OK;
        m_peerID = INVALID_PEER_ID;
        m_serverID = INVALID_SERVER_ID;
        m_serverType = (BCLib::uint8)ESERVER_UNKNOW;
    }
};

SFLIB_MSGDEBUG(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_MS2XS_REQ_ENTER_SERVER_CREATE_PEERID);
struct SMsgMS2XSReqEnterServerCreatePeerID : public SNetMessage
{
public:
    PeerID m_peerID;
    ServerID m_gatewayServerID;
    BCLib::Network::TcpStubID m_gameClientStubID;

    SMsgMS2XSReqEnterServerCreatePeerID() : SNetMessage(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_MS2XS_REQ_ENTER_SERVER_CREATE_PEERID)
    {
        m_peerID = INVALID_PEER_ID;
        m_gatewayServerID = INVALID_SERVER_ID;
        m_gameClientStubID = BCLib::Network::INVALID_TCPSTUBID;
    }
};

SFLIB_MSGDEBUG(ESERVER_MASTER, EFUNC_GAMEFRAME, EMID_XS2MS_RES_ENTER_SERVER_CREATE_PEERID);
struct SMsgXS2MSResEnterServerCreatePeerID : public SNetMessage
{
public:
    EErrorType m_errorID;
    PeerID m_peerID;
    ServerID m_serverID;
    ServerType m_serverType;

    SMsgXS2MSResEnterServerCreatePeerID() : SNetMessage(ESERVER_MASTER, EFUNC_GAMEFRAME, EMID_XS2MS_RES_ENTER_SERVER_CREATE_PEERID)
    {
        m_errorID = EERROR_OK;
        m_peerID = INVALID_PEER_ID;
        m_serverID = INVALID_SERVER_ID;
        m_serverType = (BCLib::uint8)ESERVER_UNKNOW;
    }
};

//////////////////////////////////////////////////////////////////////////
// Peer离开逻辑服的流程消息（通过 Master 中转）
//////////////////////////////////////////////////////////////////////////

SFLIB_MSGDEBUG(ESERVER_MASTER, EFUNC_GAMEFRAME, EMID_XS2MS_REQ_LEAVE_SERVER);
struct SMsgXS2MSReqLeaveServer : public SNetMessage
{
public:
    PeerID m_peerID;
    ServerID m_serverID;

    SMsgXS2MSReqLeaveServer() : SNetMessage(ESERVER_MASTER, EFUNC_GAMEFRAME, EMID_XS2MS_REQ_LEAVE_SERVER)
    {
        m_peerID = INVALID_PEER_ID;
        m_serverID = INVALID_SERVER_ID;
    }
};

SFLIB_MSGDEBUG(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_MS2XS_RES_LEAVE_SERVER);
struct SMsgMS2XSResLeaveServer : public SNetMessage
{
public:
    EErrorType m_errorID;
    PeerID m_peerID;
    ServerID m_serverID;
    ServerType m_serverType;

    SMsgMS2XSResLeaveServer() : SNetMessage(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_MS2XS_RES_LEAVE_SERVER)
    {
        m_errorID = EERROR_OK;
        m_peerID = INVALID_PEER_ID;
        m_serverID = INVALID_SERVER_ID;
        m_serverType = (BCLib::uint8)ESERVER_UNKNOW;
    }
};

SFLIB_MSGDEBUG(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_MS2XS_REQ_LEAVE_SERVER_REMOVE_PEERID);
struct SMsgMS2XSReqLeaveServerRemovePeerID : public SNetMessage
{
public:
    PeerID m_peerID;
    ServerID m_gatewayServerID;

    SMsgMS2XSReqLeaveServerRemovePeerID() : SNetMessage(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_MS2XS_REQ_LEAVE_SERVER_REMOVE_PEERID)
    {
        m_peerID = INVALID_PEER_ID;
        m_gatewayServerID = INVALID_SERVER_ID;
    }
};

SFLIB_MSGDEBUG(ESERVER_MASTER, EFUNC_GAMEFRAME, EMID_XS2MS_RES_LEAVE_SERVER_REMOVE_PEERID);
struct SMsgXS2MSResLeaveServerRemovePeerID : public SNetMessage
{
public:
    EErrorType m_errorID;
    PeerID m_peerID;
    ServerID m_serverID;
    ServerType m_serverType;

    SMsgXS2MSResLeaveServerRemovePeerID() : SNetMessage(ESERVER_MASTER, EFUNC_GAMEFRAME, EMID_XS2MS_RES_LEAVE_SERVER_REMOVE_PEERID)
    {
        m_errorID = EERROR_OK;
        m_peerID = INVALID_PEER_ID;
        m_serverID = INVALID_SERVER_ID;
        m_serverType = (BCLib::uint8)ESERVER_UNKNOW;
    }
};

//////////////////////////////////////////////////////////////////////////
// Peer退出流程消息（通过 Master 中转）
//////////////////////////////////////////////////////////////////////////
SFLIB_MSGDEBUG(ESERVER_MASTER, EFUNC_GAMEFRAME, EMID_XS2MS_REQ_LEAVE_PEERID);
struct SMsgXS2MSReqLeavePeerID : public SNetMessage
{
public:
    PeerID m_peerID;

    SMsgXS2MSReqLeavePeerID() : SNetMessage(ESERVER_MASTER, EFUNC_GAMEFRAME, EMID_XS2MS_REQ_LEAVE_PEERID)
    {
        m_peerID = 0;
    }
};


SFLIB_MSGDEBUG(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_MS2XS_REQ_LEAVE_PEERID);
struct SMsgMS2XSReqLeavePeerID : public SNetMessage
{
public:
    PeerID m_peerID;

    SMsgMS2XSReqLeavePeerID() : SNetMessage(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_MS2XS_REQ_LEAVE_PEERID)
    {
        m_peerID = 0;
    }
};

SFLIB_MSGDEBUG(ESERVER_MASTER, EFUNC_GAMEFRAME, EMID_XS2MS_ACK_LEAVE_PEERID);
struct SMsgXS2MSAckLeavePeerID : public SNetMessage
{
public:
    PeerID m_peerID;
    ServerType m_serverType;
    ServerID m_serverID;

    SMsgXS2MSAckLeavePeerID() : SNetMessage(ESERVER_MASTER, EFUNC_GAMEFRAME, EMID_XS2MS_ACK_LEAVE_PEERID)
    {
        m_peerID = 0;
        m_serverType = (BCLib::uint8)ESERVER_UNKNOW;
        m_serverID = INVALID_SERVER_ID;
    }
};

SFLIB_MSGDEBUG(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_MS2XS_NTF_REMOVE_PEERID);
struct SMsgMS2XSNtfRemovePeerID : public SNetMessage
{
public:
    PeerID m_peerID;

    SMsgMS2XSNtfRemovePeerID() : SNetMessage(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_MS2XS_NTF_REMOVE_PEERID)
    {
        m_peerID = 0;
    }
};

//////////////////////////////////////////////////////////////////////////
// Peer 进入/离开 逻辑服的流程消息（通过 直连 方式）
//////////////////////////////////////////////////////////////////////////
SFLIB_MSGDEBUG(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_XS2XS_REQ_ENTER_SERVER);
struct SMsgXS2XSReqEnterServer : public SNetMessage
{
public:
    PeerID m_peerID;
    ServerID m_serverID;
    ServerID m_gatewayServerID;
    BCLib::Network::TcpStubID m_gameClientStubID;

    SMsgXS2XSReqEnterServer() : SNetMessage(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_XS2XS_REQ_ENTER_SERVER)
    {
        m_peerID = INVALID_PEER_ID;
        m_serverID = INVALID_SERVER_ID;
        m_gatewayServerID = INVALID_SERVER_ID;
        m_gameClientStubID = BCLib::Network::INVALID_TCPSTUBID;
    }
};

SFLIB_MSGDEBUG(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_XS2XS_RES_ENTER_SERVER);
struct SMsgXS2XSResEnterServer : public SNetMessage
{
public:
    EErrorType m_errorID;
    PeerID m_peerID;
    ServerID m_serverID;
    ServerType m_serverType;

    SMsgXS2XSResEnterServer() : SNetMessage(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_XS2XS_RES_ENTER_SERVER)
    {
        m_errorID = EERROR_OK;
        m_peerID = INVALID_PEER_ID;
        m_serverID = INVALID_SERVER_ID;
        m_serverType = (BCLib::uint8)ESERVER_UNKNOW;
    }
};

SFLIB_MSGDEBUG(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_XS2XS_SET_ENTER_SERVER);
struct SMsgXS2XSSetEnterServer : public SNetMessage
{
public:
    PeerID m_peerID;
    BCLib::uint8 m_count;
    SPeerServerInfo m_serverInfo[1];

    SMsgXS2XSSetEnterServer() : SNetMessage(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_XS2XS_SET_ENTER_SERVER)
    {
        m_peerID = 0;
        m_count = 0;
    }

    int getSize() { return sizeof(SMsgMS2XSNtfInitPeerID) - sizeof(SPeerServerInfo) + m_count * sizeof(SPeerServerInfo); }
    int getSize(int count) { return sizeof(SMsgMS2XSNtfInitPeerID) - sizeof(SPeerServerInfo) + count * sizeof(SPeerServerInfo); }
};

SFLIB_MSGDEBUG(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_XS2XS_REQ_LEAVE_SERVER);
struct SMsgXS2XSReqLeaveServer : public SNetMessage
{
public:
    PeerID m_peerID;
    ServerID m_serverID;

    SMsgXS2XSReqLeaveServer() : SNetMessage(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_XS2XS_REQ_LEAVE_SERVER)
    {
        m_peerID = INVALID_PEER_ID;
        m_serverID = INVALID_SERVER_ID;
    }
};

SFLIB_MSGDEBUG(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_XS2XS_RES_LEAVE_SERVER);
struct SMsgXS2XSResLeaveServer : public SNetMessage
{
public:
    EErrorType m_errorID;
    PeerID m_peerID;
    ServerID m_serverID;
    ServerType m_serverType;

    SMsgXS2XSResLeaveServer() : SNetMessage(ESERVER_ANYXS, EFUNC_GAMEFRAME, EMID_XS2XS_RES_LEAVE_SERVER)
    {
        m_errorID = EERROR_OK;
        m_peerID = INVALID_PEER_ID;
        m_serverID = INVALID_SERVER_ID;
        m_serverType = (BCLib::uint8)ESERVER_UNKNOW;
    }
};

#ifdef WIN32
#pragma pack (pop)
#else
#pragma pack ()
#endif

}//Message

}//SFLib

#endif//__SFLIB_MESSAGE_PEERMSG_H__
