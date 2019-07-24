//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/thread/mutex.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_THREAD_MUTEX_H__
#define __BCLIB_UTILITY_THREAD_MUTEX_H__

#include <BCLib/utility/baseDef.h>

namespace BCLib
{
namespace Utility
{
class BCLIB_UTILITY_API IMutex
{
public:
    virtual ~IMutex()
    {
    }
    virtual void lock() = 0;
    virtual void unlock() = 0;
};

//进程内部锁
class BCLIB_UTILITY_API CMutex : public IMutex
{
public:
    CMutex();
    virtual ~CMutex();

    virtual void lock();
    virtual void unlock();

private:
    HndMutex m_handle;
};

enum EMutexProcessError
{
    EMPE_ERROR          = -1,
    EMPE_SUCCESS        = 0,
    EMPE_SUCCESS_EXISTS = 1,
};

//进程之间锁
class BCLIB_UTILITY_API CMutexProcess : public IMutex
{
public:
    CMutexProcess();
    virtual ~CMutexProcess();

    EMutexProcessError create(uint32 key);
    void close();

    virtual void lock();
    virtual void unlock();

private:
    HndProcessMutex m_handle;
};

//读写锁
class BCLIB_UTILITY_API CMutexFun
{
public:
    CMutexFun(IMutex* pMutex)
    :m_pMutex(pMutex)
    {
        if(m_pMutex)
        {
            m_pMutex->lock();
        }
    }

    ~CMutexFun()
    {
        if(m_pMutex)
        {
            m_pMutex->unlock();
        }
    }
private:
    IMutex* m_pMutex;
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_THREAD_MUTEX_H__
