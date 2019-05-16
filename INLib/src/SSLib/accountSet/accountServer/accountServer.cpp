//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SSLib/accountSet/accountServer/accountServer.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/io/directoryInfo.h>
#include <BCLib/utility/logFile.h>
#include <BCLib/framework/sysConfig.h>
#include <BCLib/database/connection.h>
#include <BCLib/database/dataReader.h>
#include <BCLib/network/netInit.h>
#include <BCLib/utility/io/xmlFile.h>
#include <BCLib/framework/mainThread.h>

#ifdef WIN32
#include <MWLib/crashDump/crashDump.h>
#endif//WIN32

#include "accountApp.h"
#include "accountServer.h"
#include "./network/netStubMgr.h"
#include "./network/netStub.h"
#include "./databaseMgr/databaseMgr.h"
#include "./databaseTask/databaseTaskMgr.h"

BCLIB_FW_MAIN(SSLib::AccountServer::CAccountServer::main)

BCLIB_SINGLETON_DEFINE(SSLib::AccountServer::CAccountServer);

//BCLib::Utility::CLogFile g_globalLog;

namespace SSLib
{
namespace AccountServer
{

#ifdef WIN32
    BOOL WINAPI CtrlCHandler(DWORD CtrlType)
    {
        CAccountApp::singleton().terminate();
        return TRUE;
    }

#endif

//////////////////////////////////////////////////////////////////////////
#ifdef WIN32
std::string GetDumpFileName(const std::string& moduleName)
{
    char dumpFileName[_MAX_PATH];

    BCLib::Utility::CDateTime dateTime;
    dateTime.now();

    sprintf_s(dumpFileName,
        sizeof(dumpFileName),
        "%s_%s_%02d%02d_%02d%02d.dmp",
        moduleName.c_str(),
        "Temp",
        dateTime.getMonth(),
        dateTime.getDay(),
        dateTime.getHour(),
        dateTime.getMinute());

    return dumpFileName;
}
#endif//WIN32
//////////////////////////////////////////////////////////////////////////

CAccountServer::CAccountServer():m_dyDBName("")
{
    ;
}

CAccountServer::~CAccountServer()
{
    ;
}

bool CAccountServer::hasUnLawKeyWord(const std::string& strName) const
{
    //     std::string strTemp(strName);
    //     MWLib::Misc::ANSI_To_UTF8(strTemp);
    return m_WordFilter.hasUnLawKeyWordInUTF8(strName);
}

bool CAccountServer::_init()
{
    if(!BCLib::Network::CTcpServer::_init())
    {
        return false;
    }

    if (!_initConfig())
    {
        return false;
    }
    CAccountApp::singleton().init();
    CAccountApp::singleton().start();
    return true;
}

bool CAccountServer::_callback()
{

    BCLib::Utility::CThread::msleep(1);
    return true;
    //return BCLib::Framework::CMainThread::_callback();
}

int CAccountServer::_final()
{
    //return BCLib::Network::CTcpServer::_final();
    BCLib::Network::CTcpServer::_final();
    return BCLib::Framework::CMainThread::_final();
}

BCLib::Network::CTcpStubMgr* CAccountServer::_cbGetStubMgr()
{
    return &CTcpStubMgr::singleton();
}

BCLib::Network::CTcpStubSPtr CAccountServer::_cbNewStub(const BCLib::Network::CTcpConnectionSPtr& connPtr)
{
    return new CTcpStub(connPtr);
}

bool CAccountServer::_initConfig()
{
    //if (!BCLib::Framework::CMainThread::_init()){
    //    BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "≥ı ºªØ  ß∞‹");
    //    return false;
    //}

    //
    BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "∆Ù∂ØNetServer≥…π¶");

    std::string currpath = BCLib::Utility::CDirectoryInfo::getExeDirectory();
    std::string fullConfig = currpath + "/";
    std::string strAvailability = BCLib::Framework::CSysConfig::singleton()["LocalBD/Availability"];
    if (strAvailability == "true")
    {
        std::string strDbFile = BCLib::Framework::CSysConfig::singleton()["LocalBD/DatabaseFile"];
        if (strDbFile.empty() || strDbFile == "")
		{
             strDbFile = "AccountServerDB.xml";
        }
		strDbFile = fullConfig + strDbFile;

		if (!CDatabaseMgr::singleton().init(strDbFile))
		{
			BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "CDatabaseMgr init Failed!");
			return false;
		}
    }

    //
    BCLib::uint32 maxTcpStub = BCLib::Framework::CSysConfig::singleton().getValueInt("Network","TcpPoolCapacity");
    if (maxTcpStub <= 0){
        maxTcpStub = 1024;
    }

    BCLib::uint32 minThreadTcpStub = BCLib::Framework::CSysConfig::singleton().getValueInt("Network", "TcpPoolMinThread");
    if (minThreadTcpStub <= 0){
        minThreadTcpStub = 1;
    }

