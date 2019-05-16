//////////////////////////////////////////////////////////////////////
//  created:    2012/08/01
//  filename:   BCLib/framework/netfw/server/main.cpp
//  author:     aaron
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/logFile.h>
#include "mainThread.h"

#pragma comment(lib,"Advapi32")

//BCLIB_FW_MAIN(CMainThread::main)

#ifdef WIN32

 BOOL WINAPI CtrlCHandler(DWORD CtrlType)
 {
     CMainThread::singleton().setValid(false);
     return TRUE;
 }

#endif

BCLib::Utility::CLogFile g_globalLog;

void CMainThread::main()
{
#ifdef WIN32

    //::SetConsoleCtrlHandler(CtrlCHandler, TRUE);

#endif//WIN32

    g_globalLog.setOutFile("netFwService.log");
    g_globalLog.setModuleLevel(BCLib::ELOGMODULE_ALL, BCLib::Utility::CLog::ELOGLEVEL_DEBUG);
    g_globalLog.setOutConsole(true);

    BCLib::Utility::CLogFile::setGlobalLog(&g_globalLog);

    BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "启动 Server ......");

    CMainThread::singleton()._main();

    g_globalLog.close();
    BCLib::Utility::CLogFile::setGlobalLog(NULL);
    
}

//////////////////////////////////////////////////////////////////////////

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

void ControlHandler(DWORD request)
{
    switch(request)
    {
    case SERVICE_CONTROL_STOP:
        ServiceStatus.dwWin32ExitCode = 0;
        ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hStatus,&ServiceStatus);
        return;
    case SERVICE_CONTROL_SHUTDOWN:
        ServiceStatus.dwWin32ExitCode = 0;
        ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hStatus,&ServiceStatus);
        return;
    default:
        break;
    }
    SetServiceStatus(hStatus,&ServiceStatus);
    return;
}

void ReconfigureService(LPSTR lpServiceName, LPSTR lpDesc)
{
    SC_HANDLE schSCManager = ::OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
    if (schSCManager)
    {
        SC_LOCK scLock = LockServiceDatabase(schSCManager);
        if (scLock)
        {
            SC_HANDLE schService = OpenService(schSCManager,lpServiceName,SERVICE_CHANGE_CONFIG);
            if (schService)
            {
                SERVICE_DESCRIPTION sdBuf;
                sdBuf.lpDescription = lpDesc;
                if (ChangeServiceConfig2(schService,SERVICE_CONFIG_DESCRIPTION,&sdBuf))
                {
                    ;
                }
                CloseServiceHandle(schService);
            }
            UnlockServiceDatabase(scLock);
        }
        CloseServiceHandle(schSCManager);
    }
}

void ServiceMain(int argc,char* argv[])
{
    ServiceStatus.dwServiceType = SERVICE_WIN32;
    ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    ServiceStatus.dwWin32ExitCode = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint = 0;
    ServiceStatus.dwWaitHint = 0;
    hStatus = RegisterServiceCtrlHandler("FrameworkService",(LPHANDLER_FUNCTION)ControlHandler);
    if (hStatus == (SERVICE_STATUS_HANDLE)0)
    {
        return;
    }
    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(hStatus,&ServiceStatus);
    ReconfigureService("FrameService","这就是描述了！！！嗯，就这样子！");
    CMainThread::main();
}

int main(int argc,char* argv[])
{
    BCLib::Framework::CSysConfig::singleton().initMain(argc, argv);
    SERVICE_TABLE_ENTRY ServiceTable[2];
    ServiceTable[0].lpServiceName = "FrameworkService";
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
    
    ServiceTable[1].lpServiceName = NULL;
    ServiceTable[1].lpServiceProc = NULL;
    
    StartServiceCtrlDispatcher(ServiceTable);
    
}