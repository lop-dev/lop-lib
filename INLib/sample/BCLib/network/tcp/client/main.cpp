//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/tcp/client/main.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/logFile.h>
#include <BCLib/utility/dateTime.h>
#include <BCLib/network/tcp/netTime.h>
#include "main.h"

/////////////////////////////////////

BCLIB_FW_MAIN(CTcpClt::main);

BCLib::int32 g_sndRate = 1;

BCLIB_SINGLETON_DEFINE(CTcpClt)


int CTcpClt::main()
{
    //timeBeginPeriod(1);

    BCLib::Utility::CLogFile::singleton().setModuleLevel(BCLib::Utility::CLog::ELOGLEVEL_DEBUG, BCLib::Utility::CLog::ELOGLEVEL_DEBUG);
    BCLib::Utility::CLogFile::singleton().setOutConsole(true);
    BCLib::Utility::CLogFile::singleton().setOutFile("./tcpClient.log");
    BCLib::Utility::CLogFile::singleton().setModuleON(BCLib::ELOGMODULE_ALL);

    return CTcpClt::singleton()._main();
}

CTcpClt::CTcpClt()
{
    m_pClt = new CMyTcpClient();
    m_pClt->m_owner = this;
}

CTcpClt::~CTcpClt()
{

}

bool CTcpClt::_init()
{
    BCLib::Utility::CRealTime realTime;
    realTime.now();
    srand(realTime.getMilliseconds());

    BCLib::Network::CNetTimeMgr::singleton().init(false);

/**
    std::string serverIP = BCLib::Framework::CSysConfig::singleton()["1"];
    std::string serverPort = BCLib::Framework::CSysConfig::singleton()["2"];
    std::string sendRate = BCLib::Framework::CSysConfig::singleton()["3"];
    BCLib::int32 port = 0;
    sscanf(serverPort.c_str(), "%d", &port);
    sscanf(sendRate.c_str(), "%d", &g_sndRate);
    if(g_sndRate <= 0)
    {
        g_sndRate = 10;
    }
**/
    g_sndRate = 10;
    g_sndRate = 1000 / g_sndRate;
    //std::string serverIP = "192.168.1.116";
    std::string serverIP = "192.168.1.216";
    //std::string serverIP = "192.168.1.33";
    //std::string serverIP = "127.0.0.1";
    BCLib::int32 port = 55000;

    //if(!connect("127.0.0.1", 8888))
    if(!m_pClt->connect(serverIP.c_str(), port))
    {
        return false;
    }
    if(!m_pClt->start())
    {
        return false;
    }

    m_pClt->setRTT(true);
    //m_pClt->setSendDelay(100, 0);

    return BCLib::Framework::CMainThread::_init();
}

bool CTcpClt::_callback()
{
    if(m_pClt)
    {
        return m_pClt->sendMsg();
    }
    return true;
}

int CTcpClt::_final()
{
    BCLIB_SAFE_DELETE(m_pClt);
    return BCLib::Framework::CMainThread::_final();
}

static BCLib::Utility::CRealTime g_realTime;
bool CMyTcpClient::sendMsg()
{
    static int cnt = 0;
    g_realTime.now();

    TestInfo* info = (TestInfo*)m_buf;
    info->num = cnt;
    info->time = g_realTime.getMilliseconds();

    int pkSize = 0;
    int defBufSize = 2 * 1024;
    if(rand() * 10.0f / RAND_MAX < 1.0f)
    {
        pkSize = (rand() * 10) % sizeof(m_buf);
        if(pkSize <= defBufSize)
        {
            pkSize = defBufSize + 1;
        }
    }
    else
    {
        pkSize = rand() % defBufSize;
        if(pkSize < sizeof(TestInfo))
        {
            pkSize = sizeof(TestInfo);
        }
    }
    if(pkSize > 1024 * 10)
        pkSize = 1024 * 10;
    pkSize = sizeof(TestInfo);

    BCLib::uint32 rtt = getRTTInMS();

    ++cnt;
    if(send(m_buf, pkSize) > 0)
    {
        //BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "==> client send msg[%d:%lld] size[%d] RTT[%d]", info->num, info->time, pkSize, rtt);
        BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "==> client send msg[%d:%lld] size[%d]", info->num, info->time, pkSize);
    }

    //if(cnt > 200)
    //{
    //    //close();
    //    return false;
    //}

    BCLib::Utility::CThread::msleep(g_sndRate);
    return true;
}

bool CMyTcpClient::_cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize)
{
    g_realTime.now();
    BCLib::int64 nowTime = g_realTime.getMilliseconds();
    //if(msgSize != sizeof(TestInfo))
    //{
    //    return false;
    //}
    TestInfo* info = (TestInfo*)msgBuff;
    //BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "<== client recv msg[%d:%lld] size[%d] rRTT[%lld]", info->num, info->time, msgSize, nowTime - info->time);
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "<== client recv msg[%d:%lld] size[%d] rRTT[%d]", info->num, info->time, msgSize, getRTTInMS());
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "    netTime[%lld]", BCLib::Network::CNetTimeMgr::singleton().getMilliseconds());
    return true;
}

void CMyTcpClient::_cbTerminate(const BCLib::Network::CTcpConnectionSPtr& connPtr)
{
    //m_owner->m_pClt = NULL;
    //delete this;
    BCLib::Network::CTcpClient::_cbTerminate(connPtr);

    //BCLib::Network::CTcpClient::_cbTerminate();

    //while(!connect("127.0.0.1", 8888))
    //{
    //    BCLib::Utility::CThread::msleep(1000);
    //}

    //start();
}