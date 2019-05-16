//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/thread/thdMsgQueue.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_THREAD_THDMSGQUEUE_H__
#define __BCLIB_UTILITY_THREAD_THDMSGQUEUE_H__

#include <BCLib/utility/baseDef.h>

namespace BCLib
{
namespace Utility
{
class BCLIB_UTILITY_API CThdMsgQueue
{
protected:
    CThdMsgQueue()
    {
    }
    virtual ~CThdMsgQueue()
    {
    }

public:
    static bool create(HndThdMsgQueue& queHandle);
    static void remove(HndThdMsgQueue queHandle);

    //同步获取消息
    // 0< 异常
    // >0 收到消息
    static int recv(HndThdMsgQueue queHandle, uint32* para1, uint16* para2, void** para3);
    // =0 没有收到消息
    static int recvAsync(HndThdMsgQueue queHandle, uint32* para1, uint16* para2, void** para3);

    //异步发送消息
    static bool send(HndThdMsgQueue queHandle, uint32 para1, uint16 para2, void* para3);
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_THREAD_THDMSGQUEUE_H__
