//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/netfw/client/mainThread.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include "mainThread.h"
#include "command.h"

CCommand g_command;

BCLIB_SINGLETON_DEFINE(CMainThread);

CMainThread::CMainThread()
{
    ;
}

CMainThread::~CMainThread()
{
    ;
}

BCLib::ResThread CMainThread::_inputCommand(void* param)
{
    char commandBuff[1024];
    while(true)
    {
        printf("\n>");
        gets_s(commandBuff, sizeof(commandBuff));
        int res = g_command.parseCommand(commandBuff, NULL);
        if(res == 0)
        {
            continue;
        }
        if(res == 1)
        {
            break;
        }
    }
    return BCLIB_THREAD_RES(0);
}

bool CMainThread::_init()
{
    g_command.init(this);

    m_inputThread = BCLib::Utility::CHndThread::createThread(&CMainThread::_inputCommand, this);

    return BCLib::Framework::CMainThread::_init();
}

int CMainThread::_final()
{
    m_inputThread.cancel();

    g_command.close();

    return BCLib::Framework::CMainThread::_final();
}