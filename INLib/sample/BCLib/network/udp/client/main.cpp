//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/udp/client/main.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/logFile.h>
#include <BCLib/utility/dateTime.h>
#include <BCLib/network/netInit.h>
#include "main.h"

/////////////////////////////////////

BCLIB_FW_MAIN(CMyUdpClient::main);

BCLIB_SINGLETON_DEFINE(CMyUdpClient)


int CMyUdpClient::main()
{
    BCLib::Utility::CLogFile::singleton().setModuleLevel(BCLib::ELOGMODULE_ALL, BCLib::Utility::CLog::ELOGLEVEL_DEBUG);
    BCLib::Utility::CLogFile::singleton().setOutConsole(true);
    BCLib::Utility::CLogFile::singleton().setOutFile("./udpClient.log");
    BCLib::Utility::CLogFile::singleton().setModuleON(BCLib::ELOGMODULE_ALL);

    return CMyUdpClient::singleton()._main();
}

CMyUdpClient::CMyUdpClient()
{

}

CMyUdpClient::~CMyUdpClient()
{

}

bool CMyUdpClient::_init()
{
    BCLib::Utility::CRealTime realTime;
    realTime.now();
    srand(realTime.getMilliseconds());

    std::string serverIP = BCLib::Framework::CSysConfig::singleton()["1"];
    std::string serverPort = BCLib::Framework::CSysConfig::singleton()["2"];
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "start[%s:%d]", serverIP.c_str(), atoi(serverPort.c_str()));
    realTime.now();
    BCLib::int64 t1 = realTime.getMilliseconds();
    if(!BCLib::Network::CUdpClient::start(serverIP.c_str(), atoi(serverPort.c_str())))
    //if(!BCLib::Network::CUdpClient::start("192.168.1.116", 1600))
    //if(!BCLib::Network::CUdpClient::start("192.168.1.216", 1600))
    {
        realTime.now();
        BCLib::int64 t2 = realTime.getMilliseconds();
        BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "startÊ§°Ü,dura[%lld]", t2-t1);
        return false;
    }
    realTime.now();
    BCLib::int64 t2 = realTime.getMilliseconds();
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "start³É¹¦,dura[%lld]", t2-t1);
    return BCLib::Framework::CMainThread::_init();
}

bool CMyUdpClient::_callback()
{
    static int cnt = 0;
    static BCLib::Utility::CRealTime realTime;
    realTime.now();

    TestInfo* info = (TestInfo*)m_buf;
    info->num = cnt;
    info->time = realTime.getMilliseconds();

    int defBufSize = 1472;
    int pkSize = rand() % defBufSize;
    if(pkSize < sizeof(TestInfo))
    {
        pkSize = sizeof(TestInfo);
    }

    realTime.now();
    BCLib::int64 t1 = realTime.getMilliseconds();
    if(BCLib::Network::CUdpClient::send(m_buf, pkSize) > 0)
    {
        realTime.now();
        BCLib::int64 t2 = realTime.getMilliseconds();
        BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "==> client send msg[%ld:%I64d] size[%ld] dura[%lld]", info->num, info->time, pkSize, t2-t1);
        ++cnt;
    }
    else
    {
        realTime.now();
        BCLib::int64 t2 = realTime.getMilliseconds();
        BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "==> client send msg[%ld:%I64d] size[%ld] Ê§°Ü dura[%lld]", info->num, info->time, pkSize, t2-t1);
    }

    BCLib::Utility::CThread::msleep(1000);
    return true;
}

int CMyUdpClient::_final()
{
    BCLib::Network::CUdpClient::close();
    return BCLib::Framework::CMainThread::_final();
}

bool CMyUdpClient::_cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize)
{
    TestInfo* info = (TestInfo*)msgBuff;
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "<== client recv msg[%ld:%I64d] size[%ld]", info->num, info->time, msgSize);
    return true;
}
