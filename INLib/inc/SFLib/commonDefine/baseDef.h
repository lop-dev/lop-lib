//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonDefine/baseDef.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONDEFINE_BASEDEF_H__
#define __SFLIB_COMMONDEFINE_BASEDEF_H__

#include <BCLib/utility/baseDef.h>

#ifdef SFLIB_EXPORTS
#    define SFLIB_SF_API _declspec(dllexport)
#elif defined(SFLIB_IMPORTS)
#    define SFLIB_SF_API _declspec(dllimport)
#else
#    define SFLIB_SF_API
#endif

namespace SFLib
{
typedef BCLib::uint8 ServerType;
const ServerType INVALID_SERVER_TYPE = 0;

typedef BCLib::uint16 ServerID;
const ServerID INVALID_SERVER_ID = 0;

typedef BCLib::uint64 GroupID;
const GroupID INVALID_GROUP_ID = 0;

typedef BCLib::uint64 PeerID;
const PeerID INVALID_PEER_ID = 0;

typedef BCLib::uint64 EntityID;
const EntityID INVALID_ENTITY_ID = 0;

const BCLib::uint8 SFLIB_IP_TEXT_MAX = 63;

enum EPeerEnterReason
{
    EPEERENTERREASON_UNKNOW,        // 未知原因
    EPEERENTERREASON_LOGIN,         // 登录进入
    EPEERENTERREASON_RECONNECT,     // 重连进入
};

enum EPeerLeaveReason
{
    EPEERLEAVEREASON_UNKNOW,        // 未知原因
    EPEERLEAVEREASON_CLEAR_PLAYERS, // 清理玩家
    EPEERLEAVEREASON_TOP_OFF,       // 登录顶号
    EPEERLEAVEREASON_LOG_OUT,       // 主动退出
    EPEERLEAVEREASON_KICK_PLAYER,   // 管理踢人
    EPEERLEAVEREASON_EXTEND,        // 上层扩展
};

}//SFLib

#endif//__SFLIB_COMMONDEFINE_BASEDEF_H__
