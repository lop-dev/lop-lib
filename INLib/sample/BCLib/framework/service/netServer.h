//////////////////////////////////////////////////////////////////////
//  created:    2012/08/01
//  filename:   BCLib/framework/netfw/server/netServer.h
//  author:     aaron
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_NETSERVER_H__
#define __BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_NETSERVER_H__

#include <BCLib/utility/pointer.h>
#include <BCLib/network/netServer.h>
#include <BCLib/framework/serviceID.h>
#include "netStub.h"

class CNetServer : public BCLib::Network::CNetServer
{
public:
    CNetServer();
    virtual ~CNetServer();

    bool init();
    void final();

    bool setStubPool(BCLib::Network::ENetType netType, BCLib::uint32 maxStub, BCLib::uint32 minThread, BCLib::uint32 maxStubPerThd);

protected:
    virtual BCLib::Network::CNetStubMgr* _cbGetNetStubMgr();
    virtual BCLib::Network::CNetStub* _cbNewStub(BCLib::Network::ENetType netType, const BCLib::Network::CNetConnectionSPtr& connPtr);
};


#endif//__BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_NETSERVER_H__
