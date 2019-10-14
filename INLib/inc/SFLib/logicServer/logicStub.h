//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/logicServer/logicStub.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_LOGICSERVER_LOGINSTUB_H__
#define __SFLIB_LOGICSERVER_LOGINSTUB_H__

#include <SFLib/commonServer/tcpStub.h>
#include <SFLib/logicServer/netPeer.h>
#include <SFLib/commonServer/netClient/tcpClient.h>

namespace SFLib
{
namespace Logic
{
class CLogicStub : public SFLib::CommonServer::CTcpStub
{
public:
    CLogicStub(const BCLib::Network::CTcpConnectionSPtr& netConn);
    virtual ~CLogicStub();

    /// @brief 只在有真正玩家连接的服务器上才会生效
    /// @return PeerID
    virtual PeerID getPeerID() const;

    void setNetPeerPtr(CNetPeerPtr& logicPeer);
    CNetPeerPtr getNetPeerPtr() { return m_logicPeer; }

protected:
    /// @brief 未处理的消息放到 CNetMsgQueueBySFMsgLabel 队列中
    /// @return void
    /// @param msgLabel
    /// @param msg
    /// @param msgSize
    virtual void _unhandledMsg(SFLib::CommonServer::SSFMsgLabel& msgLabel, SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);
    virtual bool _transformMsg(PeerID peerID, SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);

    virtual void _cbTerminate();

    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::uint16 id, BCLib::Framework::CMsgExecPtr& msgExecPtr);
    virtual void _onXX2XSNtfServerType(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

    virtual void _onXS2XSReqEnterServer(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onXS2XSResEnterServer(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onXS2XSSetEnterServer(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onXS2XSReqLeaveServer(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onXS2XSResLeaveServer(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

private:
    void _setServerInfo(CNetPeerPtr netPeerPtr);

protected:
    CNetPeerPtr m_logicPeer;
};
typedef BCLib::Utility::CSPointer<CLogicStub> CLogicStubPtr;
}//Logic
}//SFLib

#endif//__SFLIB_LOGICSERVER_LOGINSTUB_H__
