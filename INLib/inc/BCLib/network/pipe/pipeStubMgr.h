//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/pipe/pipeStubMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_NETWORK_PIPE_PIPESTUBMGR_H__
#define __BCLIB_NETWORK_PIPE_PIPESTUBMGR_H__

#include <BCLib/network/baseDef.h>
#include <BCLib/utility/thread/mutex.h>
#include <BCLib/network/pipe/pipeStub.h>


namespace BCLib
{
namespace Network
{
class BCLIB_NETWORK_API CPipeStubMgr
{
public:
    class CPipeStubMgrCallback
    {
    public:
        virtual ~CPipeStubMgrCallback()
        {
        }
        virtual bool execute(const CPipeStubSPtr& stubPtr) = 0;
    };

public:
    CPipeStubMgr();
    virtual ~CPipeStubMgr();

    bool addStub(const CPipeStubSPtr& stubPtr);
    bool delStub(const CPipeStubSPtr& stubPtr);
    bool delStub(PipeStubID stubID);
    CPipeStubSPtr getStubByStubID(PipeStubID stubID);
    CPipeStubSPtr getStubByPipeID(PipeID pipeID);
    void traversal(CPipeStubMgrCallback& cb);

    bool sendMsg(PipeStubID stubID, BCLib::Utility::CStream& stream);
    bool sendMsg(PipeStubID stubID, const void* msgBuff, BCLib::uint32 buffSize);

private:
    BCLib::Utility::CMutex m_mutex;
    std::map<PipeStubID, CPipeStubSPtr> m_stubMap;
    std::map<PipeID, CPipeStubSPtr> m_pipeMap;
};
}//Network
}//BCLib

#endif//__BCLIB_NETWORK_PIPE_PIPESTUBMGR_H__
