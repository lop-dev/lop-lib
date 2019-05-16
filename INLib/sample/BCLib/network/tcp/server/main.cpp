//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/tcp/server/main.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/logFile.h>
#include <BCLib/network/netInit.h>
#include <BCLib/network/tcp/netTime.h>
#include <BCLib/utility/dateTime.h>
#include "main.h"


BCLIB_FW_MAIN(CTcpServer::main);

BCLIB_SINGLETON_DEFINE(CTcpServer)

void testPushStack(char* fmt, BCLib::uint64 u64_1, BCLib::uint32 u32_1, BCLib::uint64 u64_2, BCLib::uint32 u32_2)
{
    BCLib::uint32 s = 0;
    printf("fmt:   [%x-%x]\n", &fmt, &fmt+1);
    printf("u64_1: [%x-%x]\n", &u64_1, &u64_1+1);
    printf("u32_1: [%x-%x]\n", &u32_1, &u32_1+1);
    printf("u64_2: [%x-%x]\n", &u64_2, &u64_2+1);
    printf("u32_2: [%x-%x]\n", &u32_2, &u32_2+1);
    printf("u32_s: [%x-%x]\n", &s, &s+1);
}

int CTcpServer::main()
{
    //timeBeginPeriod(1);

    BCLib::Utility::CLogFile::singleton().setModuleLevel(BCLib::Utility::CLog::ELOGLEVEL_DEBUG, BCLib::Utility::CLog::ELOGLEVEL_DEBUG);
    BCLib::Utility::CLogFile::singleton().setOutConsole(true);
    BCLib::Utility::CLogFile::singleton().setOutFile("./tcpServer.log");

    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "%s", "*************TCP ²âÊÔ·þÎñ¶Ë***********");
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "%s", "***************************************");

    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "sizeof(int)==%d", sizeof(int));
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "sizeof(long)==%d", sizeof(long));
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "sizeof(size_t)==%d", sizeof(size_t));
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "sizeof(BCLib::int32)==%d", sizeof(BCLib::int32));

    BCLib::uint64 u64_1 = 0xFFFFFFFFFFFFFFFF;
    BCLib::uint32 u32_1 = 1;
    BCLib::uint64 u64_2 = 0xFFFFFFFFFFFFFFFF;
    BCLib::uint32 u32_2 = 2;

    //BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "%lld,%d", u64, u32);
    //BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "%ld,%d", u64, u32);
    //BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "%d,%d", u64, u32);
    printf("%llu,%u,%llu,%u\n", u64_1, u32_1, u64_2, u32_2);
    printf("%lu,%u,%lu,%u\n", u64_1, u32_1, u64_2, u32_2);
    printf("%u,%u,%u,%u\n", u64_1, u32_1, u64_2, u32_2);
    testPushStack("abc", u64_1, u32_1, u64_2, u32_2);

    return CTcpServer::singleton()._main();
}

CTcpServer::CTcpServer()
{

}

CTcpServer::~CTcpServer()
{

}

bool CTcpServer::_init()
{
    BCLib::Network::CNetInit::Tcp::Connect::setTimeout(100000);
    std::string serverPort = BCLib::Framework::CSysConfig::singleton()["1"];
    BCLib::int32 port = 0;
    sscanf(serverPort.c_str(), "%d", &port);
    port = 55000;

    BCLib::Network::CNetTimeMgr::singleton().init(true);

    if(!BCLib::Network::CTcpServer::_init())
    {
        return false;
    }
    if(!BCLib::Network::CTcpServer::_setStubPool(100000, 10, 500))
    {
        return false;
    }
    //if(!BCLib::Network::CTcpServer::addAcceptor("0.0.0.0", port))
    //if(!BCLib::Network::CTcpServer::addAcceptor("192.168.1.116", port))
    //{
    //    return false;
    //}
    //if(!BCLib::Network::CTcpServer::addAcceptor("192.168.1.116", port))
    if(!BCLib::Network::CTcpServer::addAcceptor("192.168.1.216", port))
    //if(!BCLib::Network::CTcpServer::addAcceptor("192.168.1.33", port))
    {
        return false;
    }
    return BCLib::Framework::CMainThread::_init();
}

bool CTcpServer::_callback()
{
    BCLib::Utility::CThread::msleep(1);
    return true;
}

int CTcpServer::_final()
{
    BCLib::Network::CTcpServer::_final();
    return BCLib::Framework::CMainThread::_final();
}

BCLib::Network::CTcpStubMgr* CTcpServer::_cbGetStubMgr()
{
    return &m_stubMgr;
}

BCLib::Network::CTcpStubSPtr CTcpServer::_cbNewStub(const BCLib::Network::CTcpConnectionSPtr& connPtr)
{
    CTcpStub* stub = new CTcpStub(connPtr);
    //stub->setRTT(true);
    stub->openNetTime();
    //stub->setSendDelay(100, 0);
    return stub;
}

CTcpStub::CTcpStub(const BCLib::Network::CTcpConnectionSPtr& netConnPtr)
:BCLib::Network::CTcpStub(netConnPtr)
,m_bSetStateOK(false)
{

}

bool CTcpStub::_cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize)
{
    static BCLib::Utility::CRealTime realTime;
    realTime.now();
    BCLib::int64 nowTime = realTime.getMilliseconds();
    //if(msgSize != sizeof(TestInfo))
    //{
    //    return false;
    //}

    if(!m_bSetStateOK)
    {
        m_bSetStateOK = true;
        setState(BCLib::Network::ESS_OKAY);
    }

    TestInfo* testInfo = (TestInfo*)msgBuff;
    //BCLIB_LOG_DEBUG(BCLib::ELOGMODULE_DEFAULT, "<== server recv msg[%d:%lld] size[%d] dT[%lld]", testInfo->num, testInfo->time, msgSize, nowTime - testInfo->time);
    BCLib::uint32 rtt = getRTTInMS();
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "<== server recv msg[%d:%lld] size[%d]", testInfo->num, testInfo->time, msgSize);
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "    netTime[%lld]", BCLib::Network::CNetTimeMgr::singleton().getMilliseconds());
    send(msgBuff, msgSize);
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "==> server send msg[%d:%lld] size[%d] RTT[%d]", testInfo->num, testInfo->time, msgSize, rtt);
    return true;
}
