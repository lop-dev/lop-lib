//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include "./main.h"
#include <BCLib/utility/logFile.h>


BCLIB_SINGLETON_DEFINE(CPipeClient);
BCLIB_FW_MAIN(CPipeClient::main);

CMyPipeClient::CMyPipeClient()
{
    ;
}

CMyPipeClient::~CMyPipeClient()
{
    ;
}

bool CMyPipeClient::sendMsg()
{
    char buf[1024*2] = {0};
    static int s_id = 0;
    sprintf(buf, "pipe test %d", ++s_id);
    //int len = strlen(buf)+1;
    int len = sizeof(buf);
    if(BCLib::Network::CPipeClient::send(buf, len) == len)
    {
        BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "==> client send msg[%s] size[%d]", buf, len);
    }
    else
    {
        return false;
    }
    if(s_id > 10)
    {
        return false;
    }
    BCLib::Utility::CThread::msleep(1000);
    return true;
}

bool CMyPipeClient::_cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize)
{
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "<== client recv msg[%s] size[%d]", (const char*)msgBuff, msgSize);
    return true;
}

void CMyPipeClient::_cbTerminate()
{
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "pipe terminate");
    return BCLib::Network::CPipeClient::_cbTerminate();
}

int CPipeClient::main()
{
    BCLib::Utility::CLogFile::singleton().setModuleLevel(BCLib::Utility::CLog::ELOGLEVEL_DEBUG, BCLib::Utility::CLog::ELOGLEVEL_DEBUG);
    BCLib::Utility::CLogFile::singleton().setOutConsole(true);
    BCLib::Utility::CLogFile::singleton().setOutFile("./pipeClient.log");
    BCLib::Utility::CLogFile::singleton().setModuleON(BCLib::ELOGMODULE_ALL);

    return CPipeClient::singleton()._main();
}

CPipeClient::CPipeClient()
:m_pipe(NULL)
{
    ;
}

CPipeClient::~CPipeClient()
{
    ;
}

bool CPipeClient::_init()
{
    m_pipe = new CMyPipeClient();
    if(NULL == m_pipe)
    {
        BCLIB_LOG_DEBUG(BCLib::ELOGMODULE_DEFAULT, "创建pipe失败");
        return false;
    }
    char srvName[] = "test";
    if(!m_pipe->connect(srvName))
    {
        BCLIB_LOG_DEBUG(BCLib::ELOGMODULE_DEFAULT, "pipe连接[%s]失败", srvName);
        return false;
    }
    //m_pipe->sendMsg();
    if(!m_pipe->start())
    {
        BCLIB_LOG_DEBUG(BCLib::ELOGMODULE_DEFAULT, "pipe启动失败");
        return false;
    }
    return BCLib::Framework::CMainThread::_init();
}

bool CPipeClient::_callback()
{
    if(m_pipe)
    {
        return m_pipe->sendMsg();
    }
    return true;
}

int CPipeClient::_final()
{
    BCLIB_SAFE_DELETE(m_pipe);
    return BCLib::Framework::CMainThread::_final();
}