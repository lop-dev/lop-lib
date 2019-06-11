//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/version.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_VERSION_H__
#define __BCLIB_UTILITY_VERSION_H__

#define BCLIB_STRING(x) #x

#ifndef BCLIB_MAJOR_VERSION
#define BCLIB_MAJOR_VERSION "1"
#endif

#ifndef BCLIB_MINOR_VERSION
#define BCLIB_MINOR_VERSION "0"
#endif

#ifndef BCLIB_MICRO_VERSION
#define BCLIB_MICRO_VERSION "0"
#endif

#ifndef BCLIB_DEFINE_VERSION
#define BCLIB_VERSION_TEXT BCLIB_MAJOR_VERSION" . "BCLIB_MINOR_VERSION" . "BCLIB_MICRO_VERSION
#else
#define BCLIB_VERSION_TEXT BCLIB_DEFINE_VERSION
#endif

#endif//__BCLIB_UTILITY_VERSION_H__
