//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SSLib/message/msgTypeSrv.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SSLIB_MESSAGE_MSGTYPESRV_H__
#define __SSLIB_MESSAGE_MSGTYPESRV_H__

#include <BCLib/framework/msgType.h>

namespace SSLib
{

//SSLib 的 EMsgType统一管理
//新类型只能追加，不允许插入类型或删除类型
enum EMsgType
{
    EMT_ACCOUNTSERVER = BCLib::Message::EMT_EXTEND,
    EMT_ACCOUNTCLIENT,
    EMT_EXTEND,
};

}//SSLib

#endif//__SSLIB_MESSAGE_MSGTYPESRV_H__
