//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/mainThread.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_MAINTHREAD_H__
#define __BCLIB_FRAMEWORK_MAINTHREAD_H__

#include <BCLib/framework/threadFrame.h>
#include <BCLib/framework/sysConfig.h>
#include <BCLib/framework/subThread.h>

namespace BCLib
{
namespace Framework
{
class BCLIB_FRAMEWORK_API CMainThread : public CThreadFrame
{
public:
    CMainThread();
    virtual ~CMainThread();

public:
    virtual bool isMainThread()
    {
        return true;
    }

public:
    static CMainThread& getMainThread()
    {
        return *m_pMainThread;
    }

protected:
    virtual bool _init();
    virtual int _final();
    virtual bool _callback();

    bool _addSubThread(CSubThread* subThread);
    bool _removeSubThread(CSubThread* subThread);

    void _setSysKey(uint16 sysKey)
    {
        m_sysKey = sysKey;
    }

private:
    uint16 m_sysKey;
    std::vector<CSubThread*> m_subThreadList;

private:
    static CMainThread* m_pMainThread;
};
}//Framework
}//BCLib

#ifdef _WINDOWS
#    define BCLIB_FW_MAIN(funMain) int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)\
    {\
        BCLib::Framework::CSysConfig::singleton().initMain(lpCmdLine);\
        return funMain();\
    }
#elif defined(_CONSOLE) || defined(_LINUX)
#    define BCLIB_FW_MAIN(funMain) int main(int argc, char* argv[])\
    {\
        BCLib::Framework::CSysConfig::singleton().initMain(argc, argv);\
        return funMain();\
    }
#endif


#endif//__BCLIB_FRAMEWORK_MAINTHREAD_H__
