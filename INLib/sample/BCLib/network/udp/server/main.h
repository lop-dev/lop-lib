//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/udp/server/main.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_NETWORK_UDP_SERVER_MAIN_H__
#define __BCLIB_SAMPLE_NETWORK_UDP_SERVER_MAIN_H__

#include <BCLib/utility/singleton.h>
#include <BCLib/framework/mainThread.h>
#include <BCLib/network/udp/udpServer.h>

struct TestInfo
{
    int num;
    BCLib::int64 time;
};

class CMyUdpProxy : public BCLib::Network::CUdpProxy
{
public:
    CMyUdpProxy();
    ~CMyUdpProxy();

protected:
    virtual bool _cbParseMsg(const BCLib::Network::CSockAddr& peerAddr, const void* msgBuff, BCLib::uint32 msgSize);
};

class CMyUdpServer : public BCLib::Network::CUdpServer, public BCLib::Framework::CMainThread
{
    BCLIB_SINGLETON_DECLARE(CMyUdpServer);
public:

    static int main();

protected:
    CMyUdpServer();
    virtual ~CMyUdpServer();

    virtual bool _init();
    virtual int _final();

protected:

};

#endif//__BCLIB_SAMPLE_NETWORK_UDP_SERVER_MAIN_H__
