//////////////////////////////////////////////////////////////////////
//  created:    2012/08/01
//  filename:   BCLib/framework/netfw/server/netStub.h
//  author:     aaron
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_NETSTUB_H__
#define __BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_NETSTUB_H__

#include <BCLib/utility/pointer.h>
#include <BCLib/network/netStub.h>
#include <BCLib/framework/message.h>

class CNetServer;
class CNetService;

class CNetStub : public BCLib::Network::CNetStub
{
public:
    CNetStub(const BCLib::Network::CNetConnectionSPtr& netConn);
    virtual ~CNetStub();

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

typedef BCLib::Utility::CPointer<CNetStub> CNetStubPtr;


#endif//__BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_NETSTUB_H__
