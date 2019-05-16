//////////////////////////////////////////////////////////////////////
//  created:    2013/11/11
//  filename:   
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include "./main.h"
#include <BCLib/utility/logFile.h>


BCLIB_SINGLETON_DEFINE(CPipeServer);
BCLIB_FW_MAIN(CPipeServer::main);

int CPipeServer::main()
{
    BCLib::Utility::CLogFile::singleton().setModuleLevel(BCLib::Utility::CLog::ELOGLEVEL_DEBUG, BCLib::Utility::CLog::ELOGLEVEL_DEBUG);
    BCLib::Utility::CLogFile::singleton().setOutConsole(true);
    BCLib::Utility::CLogFile::singleton().setOutFile("./pipeServer.log");

    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "%s", "*************pipe ²âÊÔ·þÎñ¶Ë***********");
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "%s", "****************************************");

    return CPipeServer::singleton()._main();
}

CPipeServer::CPipeServer()
{
    ;
}

CPipeServer::~CPipeServer()
{
    ;
}

bool CPipeServer::_init()
{
    if(!BCLib::Network::CPipeServer::_init())
    {
        BCLIB_LOG_DEBUG(BCLib::ELOGMODULE_DEFAULT, "pipeServer initÊ§°Ü");
        return false;
    }
    char srvName[] = "test";
    if(!BCLib::Network::CPipeServer::startAccept(srvName))
    {
        BCLIB_LOG_DEBUG(BCLib::ELOGMODULE_DEFAULT, "pipeServerÆô¶¯Ê§°Ü");
        return false;
    }
    return BCLib::Framework::CMainThread::_init();
}

bool CPipeServer::_callback()
{
    static int s_id = 0;
    ++s_id;
    if(s_id > 20)
    {
        return false;
    }
    BCLib::Utility::CThread::msleep(1000);
    return true;
}

int CPipeServer::_final()
{
    BCLib::Network::CPipeServer::_final();
    return BCLib::Framework::CMainThread::_final();
}

BCLib::Network::CPipeStubMgr* CPipeServer::_cbGetStubMgr()
{
    return &m_stubMgr;
}

BCLib::Network::CPipeStubSPtr CPipeServer::_cbNewStub(const BCLib::Network::CPipeSPtr& pipePtr)
{
    CPipeStub* stub = new CPipeStub(pipePtr);
    return stub;
}

CPipeStub::CPipeStub(const BCLib::Network::CPipeSPtr& pipePtr)
:BCLib::Network::CPipeStub(pipePtr)
{
    ;
}

bool CPipeStub::_cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize)
{
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "<== server recv msg[%s] size[%d]", (const char*)msgBuff, msgSize);
    if(send(msgBuff, msgSize) == msgSize)
    {
        BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "==> server send msg[%s] size[%d]", (const char*)msgBuff, msgSize);
        return true;
    }
    return false;
}

void CPipeStub::_cbTerminate()
{
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "pipe terminate");
    return BCLib::Network::CPipeStub::_cbTerminate();
}