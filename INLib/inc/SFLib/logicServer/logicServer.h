//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/logicServer/logicServer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_LOGICSERVER_LOGICSERVER_H__
#define __SFLIB_LOGICSERVER_LOGICSERVER_H__

#include <SFLib/commonServer/commonServer2Master.h>
#include <BCLib/network/udp/udpServer.h>
#include <SFLib/logicServer/netPeer.h>

namespace SFLib
{
namespace Logic
{
class SFLIB_LOGIC_API CLogicServer : public SFLib::CommonServer::CCommonServer2Master
{
public:
    CLogicServer(const std::string& name);
    virtual ~CLogicServer();

    /// @brief 申请PeerID
    /// @return bool
    /// @param peerID 申请指定的PeerID，如果为0，则随机分配
    /// @param gatewayServerID 该玩家所在网关的ServerID
    /// @param gameClientStubID 该玩家所在网关上，对应的客户端StubID
    bool requestPeerID(PeerID peerID, ServerID gatewayServerID, BCLib::Network::TcpStubID gameClientStubID);

    /// @brief requestPeerID请求的回调函数
    /// @return void
    /// @param peerID 申请到得PeerID
    virtual void cbRespondPeerID(PeerID peerID, ServerID gatewayServerID, BCLib::Network::TcpStubID gameClientStubID) {}

protected:
    virtual bool _init();

    virtual BCLib::Network::CTcpStubSPtr _cbNewStub(const BCLib::Network::CTcpConnectionSPtr& connPtr);

    virtual SFLib::CommonServer::CTcpClientPtr _cbNewMasterClient();
    virtual SFLib::CommonServer::CCommonClientPtr _cbNewLogicClient(SFLib::Message::SServerInfo& serverInfo);
    virtual CNetPeerPtr _cbNewPeer(PeerID peerID, ServerID gatewayServerID, BCLib::Network::TcpStubID gameClientStubID);

    /// @brief 如果要做P2P功能，可以开启UDP服务
    /// @return bool
    bool _initUdpServer();
    virtual BCLib::Network::CUdpProxy* _cbNewUdpProxy();

private:
    BCLib::Network::CUdpServer m_udpSrv;

    friend class CMasterClient;
    friend class CLogicClient;
    friend class CLogicStub;
    friend class CNetPeerMgr;
};
}//Logic
}//SFLib

#endif//__SFLIB_LOGICSERVER_LOGICSERVER_H__
