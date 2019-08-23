//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/logicServer/netPeer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_LOGICSERVER_NETPEER_H__
#define __SFLIB_LOGICSERVER_NETPEER_H__

#include <SFLib/commonServer/netPeer.h>
#include <SFLib/commonServer/tcpStub.h>
#include <SFLib/commonServer/netClient/masterClient.h>
#include <SFLib/message/peerMsg.h>

namespace SFLib
{
namespace Logic
{
class CNetPeer : public SFLib::CommonServer::CNetPeer
{
public:
    CNetPeer();
    CNetPeer(PeerID peerID, ServerID gatewayServerID, BCLib::Network::TcpStubID gameClientStubID);
    virtual ~CNetPeer();

    /// @brief 请求离开
    /// @return bool
    /// @param nReason : 1 代表清理玩家，2 代表登录踢人
    virtual bool reqLeave(EPeerLeaveReason nReason);

    /// @brief 请求离开
    /// @return void
    virtual void ackLeave();

    virtual void terminate();

	virtual bool serializeTo(BCLib::Utility::CStream& stream) const;
	virtual bool serializeFrom(BCLib::Utility::CStream& stream);

	/// @brief 发送消息到其他服务器
	/// @return bool
	/// @param serverType 服务器类型
	///        如果使用 enterLogicServer(ServerID serverID) 函数进入的话，则直接发送消息到这个服务器
	///        如果使用 enterLogicServer(EServerType serverType) 函数进入的话，则随机一个服务器进行发送
	virtual bool sendMsgByType(ServerType serverType, const SFLib::Message::CNetMessage* msg);
	virtual bool sendMsgByType(ServerType serverType, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    bool sendMsgToGC(const SFLib::Message::CNetMessage* msg);
	bool sendMsgToGC(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    bool sendMsgToMS(const SFLib::Message::CNetMessage* msg);
	bool sendMsgToMS(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

public:
	ServerID getGatewayServerID() { return m_gatewayServerID; }
    void setGatewayServerID(ServerID gatewayServerID) { m_gatewayServerID = gatewayServerID; }

	BCLib::Network::TcpStubID getGameClientStubID() { return m_gameClientStubID; }
    void setGameClientStubID(BCLib::Network::TcpStubID gameClientStubID);

	SFLib::CommonServer::CTcpStubPtr getGameClientStub() { return m_gameClientStub;	}

    ServerID getServerIDByType(EServerType serverType);
	BCLib::uint32 getPingValue();

public:
    /// @brief 申请进入某个逻辑服务器
    /// @return bool
    /// @param serverID 服务器ID
    bool enterLogicServer(ServerID serverID);

    /// @brief 申请离开某个逻辑服务器
    /// @return bool
	/// @param serverID 服务器ID
    bool leaveLogicServer(ServerID serverID, EPeerLeaveReason nReason);

	/// @brief 申请进入某个外部服务器
	/// @return bool
	/// @param serverID 服务器ID
	bool enterExternalServer(ServerID serverID);

	/// @brief 申请进入某个外部服务器
	/// @return bool
	/// @param serverID 服务器ID
	bool leaveExternalServer(ServerID serverID, EPeerLeaveReason nReason);

	/// @brief 申请进入某类外部服务器
	/// @return bool
	/// @param serverType 服务器类型
	bool enterExternalServer(EServerType serverType);

	/// @brief 申请离开某类外部服务器
	/// @return bool
	/// @param serverType 服务器类型
	bool leaveExternalServer(EServerType serverType, EPeerLeaveReason nReason);

protected:
    virtual void _setLogicServerInfo(EServerType serverType, ServerID serverID);
    virtual void _delLogicServerInfo(EServerType serverType, ServerID serverID);
    BCLib::uint8 _getLogicServerInfo(SFLib::Message::SPeerServerInfo* serverList);

    virtual void _setExternalServerInfo(EServerType serverType, ServerID serverID);
    virtual void _delExternalServerInfo(EServerType serverType, ServerID serverID);
    BCLib::uint8 _getExternalServerInfo(SFLib::Message::SPeerServerInfo* serverList);

private:
    bool _sendMsgToGW(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

private:
    ServerID m_gatewayServerID;
    BCLib::Network::TcpStubID m_gameClientStubID;
    SFLib::CommonServer::CTcpStubPtr m_gameClientStub;

    std::map<EServerType, ServerID> m_logicServerTypeMap;
    BCLib::Utility::CMutex  m_mutexLogicServerTypeMap;

	std::map<EServerType, ServerID> m_externalServerTypeMap;
	BCLib::Utility::CMutex  m_mutexExternalServerTypeMap;

	//
    friend class CMasterClient;
    friend class CLogicStub;
    friend class CLogicClient;
    friend class CLogicServer;
};
typedef BCLib::Utility::CSPointer<CNetPeer> CNetPeerPtr;
}//Logic
}//SFLib

#endif//__SFLIB_LOGICSERVER_NETPEER_H__
