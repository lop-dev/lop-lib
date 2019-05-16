//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/netfw/client/mainThread.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_FRAMEWORK_NETFW_CLIENT_MAINTHREAD_H__
#define __BCLIB_SAMPLE_FRAMEWORK_NETFW_CLIENT_MAINTHREAD_H__

#include <BCLib/framework/mainThread.h>

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
    BCLib::ResThread _inputCommand(void* param);

    BCLib::Utility::CHndThread m_inputThread;
};

#endif//__BCLIB_SAMPLE_FRAMEWORK_NETFW_CLIENT_MAINTHREAD_H__
