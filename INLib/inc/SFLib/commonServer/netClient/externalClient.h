//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/netClient/externalClient.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_NETCLIENT_EXTERNALCLIENT_H__
#define __SFLIB_COMMONSERVER_NETCLIENT_EXTERNALCLIENT_H__

#include <SFLib/commonServer/netClient/commonClient.h>
#include <SFLib/message/peerMsg.h>

namespace SFLib
{
namespace CommonServer
{
class SFLIB_COMMON_API CExternalClient : public CCommonClient
{
public:
    CExternalClient();
    virtual ~CExternalClient();

protected:
    virtual bool _transformMsg(PeerID peerID, SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);
    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::uint16 id, BCLib::Framework::CMsgExecPtr& msgExecPtr);

    virtual void _onXX2XSNtfServerType(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

    virtual void _onXS2XSResEnterServer(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onXS2XSResLeaveServer(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

    virtual void _onXS2XSResEnterOrLeaveServer(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

private:
    void _doResEnterServer(SFLib::Message::SResEnterOrLeaveServer& resEnterOrLeaveServer);
    void _doResLeaveServer(SFLib::Message::SResEnterOrLeaveServer& resEnterOrLeaveServer);
};
typedef BCLib::Utility::CSPointer<SFLib::CommonServer::CExternalClient> CExternalClientPtr;
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_NETCLIENT_EXTERNALCLIENT_H__