    BCLib::uint32 maxTcpStubPerThd = BCLib::Framework::CSysConfig::singleton().getValueInt("Network", "TcpCountPerThread");
    if (maxTcpStubPerThd <= 0){
        maxTcpStubPerThd = 512;
    }

    if(!BCLib::Network::CTcpServer::_setStubPool(maxTcpStub, minThreadTcpStub, maxTcpStubPerThd))
    {
        return false;
    }

    BCLib::Network::CNetInit::Stub::setVerifyTimeout(60000);

    //
    std::string acceptIP = BCLib::Framework::CSysConfig::singleton()["Network/AcceptIP"];
    if (acceptIP.empty() || acceptIP == "") {
        acceptIP = "0.0.0.0";
    }

    BCLib::uint16 acceptPort = BCLib::Framework::CSysConfig::singleton().getValueInt("Network", "AcceptPort");
    if (acceptPort == 0) {
        acceptPort = 8700;
    }

    //if (!addAcceptor(acceptIP.c_str(), acceptPort)){
    //    BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "…Ë÷√º‡Ã˝∂Àø⁄ ß∞‹");
    //    return false;
    //}

    BCLib::Utility::CStringA refAcceptIP = acceptIP;
    std::list<BCLib::Utility::CStringA> acceptIPList;
    refAcceptIP.split(acceptIPList, ',');

    std::list<BCLib::Utility::CStringA>::iterator it;
    for (it = acceptIPList.begin(); it != acceptIPList.end(); ++it)
    {
        BCLib::Utility::CStringA strIP = *it;
        if (!addAcceptor(strIP.c_str(), acceptPort))
        {
            BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "∆Ù∂Øº‡Ã˝∂Àø⁄ ß∞‹[%s:%d]", strIP.c_str(), acceptPort);
            return false;
        }
    }

    //
    fullConfig += "screenWordList.csv";
    BCLib::Utility::CDirectoryInfo::standardization(fullConfig);
    FILE* pkFile = fopen(fullConfig.c_str(), "rt");
    if(!pkFile)
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "Load [%s] Failed!", fullConfig.c_str());
        return false;
    }

    fseek(pkFile, 0, SEEK_END);
    size_t uiFileSize = ftell(pkFile);
    char* pkBuf = new char[uiFileSize];
    fseek(pkFile, 0, SEEK_SET);
    fread(pkBuf, uiFileSize, 1, pkFile);
    fclose(pkFile);

    m_WordFilter.addUnLawKeyWordsInUTF8(pkBuf);
    delete[] pkBuf;

    return true;
}

int CAccountServer::main()
{
#ifdef WIN32
	MWLib::CrashDump::SetupCrashDump(GetDumpFileName, NULL, MiniDumpWithFullMemory);
    ::SetConsoleCtrlHandler(CtrlCHandler, TRUE);
#endif//WIN32

    BCLib::Utility::CLogFile::singleton().setOutFile("accountServer.log");
    BCLib::Utility::CLogFile::singleton().setModuleLevel(BCLib::ELOGMODULE_ALL, BCLib::Utility::CLog::ELOGLEVEL_DEBUG);
    BCLib::Utility::CLogFile::singleton().setOutConsole(true);

    //BCLib::Utility::CLogFile::setGlobalLog(&g_globalLog);

    BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "∆Ù∂Ø Server ......");

    std::string configFile = BCLib::Framework::CSysConfig::singleton()["config"];
    std::string currpath = BCLib::Framework::CSysConfig::singleton()["workpath"];

    if (currpath.empty() || currpath == ""){
        currpath = BCLib::Utility::CDirectoryInfo::getExeDirectory();
        BCLib::Framework::CSysConfig::singleton()["workpath"] = currpath;
    }
    if (configFile.empty() || configFile == "") {
        configFile = "accountServer.ini";
        BCLib::Framework::CSysConfig::singleton()["config"] = configFile;
    }
    std::string fullConfig = currpath + "\\";
    fullConfig += configFile;

    BCLib::Network::CNetInit::Stub::setVerifyTimeout(2000);

    BCLib::Utility::CDirectoryInfo::standardization(fullConfig);
    BCLib::Framework::CSysConfig::singleton()["config_full"] = fullConfig;

    bool bret = BCLib::Framework::CSysConfig::singleton().open(fullConfig);
    if (bret == false)
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "º”‘ÿ≈‰÷√Œƒº˛ ß∞‹");
        return -1;
    }
//     if (!CAccountServer::singleton()._init())
//     {
//         BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "º”‘ÿ≈‰÷√Œƒº˛ ß∞‹");
//         return -1;
//     }

//     while(true)
//     {
//         if (CAccountApp::singleton().isRunning())
//         {
//             Sleep(1);
//         }
//         else
//         {
//             break;
//         }
//     }
//
//     CAccountServer::singleton()._final();

    //g_globalLog.close();
    //BCLib::Utility::CLogFile::setGlobalLog(NULL);

    return CAccountServer::singleton()._main();
}

}//AccountServer
}//SSLib
