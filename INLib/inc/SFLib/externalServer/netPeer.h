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

    bool sendMsgToGC(const SFLib::Message::CNetMessage* msg);
	bool sendMsgToCT(const SFLib::Message::CNetMessage* msg);

    bool sendMsgToGC(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);
	bool sendMsgToCT(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

	//
    void setGatewayServerID(ServerID gatewayServerID) { m_gatewayServerID = gatewayServerID; }
    ServerID getGatewayServerID() { return m_gatewayServerID; }

	void setGameClientStubID(BCLib::Network::TcpStubID gameClientStubID);
	BCLib::Network::TcpStubID getGameClientStubID() { return m_gameClientStubID; }

	void setExternalStub(SFLib::CommonServer::CTcpStubPtr externalStub) { m_externalStub = externalStub; }
	SFLib::CommonServer::CTcpStubPtr getExternalStub() { return m_externalStub; }

    void setOfflineTime(BCLib::Utility::CDateTime dtOffline) { m_dtOffline = dtOffline; }
    BCLib::Utility::CDateTime getOfflineTime() { return m_dtOffline; }

	BCLib::uint32 getPingValue();

    virtual bool serializeTo(BCLib::Utility::CStream& stream) const;
    virtual bool serializeFrom(BCLib::Utility::CStream& stream);

private:
    void _setServerInfo(EServerType serverType, ServerID serverID);
    void _delServerInfo(EServerType serverType, ServerID serverID);
    BCLib::uint8 _getServerInfo(SFLib::Message::SPeerServerInfo* serverList);

private:
    ServerID m_gatewayServerID;
    BCLib::Network::TcpStubID m_gameClientStubID;
    SFLib::CommonServer::CTcpStubPtr m_gameClientStub;
	SFLib::CommonServer::CTcpStubPtr m_externalStub;

    BCLib::Utility::CDateTime m_dtOffline;

    std::map<EServerType, ServerID> m_logicServerTypeMap;
    BCLib::Utility::CMutex  m_mutexLogicServerTypeMap;

	//
    friend class CExternalStub;
};
typedef BCLib::Utility::CSPointer<CNetPeer> CNetPeerPtr;
}//External
}//SFLib

#endif//__SFLIB_EXTERNALSERVER_NETPEER_H__
