//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/msgType.h
//  author:     League of Perfect
/// @brief      EMID_XX2XX_REQ_*
///             EMID_XX2XX_RES_*
///             EMID_XX2XX_NTF_*
///             gameClient : GC
///             gateServer : GS
///             p2spServer : PS
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_MSGTYPE_H__
#define __BCLIB_FRAMEWORK_MSGTYPE_H__

#include <BCLib/framework/message.h>

namespace BCLib
{
namespace Message
{
//消息类型定义
enum EMsgType
{
    //framework消息类型定义
    EMT_FRAMEWORK   = 1,
    //////////////////////////////////////////////////////////////////////////
    //游戏逻辑消息类型
    //////////////////////////////////////////////////////////////////////////
    //P2p服务
    EMT_P2P,
    //游戏客户端
    EMT_SAMPLE,
    EMT_EXTEND      = 256,
};
}//Message
}//BCLib

#define BCLIB_MSG_BUFF_MAX (1024 * 64)

#define BCLIB_DEFINE_OBJECT(type, param) \
    char __tmepBuff##param [BCLIB_MSG_BUFF_MAX+1024]={0};\
    param = (type*)__tmepBuff##param;\
    new (static_cast<void*>(param)) type();

#endif//__BCLIB_FRAMEWORK_MSGTYPE_H__
