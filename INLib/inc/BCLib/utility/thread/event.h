//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/thread/event.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_THREAD_EVENT_H__
#define __BCLIB_UTILITY_THREAD_EVENT_H__

#include <BCLib/utility/baseDef.h>

namespace BCLib
{
namespace Utility
{
class BCLIB_UTILITY_API CEvent
{
public:
    CEvent(bool manualReset = false);
    ~CEvent();

    void set();
    void reset();
    int wait(uint32 timeout = (uint32) - 1);

private:
    bool m_manualReset;

#if defined(_WIN32)
    HANDLE m_event;
#elif defined(_LINUX)
    pthread_mutex_t m_mutex;
    bool m_isSet;
    pthread_cond_t m_cond;
#endif
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_THREAD_EVENT_H__
