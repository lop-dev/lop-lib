//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/udp/client/main.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_NETWORK_UDP_CLIENT_MAIN_H__
#define __BCLIB_SAMPLE_NETWORK_UDP_CLIENT_MAIN_H__

#include <BCLib/utility/singleton.h>
#include <BCLib/framework/mainThread.h>
#include <BCLib/network/udp/udpClient.h>


struct TestInfo
{
    int num;
    BCLib::int64 time;
};

class CMyUdpClient : BCLib::Network::CUdpClient, public BCLib::Framework::CMainThread
{
    BCLIB_SINGLETON_DECLARE(CMyUdpClient);
public:

    static int main();

protected:
    CMyUdpClient();
    virtual ~CMyUdpClient();

    virtual bool _init();
    virtual bool _callback();
    virtual int _final();

    virtual bool _cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize);

protected:
    char m_buf[256*1024];
};

#endif//__BCLIB_SAMPLE_NETWORK_UDP_CLIENT_MAIN_H__
