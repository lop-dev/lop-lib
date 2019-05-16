//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/thread/threadTask.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_THREAD_THREADTASK_H__
#define __BCLIB_UTILITY_THREAD_THREADTASK_H__

#include <BCLib/utility/task.h>
#include <BCLib/utility/thread/thread.h>

namespace BCLib
{
namespace Utility
{
//////////////////////////////////////////////////////////////////////////
/// @brief 带线程特性的任务队列
//////////////////////////////////////////////////////////////////////////
class BCLIB_UTILITY_API CThreadTaskQueue : public CThread, public CTaskQueue
{
public:
    CThreadTaskQueue()
    {
    }
    virtual ~CThreadTaskQueue()
    {
    }

    virtual void terminate()
    {
        m_isRunning = false;
    }

    virtual bool isReady()
    {
        return m_isRunning;
    }

protected:
    virtual void _run()
    {
        m_isRunning = true;
        while(m_isRunning)
        {
            CTaskSPtr task = CTaskQueue::_popTask();
            if(task == NULL)
            {
                CThread::msleep(50);
                continue;
            }
            _execute(task);
        }
    }

    virtual void _execute(CTaskSPtr task)
    {
        task->executeTask();
    }

private:
    bool m_isRunning;
};


//////////////////////////////////////////////////////////////////////////
/// @brief
//////////////////////////////////////////////////////////////////////////
class BCLIB_UTILITY_API CThreadTaskPool : public CTaskPool
{
public:
    CThreadTaskPool()
    {
    }
    virtual ~CThreadTaskPool()
    {
    }

    void addThreadTaskQueue(const CThreadTaskQueue* threadTaskQueue)
    {
        CTaskPool::addQueue(threadTaskQueue);
    }
};
}//Utility
}//BCLib
#endif//__BCLIB_UTILITY_THREAD_THREADTASK_H__
