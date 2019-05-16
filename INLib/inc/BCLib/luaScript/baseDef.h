//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/luaScript/baseDef.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_LUASCRIPT_BASEDEF_H__
#define __BCLIB_LUASCRIPT_BASEDEF_H__

#include <BCLib/utility/baseDef.h>

#ifdef BCLIB_LUASCRIPT_EXPORTS
#    define BCLIB_LUASCRIPT_API _declspec(dllexport)
#elif defined(BCLIB_LUASCRIPT_IMPORTS)
#    define BCLIB_LUASCRIPT_API _declspec(dllimport)
#else
#    define BCLIB_LUASCRIPT_API
#endif

#endif//__BCLIB_LUASCRIPT_BASEDEF_H__
