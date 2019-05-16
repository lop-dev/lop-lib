//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/msgQueue.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_MSGQUEUE_H__
#define __BCLIB_FRAMEWORK_MSGQUEUE_H__

#include <BCLib/utility/thread/mutex.h>
#include <BCLib/utility/queue.h>
#include <BCLib/utility/singleton.h>
#include <BCLib/framework/message.h>

namespace BCLib
{
namespace Framework
{
class CMsgQueueCallback
{
public:
    virtual ~CMsgQueueCallback()
    {
    }

    virtual bool parse(const SMessage* msg, const BCLib::uint32 msgSize) = 0;
};

class CMsgQueue
{
    BCLIB_SINGLETON_DECLARE(CMsgQueue);

public:
    CMsgQueue();
    virtual ~CMsgQueue();

    void push(const SMessage* netMessage, const BCLib::uint32 netMessageSize);
    bool get(SMessage** netMessage, BCLib::uint32* netMessageSize);
    void pop();
    void doParse(CMsgQueueCallback& cb);

private:
    BCLib::Utility::CMutex m_mutex;
    BCLib::Utility::CQueue<SMessage> m_queueChar;
};
}//Framework
}//BCLib

#endif//__BCLIB_FRAMEWORK_MSGQUEUE_H__
