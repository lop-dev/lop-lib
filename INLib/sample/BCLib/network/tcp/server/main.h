//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/tcp/server/main.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_NETWORK_TCP_SERVER_MAIN_H__
#define __BCLIB_SAMPLE_NETWORK_TCP_SERVER_MAIN_H__

#include <BCLib/utility/singleton.h>
#include <BCLib/framework/mainThread.h>
#include <BCLib/network/tcp/tcpServer.h>
#include <BCLib/network/tcp/tcpStubMgr.h>

struct TestInfo
{
    int num;
    BCLib::int64 time;
};

class CTcpServer : public BCLib::Network::CTcpServer, public BCLib::Framework::CMainThread
{
    BCLIB_SINGLETON_DECLARE(CTcpServer);
public:

    static int main();

protected:
    CTcpServer();
    virtual ~CTcpServer();

    virtual bool _init();
    virtual bool _callback();
    virtual int _final();

    virtual BCLib::Network::CTcpStubMgr* _cbGetStubMgr();
    virtual BCLib::Network::CTcpStubSPtr _cbNewStub(const BCLib::Network::CTcpConnectionSPtr& connPtr);

protected:
    BCLib::Network::CTcpStubMgr m_stubMgr;
};

class CTcpStub : public BCLib::Network::CTcpStub
{
public:
    CTcpStub(const BCLib::Network::CTcpConnectionSPtr& netConnPtr);

protected:
    virtual bool _cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize);

    bool m_bSetStateOK;
};


#endif//__BCLIB_SAMPLE_NETWORK_TCP_SERVER_MAIN_H__
