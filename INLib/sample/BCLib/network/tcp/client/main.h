//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/tcp/client/main.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_NETWORK_TCP_CLIENT_MAIN_H__
#define __BCLIB_SAMPLE_NETWORK_TCP_CLIENT_MAIN_H__

#include <BCLib/utility/singleton.h>
#include <BCLib/framework/mainThread.h>
#include <BCLib/network/tcp/tcpClient.h>


struct TestInfo
{
    int num;
    BCLib::int64 time;
};

class CTcpClt;

class CMyTcpClient : public BCLib::Network::CTcpClient
{
public:
    bool sendMsg();

    CTcpClt* m_owner;

protected:
    virtual bool _cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize);
    virtual void _cbTerminate(const BCLib::Network::CTcpConnectionSPtr& connPtr);

protected:
    char m_buf[128*1024];
};

class CTcpClt : public BCLib::Framework::CMainThread
{
    BCLIB_SINGLETON_DECLARE(CTcpClt);
public:

    static int main();

    CMyTcpClient* m_pClt;

protected:
    CTcpClt();
    virtual ~CTcpClt();

    virtual bool _init();
    virtual bool _callback();
    virtual int _final();

protected:

};

#endif//__BCLIB_SAMPLE_NETWORK_TCP_CLIENT_MAIN_H__
