//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/pipeStubMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_PIPESTUBMGR_H__
#define __SFLIB_COMMONSERVER_PIPESTUBMGR_H__

#include <BCLib/utility/singleton.h>
#include <BCLib/network/pipe/pipeStubMgr.h>
#include <SFLib/commonDefine/baseDef.h>
#include <SFLib/message/message.h>
namespace SFLib
{
namespace CommonServer
{
class SFLIB_COMMON_API CPipeStubMgr : public BCLib::Network::CPipeStubMgr
{
    BCLIB_SINGLETON_DECLARE(CPipeStubMgr);

public:
    CPipeStubMgr();
    virtual ~CPipeStubMgr();

    bool delStub(const BCLib::Network::CPipeStubSPtr& stub);
    bool delStub(BCLib::Network::PipeStubID stubID);
    BCLib::Network::CPipeStubSPtr getStubByServerID(ServerID serverID);
    void addServerID(BCLib::Network::PipeStubID stubID, ServerID serverID);

private:
    BCLib::Utility::CMutex m_mutex;
    BCLib::Utility::CHashMap<ServerID, BCLib::Network::PipeStubID> m_serverHashMap;
};
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_PIPESTUBMGR_H__
