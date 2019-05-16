//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/singleton.h>
#include <BCLib/network/pipe/pipeServer.h>
#include <BCLib/network/pipe/pipeStubMgr.h>
#include <BCLib/framework/mainThread.h>


class CPipeServer : public BCLib::Network::CPipeServer, public BCLib::Framework::CMainThread
{
    BCLIB_SINGLETON_DECLARE(CPipeServer);
public:
    static int main();

protected:
    CPipeServer();
    virtual ~CPipeServer();

    virtual bool _init();
    virtual bool _callback();
    virtual int _final();

    virtual BCLib::Network::CPipeStubMgr* _cbGetStubMgr();
    virtual BCLib::Network::CPipeStubSPtr _cbNewStub(const BCLib::Network::CPipeSPtr& pipePtr);

protected:
    BCLib::Network::CPipeStubMgr m_stubMgr;
};

class CPipeStub : public BCLib::Network::CPipeStub
{
public:
    CPipeStub(const BCLib::Network::CPipeSPtr& pipePtr);

protected:
    virtual bool _cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize);
    virtual void _cbTerminate();
};