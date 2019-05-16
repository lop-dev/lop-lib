//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/udp/server/main.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/logFile.h>
#include <BCLib/network/netInit.h>
#include "main.h"


BCLIB_FW_MAIN(CMyUdpServer::main);

BCLIB_SINGLETON_DEFINE(CMyUdpServer)

CMyUdpProxy::CMyUdpProxy()
{

}

CMyUdpProxy::~CMyUdpProxy()
{

}

bool CMyUdpProxy::_cbParseMsg(const BCLib::Network::CSockAddr& peerAddr, const void* msgBuff, BCLib::uint32 msgSize)
{
    TestInfo* testInfo = (TestInfo*)msgBuff;
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "<== server recv msg[%ld:%I64d] size[%ld]", testInfo->num, testInfo->time, msgSize);
    if(send(peerAddr, msgBuff, msgSize) > 0)
    {
        BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "==> server send msg[%ld:%I64d] size[%ld]", testInfo->num, testInfo->time, msgSize);
    }
    else
    {
        BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "==> server send msg[%ld:%I64d] size[%ld] Ê§°Ü", testInfo->num, testInfo->time, msgSize);
    }
    return true;
}

int CMyUdpServer::main()
{
    BCLib::Utility::CLogFile::singleton().setModuleLevel(BCLib::ELOGMODULE_ALL, BCLib::Utility::CLog::ELOGLEVEL_DEBUG);
    BCLib::Utility::CLogFile::singleton().setOutConsole(true);
    BCLib::Utility::CLogFile::singleton().setOutFile("./udpServer.log");
    BCLib::Utility::CLogFile::singleton().setModuleON(BCLib::ELOGMODULE_ALL);

    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "%s", "*************UDP ²âÊÔ·þÎñ¶Ë***********");
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "%s", "***************************************");

    return CMyUdpServer::singleton()._main();
}

CMyUdpServer::CMyUdpServer()
{

}

CMyUdpServer::~CMyUdpServer()
{

}

bool CMyUdpServer::_init()
{
    if(!BCLib::Network::CUdpServer::init())
    {
        return false;
    }
    std::string serverIP = BCLib::Framework::CSysConfig::singleton()["1"];
    std::string serverPort = BCLib::Framework::CSysConfig::singleton()["2"];
    BCLib::Network::CUdpProxySPtr udpProxy = new CMyUdpProxy();
    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "addUdpAcceptor[%s:%d]", serverIP.c_str(), atoi(serverPort.c_str()));
    if(!BCLib::Network::CUdpServer::addAcceptor(serverIP.c_str(), atoi(serverPort.c_str()), udpProxy))
    //if(!BCLib::Network::CUdpServer::addAcceptor("192.168.1.116", 1600, udpProxy))
    //if(!BCLib::Network::CUdpServer::addAcceptor("192.168.1.216", 1600, udpProxy))
    {
        return false;
    }
    return BCLib::Framework::CMainThread::_init();
}

int CMyUdpServer::_final()
{
    BCLib::Network::CUdpServer::final();
    return BCLib::Framework::CMainThread::_final();
}
