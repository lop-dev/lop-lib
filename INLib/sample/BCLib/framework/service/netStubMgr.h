//////////////////////////////////////////////////////////////////////
//  created:    2012/08/01
//  filename:   BCLib/framework/netfw/server/netStubMgr.h
//  author:     aaron
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_NETSTUBMGR_H__
#define __BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_NETSTUBMGR_H__

#include <BCLib/utility/singleton.h>
#include <BCLib/network/netStubMgr.h>

class CNetStubMgr : public BCLib::Network::CNetStubMgr
{
    BCLIB_SINGLETON_DECLARE(CNetStubMgr);
public:
    CNetStubMgr();
    virtual ~CNetStubMgr();
};


#endif//__BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_NETSTUBMGR_H__
