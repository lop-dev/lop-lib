//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/masterServer/masterStub.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_MASTERSERVER_MASTERSTUB_H__
#define __SFLIB_MASTERSERVER_MASTERSTUB_H__

#include <SFLib/commonServer/tcpStub.h>

namespace SFLib
{
namespace Master
{
class CMasterStub : public SFLib::CommonServer::CTcpStub
{
public:
    CMasterStub(const BCLib::Network::CTcpConnectionSPtr& netConn);
    virtual ~CMasterStub();

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
    virtual void _onXS2MSResAccept(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onXS2MSNtfShakeHands(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onXS2MSNtfServerInited(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

    virtual void _onXS2MSReqEnterServer(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onXS2MSReqLeaveServer(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

    virtual void _onXS2MSReqValidRandKey(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onXS2MSAckServerSave(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

	virtual ServerID _getServerIDByType(EServerType serverType);
};
}//Master
}//SFLib

#endif//__SFLIB_MASTERSERVER_MASTERSTUB_H__
