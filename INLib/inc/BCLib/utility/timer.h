//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/timer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_TIMER_H__
#define __BCLIB_UTILITY_TIMER_H__

#include <BCLib/utility/function.h>
#include <BCLib/utility/hashMap.h>
#include <BCLib/utility/ptrQueue.h>
#include <BCLib/utility/pointer.h>
#include <BCLib/utility/dateTime.h>

namespace BCLib
{
namespace Utility
{
class CLinkList;
class CTimer;
class CTimerServer;

struct SListHead
{
    SListHead() : m_prev(NULL), m_next(NULL){}
    SListHead* m_prev;
    SListHead* m_next;
};

struct SListNode : public SListHead
{
    SListNode() : m_timer(NULL) {}
    CTimer* m_timer;
};

//////////////////////////////////////////////////////////////////////////
/// @brief 定时器
//////////////////////////////////////////////////////////////////////////
class BCLIB_UTILITY_API CTimer
{
public:
    typedef void (*FTimerCbCallback)(CTimer* timer, void* param);

public:
    /// @brief
    /// @return
    /// @param timerServer 每一个定时器必需指定一个timerserver
    /// @param msIterval 执行间隔
    /// @param msDelay 延迟时间
    /// @param counts 执行次数
    CTimer(CTimerServer* timerServer, BCLib::uint32 msIterval, uint32 msDelay, BCLib::uint32 counts = -1);
    virtual ~CTimer();

    /// @brief 启动定时器
    /// @return bool
    bool start();
    /// @brief 取消定时器
    /// @return bool
    bool cancel();

    /// @brief 定时器的回调函数
    /// @return bool
    /// @param fun
    /// @param param
    bool setTriggerCb(FTimerCbCallback fun, void* param);
    bool setTriggerCb(const CFunction<void, CTimer*, void*>& fun, void* param);

    template<class NAME>
    bool setTriggerCb(void(NAME::* fun)(CTimer*, void*), NAME* object, void* param)
    {
        return setTriggerCb(CFunctionObject<void, NAME, CTimer*, void*>(fun, object), param);
    }

    /// @brief 设置延迟时间
    /// @return void
    /// @param msDelay
    void setDelay(uint32 msDelay);
    void setOneTouch(bool oneTouch = true);

    void initLink()
    {
        m_entry.m_next = m_entry.m_prev = NULL; m_entry.m_timer = this;
    }//&m_entry;}

public:
    virtual void _touchCallback(CTimer* timer, void* param);

private:
    void _execute();

private:
    CFunction<void, CTimer*, void*>* m_fun;
    void* m_para;

    CTimerServer* m_timerServer;

    SListNode m_entry;
    BCLib::uint32 m_itervalMS;
    BCLib::uint32 m_delayMS;
    BCLib::uint32 m_counts;
    BCLib::uint32 m_remainCounts;

    friend class CTimerServer;
};

class BCLIB_UTILITY_API CTimerServer
{
public:
    CTimerServer();
    virtual ~CTimerServer();

    bool addTimer(CTimer*);
    bool delTimer(CTimer* timer);

    bool isTimerExist(CTimer* timer);
    /// @brief 更新定时器
    /// @return void
    void synRun();
protected:
    void cascadeTimer();
    int _cascadeTimer(CLinkList* list);
    bool _addTimerInteral(CTimer* timer, BCLib::uint32 expires);
    void _eraseTimer(CTimer* timer);

    void _copyRun(SListHead* head);
    void _adjustExistTimer(SListHead* head);

private:
    CLinkList* m_tv1;
    CLinkList* m_tv2;
    CSteadyTime m_steadyTime;
    friend class CTimer;
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_TIMER_H__
