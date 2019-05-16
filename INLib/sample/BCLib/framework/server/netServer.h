//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/netfw/server/netServer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_NETSERVER_H__
#define __BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_NETSERVER_H__

#include <BCLib/utility/pointer.h>
#include <BCLib/network/tcp/tcpServer.h>
#include <BCLib/framework/serviceID.h>
#include "netStub.h"

class CTcpServer : public BCLib::Network::CTcpServer
{
public:
    CTcpServer();
    virtual ~CTcpServer();

    bool init();
    void final();

    bool setStubPool(BCLib::uint32 maxStub, BCLib::uint32 minThread, BCLib::uint32 maxStubPerThd);

protected:
    virtual BCLib::Network::CTcpStubMgr* _cbGetStubMgr();
    virtual BCLib::Network::CTcpStubSPtr _cbNewStub(const BCLib::Network::CTcpConnectionSPtr& connPtr);
};


#endif//__BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_NETSERVER_H__
