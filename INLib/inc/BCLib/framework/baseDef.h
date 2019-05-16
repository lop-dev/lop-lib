//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/baseDef.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_BASEDEF_H__
#define __BCLIB_FRAMEWORK_BASEDEF_H__

#ifdef BCLIB_FRAMEWORK_EXPORTS
#define BCLIB_FRAMEWORK_API _declspec(dllexport)
#elif defined(BCLIB_FRAMEWORK_IMPORTS)
#define BCLIB_FRAMEWORK_API _declspec(dllimport)
#else
#define BCLIB_FRAMEWORK_API
#endif

#endif//__BCLIB_FRAMEWORK_BASEDEF_H__
