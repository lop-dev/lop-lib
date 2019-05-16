//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/baseInc.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_BASEINC_H__
#define __BCLIB_UTILITY_BASEINC_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include <vector>
#include <map>
#include <list>
#include <queue>
#include <set>

// MS VC++ 14.0 _MSC_VER = 1900(VS2015)
// MS VC++ 12.0 _MSC_VER = 1800(VS2013)
// MS VC++ 11.0 _MSC_VER = 1700(VS2012)
// MS VC++ 10.0 _MSC_VER = 1600(VS2010)
// MS VC++ 9.0  _MSC_VER = 1500(VS2008)
// MS VC++ 8.0  _MSC_VER = 1400(VS2005)
// MS VC++ 7.1  _MSC_VER = 1310(VS2003)
// MS VC++ 7.0  _MSC_VER = 1300
// MS VC++ 6.0  _MSC_VER = 1200
// MS VC++ 5.0  _MSC_VER = 1100

#if defined(_WIN32)

#include <windows.h>

#include <unordered_set>
#include <unordered_map>

#elif defined(_LINUX)

#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include <tr1/unordered_set>
#include <tr1/unordered_map>

#else

#error "Unknown platform !!"

#endif

#endif//__BCLIB_UTILITY_BASEINC_H__
