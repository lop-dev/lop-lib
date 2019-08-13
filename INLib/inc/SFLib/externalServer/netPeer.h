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

    /// @brief 请求离开
    /// @return bool
    /// @param notifyMaster 是否通过 Master 转发进行全服处理
    /// @param nReason : 1 代表清理玩家，2 代表登录踢人
    virtual bool reqLeave(bool notifyMaster, EPeerLeaveReason nReason);

    /// @brief 请求离开
    /// @return void
    /// @param notifyMaster 是否通过 Master 转发进行全服处理
    virtual void ackLeave(bool notifyMaster);

    virtual void terminate();

    bool sendMsgToGC(const SFLib::Message::CNetMessage* msg);
    bool sendMsgToMS(const SFLib::Message::CNetMessage* msg);
    virtual bool sendMsgByType(ServerType serverType, const SFLib::Message::CNetMessage* msg);

    bool sendMsgToGC(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);
    bool sendMsgToMS(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);
    virtual bool sendMsgByType(ServerType serverType, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

	//
    void setGatewayServerID(ServerID gatewayServerID) { m_gatewayServerID = gatewayServerID; }
    ServerID getGatewayServerID() { return m_gatewayServerID; }

    void setGameClientStubID(BCLib::Network::TcpStubID gameClientStubID);
    BCLib::Network::TcpStubID getGameClientStubID() { return m_gameClientStubID; }

	SFLib::CommonServer::CTcpStubPtr getGameClientStub() { return m_gameClientStub;	}

    void setOfflineTime(BCLib::Utility::CDateTime dtOffline) { m_dtOffline = dtOffline; }
    BCLib::Utility::CDateTime getOfflineTime() { return m_dtOffline; }

    ServerID getServerIDByType(EServerType serverType);

	BCLib::uint32 getPingValue();

    virtual bool serializeTo(BCLib::Utility::CStream& stream) const;
    virtual bool serializeFrom(BCLib::Utility::CStream& stream);

private:
    bool _sendMsgToGW(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    void _setServerInfo(EServerType serverType, ServerID serverID);
    void _delServerInfo(EServerType serverType, ServerID serverID);
    BCLib::uint8 _getServerInfo(SFLib::Message::SPeerServerInfo* serverList);

private:
    ServerID m_gatewayServerID;
    BCLib::Network::TcpStubID m_gameClientStubID;
    SFLib::CommonServer::CTcpStubPtr m_gameClientStub;

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
