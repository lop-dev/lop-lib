//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/masterServer/netPeer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_MASTERSERVER_NETPEER_H__
#define __SFLIB_MASTERSERVER_NETPEER_H__

#include <SFLib/commonServer/netPeer.h>
#include <SFLib/commonServer/tcpStub.h>
#include <SFLib/message/peerMsg.h>

namespace SFLib
{
namespace Master
{
class CPeerEnterScene;
class CPeerLeaveScene;
class CPeerCreate;
class CMasterServer;
class CMasterStub;

class CNetPeer : public SFLib::CommonServer::CNetPeer
{
public:
    CNetPeer(PeerID peerID, ServerID gatewayServerID, BCLib::Network::TcpStubID gameClientStubID);
    virtual ~CNetPeer();

    virtual bool reqLeave();
    virtual void terminate();

    bool sendMsgToGC(const SFLib::Message::CNetMessage* msg);
    virtual bool sendMsgByType(ServerType serverType, const SFLib::Message::CNetMessage* msg);

    bool sendMsgToGC(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);
    virtual bool sendMsgByType(ServerType serverType, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

	//
    ServerID getGatewayServerID() { return m_gatewayServerID; }
    ServerID getGameClientStubID() { return m_gameClientStubID; }

    BCLib::uint8 getActiveServerID(std::vector<SFLib::ServerID>& serverList);

	ServerID getServerIDByType(EServerType serverType);
    bool isInServer(ServerID serverID);

private:
	bool _sendMsgToGW(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    void _setServerInfo(EServerType serverType, ServerID serverID);
    void _delServerInfo(EServerType serverType, ServerID serverID);
    BCLib::uint8 _getServerInfo(SFLib::Message::SPeerServerInfo* serverList);

private:
    ServerID m_gatewayServerID;
    BCLib::Network::TcpStubID m_gameClientStubID;
    CPeerCreate* m_peerCreate;

    CPeerEnterScene* m_peerEnterServer;
    CPeerLeaveScene* m_peerLeaveServer;

    std::map<EServerType, ServerID> m_serverTypeMap;
    BCLib::Utility::CMutex  m_mutexServerType;

	//
    friend class CMasterServer;
    friend class CMasterStub;
    friend class CNetPeerMgr;
};
typedef BCLib::Utility::CSPointer<CNetPeer> CNetPeerPtr;

}//Master
}//SFLib

#endif//__SFLIB_MASTERSERVER_NETPEER_H__
