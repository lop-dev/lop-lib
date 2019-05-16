//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/security/baseDef.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SECURITY_BASEDEF_H__
#define __BCLIB_SECURITY_BASEDEF_H__

#include <BCLib/utility/baseDef.h>

#ifdef BCLIB_SECURITY_EXPORTS
#    define BCLIB_SECURITY_API _declspec(dllexport)
#elif defined(BCLIB_SECURITY_IMPORTS)
#    define BCLIB_SECURITY_API _declspec(dllimport)
#else
#    define BCLIB_SECURITY_API
#endif

#endif//__BCLIB_SECURITY_BASEDEF_H__
