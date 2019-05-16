//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/database/baseDef.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_DATABASE_BASEDEF_H__
#define __BCLIB_DATABASE_BASEDEF_H__

#include <BCLib/utility/baseDef.h>

#ifdef BCLIB_DATABASE_EXPORTS
#    define BCLIB_DATABASE_API _declspec(dllexport)
#elif defined(BCLIB_DATABASE_IMPORTS)
#    define BCLIB_DATABASE_API _declspec(dllimport)
#else
#    define BCLIB_DATABASE_API
#endif

#endif//__BCLIB_DATABASE_BASEDEF_H__
