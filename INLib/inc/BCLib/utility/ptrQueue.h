//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/ptrQueue.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_PTRQUEUE_H__
#define __BCLIB_UTILITY_PTRQUEUE_H__

#include <BCLib/utility/atomic.h>

namespace BCLib
{
namespace Utility
{
//////////////////////////////////////////////////////////////////////////
/// @brief FIFO队列实现，实现一个简单的缓存，如果是单线程写单线程读则是 线程安全，
///        否则 线程不安全
//////////////////////////////////////////////////////////////////////////
template<typename TYPE>
class CPtrQueue
{
private:
    struct SQueueItem
    {
        SQueueItem* m_next;
        TYPE m_data;

        SQueueItem()
        {
            m_next = NULL;
            m_data = NULL;
        }
    };

public:
    CPtrQueue()
    {
        m_headItem = new SQueueItem();
        m_endItem = m_headItem;

        m_headRecycle = new SQueueItem();
        m_endRecycle = m_headRecycle;

        m_size = 0;
    }

    virtual ~CPtrQueue()
    {
        clear();
        if(m_headRecycle != NULL)
        {
            delete m_headRecycle;
        }
        if(m_headItem != NULL)
        {
            delete m_headItem;
        }
        m_headItem = NULL;
        m_endItem = NULL;

        m_headRecycle = NULL;
        m_endRecycle = NULL;
    }

    /// @brief 首先尝试从缓存获取空间（如果失败则new）
    ///
    /// @return bool 成功与否
    /// @param data
    bool push(const TYPE& data)
    {
        if(m_headItem != NULL)
        {
            SQueueItem* item = NULL;
            if(m_endRecycle != NULL && m_endRecycle->m_next != NULL)
            {
                item = m_endRecycle;
                m_endRecycle = m_endRecycle->m_next;
            }
            if(item == NULL)
            {
                item = new SQueueItem();
                if(item == NULL)
                {
                    return false;
                }
            }
            item->m_next = NULL;
            item->m_data = NULL;
            m_headItem->m_data = data;
            m_headItem->m_next = item;
            m_headItem = item;
            m_size.increment();
            return true;
        }
        return false;
    }

    /// @brief 从队列中获取第一个item，如果队列为空，返回null
    /// @return TYPE
    TYPE get()
    {
        TYPE data = NULL;
        if(m_endItem != NULL && m_endItem->m_next != NULL)
        {
            data = m_endItem->m_data;
        }
        return data;
    }

    /// @brief 从队列中弹出第一个item，如果队列为空，返回null
    /// @return TYPE
    TYPE pop()
    {
        TYPE data = NULL;
        if(m_endItem != NULL && m_endItem->m_next != NULL)
        {
            SQueueItem* item = m_endItem;
            m_endItem = m_endItem->m_next;
            data = item->m_data;

            item->m_next = NULL;
            item->m_data = NULL;

            m_headRecycle->m_next = item;
            m_headRecycle = item;
            m_size.decrement();
        }
        return data;
    }

    bool empty()
    {
        return m_endItem == NULL || m_endItem->m_next == NULL;
    }

    void clear()
    {
        SQueueItem* item = NULL;

        while(m_endItem != NULL && m_endItem->m_next != NULL)
        {
            item = m_endItem;
            m_endItem = m_endItem->m_next;
            delete item;
        }

        while(m_endRecycle != NULL && m_endRecycle->m_next != NULL)
        {
            item = m_endRecycle;
            m_endRecycle = m_endRecycle->m_next;
            delete item;
        }
        m_size = 0;
    }

    int32 size()
    {
        return m_size.getValue();
    }

private:
    /// 队列列尾
    SQueueItem* m_headItem;
    /// 队列列头
    SQueueItem* m_endItem;

    /// 回收空间队列 列尾
    SQueueItem* m_headRecycle;
    /// 回收空间队列 列头
    SQueueItem* m_endRecycle;

    /// 对列当前长度
    CAtomic m_size;
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_PTRQUEUE_H__
