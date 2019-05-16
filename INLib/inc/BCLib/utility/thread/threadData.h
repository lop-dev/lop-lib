//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/thread/threadData.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_THREAD_THREADDATA_H__
#define __BCLIB_UTILITY_THREAD_THREADDATA_H__

#include <BCLib/utility/thread/mutex.h>

namespace BCLib
{
namespace Utility
{
class BCLIB_UTILITY_API CThreadData
{
public:
    CThreadData();
    ~CThreadData();

    void set(void* data);
    void* get();

private:
#if defined(_WIN32)

    uint32 m_dataHandle;

#elif defined(_LINUX)

    pthread_key_t m_dataHandle;

#endif

    bool m_isOk;
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_THREAD_THREADDATA_H__
