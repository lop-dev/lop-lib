//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/netfw/server/netStubMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_NETSTUBMGR_H__
#define __BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_NETSTUBMGR_H__

#include <BCLib/utility/singleton.h>
#include <BCLib/network/tcp/tcpStubMgr.h>

class CTcpStubMgr : public BCLib::Network::CTcpStubMgr
{
    BCLIB_SINGLETON_DECLARE(CTcpStubMgr);
public:
    CTcpStubMgr();
    virtual ~CTcpStubMgr();
};


#endif//__BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_NETSTUBMGR_H__
