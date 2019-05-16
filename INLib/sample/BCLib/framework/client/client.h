//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/netfw/client/client.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_FRAMEWORK_NETFW_CLIENT_CLIENT_H__
#define __BCLIB_SAMPLE_FRAMEWORK_NETFW_CLIENT_CLIENT_H__
#include <BCLib/network/tcp/tcpClient.h>
#include <BCLib/framework/message.h>

class CClient : public BCLib::Network::CTcpClient
{
public:
    CClient();
    virtual ~CClient();

    bool sendMsg(const BCLib::Framework::CMessage& msg);
    bool sendMsg(const BCLib::Framework::SMessage* msg, const BCLib::uint32 msgSize);

protected:
    virtual void _leavePoll();
    virtual bool _cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize);
};
#endif//__BCLIB_SAMPLE_FRAMEWORK_NETFW_CLIENT_CLIENT_H__
