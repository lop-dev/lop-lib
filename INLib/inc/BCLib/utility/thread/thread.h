//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/thread/thread.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_THREAD_THREAD_H__
#define __BCLIB_UTILITY_THREAD_THREAD_H__

#include <BCLib/utility/pointer.h>
#include <BCLib/utility/thread/mutex.h>
#include <BCLib/utility/noncopyable.h>
#include <BCLib/utility/function.h>

namespace BCLib
{
namespace Utility
{
//////////////////////////////////////////////////////////////////////////
/// @brief 线程类，对线程操作的封装，通过createThread创建线程
//////////////////////////////////////////////////////////////////////////
class BCLIB_UTILITY_API CHndThread
{
public:
    CHndThread()
    :m_thdHandle(BCLIB_INVALID_HNDTHREAD)
    {
    }
    CHndThread(const HndThread& thdHandle)
    :m_thdHandle(thdHandle)
    {
    }
    CHndThread(const CHndThread& thdHandle)
    :m_thdHandle(thdHandle.m_thdHandle)
    {
    }

    void join();
    void cancel();

    bool isValid()
    {
        return m_thdHandle != BCLIB_INVALID_HNDTHREAD;
    }

    CHndThread operator =(const HndThread& thdHandle)
    {
        m_thdHandle = thdHandle; return *this;
    }
    CHndThread operator =(const CHndThread& thdHandle)
    {
        m_thdHandle = thdHandle.m_thdHandle;    return *this;
    }

    bool operator ==(const HndThread& thdHandle) const
    {
        return m_thdHandle == thdHandle;
    }
    bool operator ==(const CHndThread& thdHandle) const
    {
        return m_thdHandle == thdHandle.m_thdHandle;
    }

    bool operator !=(const HndThread& thdHandle) const
    {
        return m_thdHandle != thdHandle;
    }
    bool operator !=(const CHndThread& thdHandle) const
    {
        return m_thdHandle != thdHandle.m_thdHandle;
    }

public:
    static CHndThread createThread(THREAD_START_FUN funThread, void* pVoid = NULL, uint32 _stackSize = 0);
    static CHndThread createThread(const CFunction<ResThread, void*>& fun, void* pVoid = NULL, uint32 _stackSize = 0);

    template<class NAME>
    static CHndThread createThread(ResThread(NAME::* fun)(void*), NAME* object, void* pVoid = NULL, uint32 _stackSize = 0)
    {
        return createThread(CFunctionObject<ResThread, NAME, void*>(fun, object), pVoid, _stackSize);
    }

protected:
    HndThread m_thdHandle;
};

//////////////////////////////////////////////////////////////////////////
/// @brief 线程类，对线程操作的封装，纯虚类，通过继承_run实现线程实体
//////////////////////////////////////////////////////////////////////////
class BCLIB_UTILITY_API CThread : private CNoncopyable
{
public:
    enum EThreadState
    {
        E_NULL,
        E_READY,
        E_RUNNING,
    };

public:
    CThread();
    virtual ~CThread();

	virtual bool start();
	virtual void join();
	virtual void cancel();

    EThreadState getState()
    {
        return m_state;
    }
    CHndThread& getHndThread()
    {
        return m_thdHandle;
    }

public:
    static void sleep(uint32 sec);
    static void msleep(uint32 msec);

protected:
    virtual void _run() = 0;

private:
    static ResThread _threadRunFun(void* pVoid);

private:
    EThreadState m_state;
    CHndThread m_thdHandle;
    CMutex m_mutex;
};

//////////////////////////////////////////////////////////////////////////
/// @brief 线程数组
//////////////////////////////////////////////////////////////////////////
class BCLIB_UTILITY_API CThreadList : private CNoncopyable
{
public:
    CThreadList();
    virtual ~CThreadList();

    bool addThread(CThread* threadPtr);
    CThread* getThread(uint32 index);
    uint32 getSize();
    void joinAll();

    CThread* operator[](uint32 index);

private:
    std::vector<CThread*> m_threadList;
    CMutex m_mutex;
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_THREAD_THREAD_H__
