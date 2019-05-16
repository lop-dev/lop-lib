//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/netfw/client/main.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/logFile.h>
#include "mainThread.h"

BCLIB_FW_MAIN(CMainThread::main)

#ifdef WIN32

BOOL WINAPI CtrlCHandler(DWORD CtrlType)
{
    CMainThread::singleton().setValid(false);
    return TRUE;
}

#endif

int CMainThread::main()
{
#ifdef WIN32

    ::SetConsoleCtrlHandler(CtrlCHandler, TRUE);

#endif//WIN32

    BCLib::Utility::CLogFile::singleton().setOutFile("netFwClient.log");
    BCLib::Utility::CLogFile::singleton().setModuleLevel(BCLib::ELOGMODULE_ALL, BCLib::Utility::CLog::ELOGLEVEL_DEBUG);
    BCLib::Utility::CLogFile::singleton().setOutConsole(true);

    CMainThread::singleton()._main();

    return 0;
}