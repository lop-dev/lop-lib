//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/netfw/server/mainThread.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_MAINTHREAD_H__
#define __BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_MAINTHREAD_H__

#include <BCLib/framework/mainThread.h>
#include "netServer.h"

class CMainThread : public BCLib::Framework::CMainThread
{
    BCLIB_SINGLETON_DECLARE(CMainThread);
public:
    CMainThread();
    virtual ~CMainThread();

public:
    static int main();

protected:
    virtual bool _init();
    virtual int _final();

private:
    CTcpServer m_netServer;
};

#endif//__BCLIB_SAMPLE_FRAMEWORK_NETFW_SERVER_MAINTHREAD_H__
