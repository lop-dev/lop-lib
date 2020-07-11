//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/logicServer/logicClient.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_LOGICSERVER_LOGICCLIENT_H__
#define __SFLIB_LOGICSERVER_LOGICCLIENT_H__

#include <SFLib/commonServer/netClient/logicClient.h>
#include <SFLib/logicServer/netPeer.h>

namespace SFLib
{
namespace Logic
{
class SFLIB_LOGIC_API CLogicClient : public SFLib::CommonServer::CLogicClient
{
public:
    CLogicClient();
    virtual ~CLogicClient();

protected:
    virtual void _unhandledMsg(SFLib::CommonServer::SSFMsgLabel& msgLabel, SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);
    virtual bool _transformMsg(PeerID peerID, SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);

    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::uint16 id, BCLib::Framework::CMsgExecPtr& msgExecPtr);

    virtual void _onXS2XSReqEnterServer(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onXS2XSResEnterServer(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onXS2XSSetEnterServer(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onXS2XSReqLeaveServer(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onXS2XSResLeaveServer(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

private:
    void _setServerInfo(CNetPeerPtr netPeerPtr);
};
}//Logic
}//SFLib

#endif//__SFLIB_LOGICSERVER_LOGICCLIENT_H__
