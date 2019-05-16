//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/thread/condition.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_THREAD_CONDITION_H__
#define __BCLIB_UTILITY_THREAD_CONDITION_H__

#include <BCLib/utility/thread/mutex.h>
#include <BCLib/utility/thread/semaphore.h>

namespace BCLib
{
namespace Utility
{
class CCondition
{
public:
    explicit CCondition();
    virtual ~CCondition();

    void wait(CMutex& m);
    void notify();
    void notifyAll();

private:
#if defined(_WIN32)
    CSemaphore _sema;
    long _waiters;
#elif defined(_LINUX)
    pthread_cond_t _cond;
    pthread_mutex_t _mutex;
#endif
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_THREAD_CONDITION_H__
