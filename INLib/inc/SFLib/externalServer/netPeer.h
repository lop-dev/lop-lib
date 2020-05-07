//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/externalServer/netPeer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_EXTERNALSERVER_NETPEER_H__
#define __SFLIB_EXTERNALSERVER_NETPEER_H__

#include <SFLib/commonServer/netPeer.h>
#include <SFLib/commonServer/tcpStub.h>
#include <SFLib/commonServer/netClient/masterClient.h>
#include <SFLib/message/peerMsg.h>

namespace SFLib
{
namespace External
{
class CNetPeer : public SFLib::CommonServer::CNetPeer
{
public:
    CNetPeer();
    CNetPeer(PeerID peerID, ServerID gatewayServerID, BCLib::Network::TcpStubID gameClientStubID);
    virtual ~CNetPeer();

    virtual void terminate();

	virtual bool serializeTo(BCLib::Utility::CStream& stream) const;
	virtual bool serializeFrom(BCLib::Utility::CStream& stream);

public:
    virtual ServerID getServerID(EServerType serverType);
    virtual EServerType getServerType(ServerID serverID);

	/// @brief 发送消息到其他服务器
	/// @return bool
	/// @param serverType 服务器类型，只会发送给内部服务器
	virtual bool sendMsgByType(ServerType serverType, const SFLib::Message::CNetMessage* msg);
	virtual bool sendMsgByType(ServerType serverType, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

	virtual bool sendMsgToGC(const SFLib::Message::CNetMessage* msg);
	virtual bool sendMsgToGC(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

	virtual bool sendMsgToGW(const SFLib::Message::CNetMessage* msg);
	virtual bool sendMsgToGW(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

public:
	virtual BCLib::uint32 getLogicServerNum();

public:
	ServerID getGatewayServerID() { return m_gatewayServerID; }
	void setGatewayServerID(ServerID gatewayServerID) { m_gatewayServerID = gatewayServerID; }

    BCLib::Network::TcpStubID getGameClientStubID() { return m_gameClientStubID; }
	void setGameClientStubID(BCLib::Network::TcpStubID gameClientStubID) { m_gameClientStubID = gameClientStubID; }

	ServerID getServerIDByType(EServerType serverType);

private:
	bool _sendMsgToStub(SFLib::CommonServer::CTcpStubPtr pStub, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    void _setServerInfo(EServerType serverType, ServerID serverID);
    void _delServerInfo(EServerType serverType, ServerID serverID);
    BCLib::uint16 _getServerInfo(SFLib::Message::SPeerServerInfo* serverList);

private:
    ServerID m_gatewayServerID;
    BCLib::Network::TcpStubID m_gameClientStubID;
	SFLib::CommonServer::CTcpStubPtr m_defaultStub;

    std::map<EServerType, ServerID> m_logicServerTypeMap;
    BCLib::Utility::CMutex  m_mutexLogicServerTypeMap;

	//
    friend class CExternalStub;
	friend class CNetPeerMgr;
};
typedef BCLib::Utility::CSPointer<CNetPeer> CNetPeerPtr;
}//External
}//SFLib

#endif//__SFLIB_EXTERNALSERVER_NETPEER_H__
