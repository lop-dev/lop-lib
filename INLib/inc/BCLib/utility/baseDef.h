//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/baseDef.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_BASEDEF_H__
#define __BCLIB_UTILITY_BASEDEF_H__

#include <BCLib/utility/baseInc.h>

#if defined(_WIN32)

#pragma warning(disable:4100)
#pragma warning(disable:4127)
#pragma warning(disable:4251)
#pragma warning(disable:4244)
#pragma warning(disable:4245)
#pragma warning(disable:4189)
#pragma warning(disable:4200)
#pragma warning(disable:4102)
#pragma warning(disable:4018)
#pragma warning(disable:4389)
#pragma warning(disable:4800)
#pragma warning(disable:4702)
#pragma warning(disable:4067)
#pragma warning(disable:4091)

#elif defined(_LINUX)

#endif

namespace BCLib
{
typedef int int32;
typedef unsigned int uint32;

typedef short int16;
typedef unsigned short uint16;

typedef char int8;
typedef unsigned char uint8;

#if defined(_WIN32)

typedef __int64 int64;
typedef unsigned __int64 uint64;

typedef DWORD ResThread;
typedef HANDLE HndThread;
typedef CRITICAL_SECTION HndMutex;
typedef HANDLE HndProcessMutex;

typedef ResThread(*THREAD_START_FUN)(void*);

typedef    DWORD HndThdMsgQueue;
const HndThdMsgQueue INVALID_HNDTHDMSGQUEUE = 0;
//typedef uint64    HndTimer;

#elif defined(_LINUX)

typedef long long int64;
typedef unsigned long long uint64;

typedef void* ResThread;
typedef unsigned long int HndThread;
typedef pthread_mutex_t HndMutex;
typedef int HndProcessMutex;

typedef ResThread(*THREAD_START_FUN)(void*);

typedef int HndThdMsgQueue;
const HndThdMsgQueue INVALID_HNDTHDMSGQUEUE = -1;
//typedef uint64    HndTimer;

#endif
}//BCLib

#if defined(_WIN32)

#define BCLIB_LO_SHIFT( x, y )   ( (x) >> (y) )
#define BCLIB_HI_SHIFT( x, y )   ( (x) << (y) )

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#define BCLIB_INT64_C(val)  (val##LL)
#define BCLIB_UINT64_C(val) (val##ULL)

#define BCLIB_INVALID_HNDTHREAD NULL

#define BCLibSprintf _snprintf_s

#elif defined(_LINUX)

#define BCLIB_LO_SHIFT( x, y )   ( (x) >> (y) )
#define BCLIB_HI_SHIFT( x, y )   ( (x) << (y) )

#define BCLIB_INT64_C(val)    (val##i64)
#define BCLIB_UINT64_C(val)    (val##ui64)

#define BCLIB_INVALID_HNDTHREAD 0

#define BCLibSprintf snprintf

#else

#error "Unknown platform !!"

#endif

#define BCLIB_INT8_MIN      (-0x7f - 1)
#define BCLIB_INT8_MAX      (0x7f)
#define BCLIB_UINT8_MAX     (0xff)
#define BCLIB_INT16_MIN     (-0x7fff - 1)
#define BCLIB_INT16_MAX     (0x7fff)
#define BCLIB_UINT16_MAX    (0xffff)
#define BCLIB_INT32_MIN     (-0x7fffffff - 1)
#define BCLIB_INT32_MAX     (0x7fffffff)
#define BCLIB_UINT32_MAX    (0xffffffff)
#define BCLIB_INT64_MIN     (INT64_C(-0x7fffffffffffffff) - 1)
#define BCLIB_INT64_MAX     INT64_C(0x7fffffffffffffff)
#define BCLIB_UINT64_MAX    INT64_C(0xffffffffffffffff)

#define BCLIB_THREAD_RES(x) ((BCLib::ResThread)(x))

#ifndef BCLIB_SAFE_FREE
#define BCLIB_SAFE_FREE(x)          do { if(x) { free (x); (x) = NULL; } } while(false)
#endif

#ifndef BCLIB_SAFE_DELETE
#define BCLIB_SAFE_DELETE(x)        do { if(x) { delete (x); (x) = NULL; } } while(false)
#endif

#ifndef BCLIB_SAFE_DELETE_ARRAY
#define BCLIB_SAFE_DELETE_ARRAY(x)  do { if(x) {delete [] x; x = NULL;} } while(false)
#endif

#define BCLIB_ABS(f) ( (f)>0 ? (f) : -(f) )
#define BCLIB_MAX(f1, f2) ( (f1)>(f2) ? (f1) : (f2) )
#define BCLIB_MIN(f1, f2) ( (f1)>(f2) ? (f2) : (f1) )

#ifdef BCLIB_UTILITY_EXPORTS
#define BCLIB_UTILITY_API _declspec(dllexport)
#elif defined(BCLIB_UTILITY_IMPORTS)
#define BCLIB_UTILITY_API _declspec(dllimport)
#else
#define BCLIB_UTILITY_API
#endif

#if defined(_WIN32)
#define BCLIB_DLL_EXPORT extern "C" _declspec(dllexport)
#elif defined(_LINUX)
#define BCLIB_DLL_EXPORT extern "C"
#endif

#endif//__BCLIB_UTILITY_BASEDEF_H__
