//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/GeConfig.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_GECONFIG_H__
#define __GELIB_GECONFIG_H__

#ifdef GELIB_EXPORTS
    #define GELIB_API _declspec(dllexport)
#elif defined(GELIB_IMPORTS)
    #define GELIB_API _declspec(dllimport)
#else
    #define GELIB_API
#endif

#if defined(_WIN32)
    #ifdef _XBOX
        #define GELIB_XBOX 1
    #else
        #define GELIB_WINDOWS 1
    #endif
    #define GELIB_WINDOWS_OR_XBOX 1
#elif defined(__linux)
    #define GELIB_LINUX 1
    #define GELIB_LINUX_OR_PS3 1
#elif defined(_PS3)
    #define GELIB_PS3 1
    #define GELIB_LINUX_OR_PS3 1
#elif defined(__APPLE__)
    #define GELIB_IOS 1
#endif


#ifdef _WIN64
	typedef unsigned __int64 WParam;
	typedef __int64 LParam;
#else
	typedef unsigned int WParam;
	typedef long LParam;
#endif

#endif//__GELIB_GECONFIG_H__
