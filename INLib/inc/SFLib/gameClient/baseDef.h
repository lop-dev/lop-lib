//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/gameClient/baseDef.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_GAMECLIENT_BASEDEF_H__
#define __SFLIB_GAMECLIENT_BASEDEF_H__

#include <BCLib/utility/baseDef.h>

#if defined(SFLIB_CLIENT_EXPORTS)
#define SFLIB_CLIENT_API _declspec(dllexport)
#define SFLIB_CLIENT_TMP _declspec(dllexport)
#elif defined(SFLIB_CLIENT_IMPORTS)
#define SFLIB_CLIENT_API _declspec(dllimport)
#define SFLIB_CLIENT_TMP
#else
#define SFLIB_CLIENT_API
#define SFLIB_CLIENT_TMP
#endif

#endif//__SFLIB_GAMECLIENT_BASEDEF_H__
