//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/pipe/pipeServer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_NETWORK_PIPE_PIPESERVER_H__
#define __BCLIB_NETWORK_PIPE_PIPESERVER_H__

#include <BCLib/network/baseDef.h>
#include <BCLib/utility/spointer.h>
#include <BCLib/utility/uniqueID.h>


namespace BCLib
{
namespace Network
{
struct SNewPipe;
class CPipe;
typedef BCLib::Utility::CSPointer<CPipe> CPipeSPtr;
class CPipeStub;
typedef BCLib::Utility::CSPointer<CPipeStub> CPipeStubSPtr;
class CPipeMgr;
class CPipeStubMgr;

class BCLIB_NETWORK_API CPipeServer
{
public:
    CPipeServer();
    virtual ~CPipeServer();

    bool startAccept(const char* serverName);

    CPipeStubMgr* getStubMgr() const;

protected:
    bool _init();
    BCLib::int32 _final();
    bool _newPipe(const SNewPipe& newPipe);
    void _delPipe(const CPipeSPtr& pipePtr);

    virtual CPipeStubMgr* _cbGetStubMgr() = 0;
    virtual CPipeStubSPtr _cbNewStub(const BCLib::Network::CPipeSPtr& pipePtr) = 0;

private:
    CPipeMgr* m_pipeMgr;
    CPipeStubMgr* m_pipeStubMgr;
    BCLib::Utility::CUniqueID32 m_uniqueID;
};
}//Network
}//BCLib

#endif//__BCLIB_NETWORK_PIPE_PIPESERVER_H__
