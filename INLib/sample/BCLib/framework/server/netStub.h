//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/netfw/server/netStub.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_NETSTUB_H__
#define __BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_NETSTUB_H__

#include <BCLib/utility/pointer.h>
#include <BCLib/network/tcp/tcpStub.h>
#include <BCLib/framework/message.h>

class CTcpServer;
class CNetService;

class CTcpStub : public BCLib::Network::CTcpStub
{
public:
    CTcpStub(const BCLib::Network::CTcpConnectionSPtr& netConn);
    virtual ~CTcpStub();

    bool sendMsg(const BCLib::Framework::CMessage& msg);
    bool sendMsg(const BCLib::Framework::SMessage* msg, const BCLib::uint32 msgSize);

protected:
    virtual bool _cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize);
    virtual void _cbTerminate();

    bool _onReqLogin(const BCLib::Framework::SMessage* msg, const BCLib::uint32 msgSize);
    bool _onReqService(const BCLib::Framework::SMessage* msg, const BCLib::uint32 msgSize);
    bool _onResPing(const BCLib::Framework::SMessage* msg, const BCLib::uint32 msgSize);
    bool _onResServiceClose(const BCLib::Framework::SMessage* msg, const BCLib::uint32 msgSize);
    bool _onNtfMsg(const BCLib::Framework::SMessage* msg, const BCLib::uint32 msgSize);
};

typedef BCLib::Utility::CPointer<CTcpStub> CNetStubPtr;


#endif//__BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_NETSTUB_H__
