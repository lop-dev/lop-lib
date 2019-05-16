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

typedef BCLib::uint64 PeerID;
const PeerID INVALID_PEER_ID = 0;

typedef ServerID GroupID;
const GroupID INVALID_GROUP_ID = 0;

typedef BCLib::uint64 EntityID;
const EntityID INVALID_ENTITY_ID = 0;

const BCLib::uint8 SFLIB_IP_TEXT_MAX = 63;
}//SFLib

#endif//__SFLIB_COMMONDEFINE_BASEDEF_H__
