//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/thread/semaphore.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_THREAD_SEMAPHORE_H__
#define __BCLIB_UTILITY_THREAD_SEMAPHORE_H__

#if defined(_WIN32)
#elif defined(_LINUX)
#include <semaphore.h>
#endif

#include <BCLib/utility/baseInc.h>

namespace BCLib
{
namespace Utility
{
class CSemaphore
{
public:
    CSemaphore(int n, int max);
    virtual ~CSemaphore();

    void wait();
    bool wait(long miliseconds);
    void post();

private:

#if defined(_WIN32)

    HANDLE hSemaphore;

#elif defined(_LINUX)

    sem_t hSemaphore;

#endif
};
}//Utility
}//BCLib
#endif//__BCLIB_UTILITY_THREAD_SEMAPHORE_H__
