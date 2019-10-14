//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/netClient/masterClient.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_NETCLIENT_MASTERCLIENT_H__
#define __SFLIB_COMMONSERVER_NETCLIENT_MASTERCLIENT_H__

#include <SFLib/commonServer/netClient/tcpClient.h>

namespace SFLib
{
namespace CommonServer
{
class CMasterClient : public CTcpClient
{
public:
    CMasterClient();
    virtual ~CMasterClient();

    bool ackServerSave(GroupID groupID);

protected:
    virtual bool _enterPoll();
    virtual void _leavePoll();

    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::uint16 id, BCLib::Framework::CMsgExecPtr& msgExecPtr);
    virtual void _onMS2XSReqAccept(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onMS2XSNtfLogicServerList(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onMS2XSNtfExternalServerList(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onMS2XSNtfVerifySuccess(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onMS2XSNtfRemoveServer(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onMS2XSReqServerSave(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
	virtual void _onMS2XSNtfServerClose(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
};
typedef BCLib::Utility::CSPointer<CMasterClient> CMasterClientPtr;
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_NETCLIENT_MASTERCLIENT_H__
