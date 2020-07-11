//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/logicServer/masterClient.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_LOGICSERVER_MASTERCLIENT_H__
#define __SFLIB_LOGICSERVER_MASTERCLIENT_H__

#include <SFLib/commonServer/netClient/masterClient.h>

namespace SFLib
{
namespace External
{
class SFLIB_EXTERNAL_API CMasterClient : public SFLib::CommonServer::CMasterClient
{
public:
    CMasterClient();
    virtual ~CMasterClient();

protected:
    virtual void _unhandledMsg(SFLib::CommonServer::SSFMsgLabel& msgLabel, SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);
    //virtual bool _transformMsg(PeerID peerID, SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);

    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::uint16 id, BCLib::Framework::CMsgExecPtr& msgExecPtr);

    //
    virtual void _onMS2XSResApplyForPeerID(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onMS2XSReqCreatePeerID(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

    virtual void _onMS2XSNtfInitPeerID(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

    virtual void _onMS2XSReqLeavePeerID(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onMS2XSNtfRemovePeerID(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    
    //
    virtual void _onMS2XSResPeerEnterServer(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onMS2XSReqEnterServerCreatePeerID(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

    virtual void _onMS2XSResPeerLeaveServer(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onMS2XSReqLeaveServerRemovePeerID(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
};
typedef BCLib::Utility::CSPointer<SFLib::External::CMasterClient> CMasterClientPtr;
}//External
}//SFLib

#endif//__SFLIB_LOGICSERVER_MASTERCLIENT_H__
