//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/externalServer/externalStub.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_EXTERNALSERVER_EXTERNALSTUB_H__
#define __SFLIB_EXTERNALSERVER_EXTERNALSTUB_H__

#include <SFLib/commonServer/tcpStub.h>
#include <SFLib/commonServer/netClient/netClient.h>

namespace SFLib
{
namespace External
{
class CExternalStub : public SFLib::CommonServer::CTcpStub
{
public:
    CExternalStub(const BCLib::Network::CTcpConnectionSPtr& netConn);
    virtual ~CExternalStub();

protected:
    /// @brief 未处理的消息放到 CNetMsgQueueByGroupKey 队列中
    /// @return void
    /// @param msgLabel
    /// @param msg
    /// @param msgSize
    virtual void _unhandledMsg(SFLib::CommonServer::SSFMsgLabel& msgLabel, SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);
    //virtual bool _transformMsg(PeerID peerID, SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);

    virtual void _cbTerminate();
    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::uint16 id, BCLib::Framework::CMsgExecPtr& msgExecPtr);
    virtual void _onXX2XSNtfServerType(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
	virtual void _onXS2XSReqEnterServer(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

protected:
};
typedef BCLib::Utility::CSPointer<CExternalStub> CLogicStubPtr;
}//External
}//SFLib

#endif//__SFLIB_EXTERNALSERVER_EXTERNALSTUB_H__
