//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/allocator.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_ALLOCATOR_H__
#define __BCLIB_UTILITY_ALLOCATOR_H__

#ifdef __GNUC__
#include <ext/pool_allocator.h>
#include <ext/mt_allocator.h>
#else
#include <xmemory>
#endif

#include <BCLib/utility/baseDef.h>

namespace BCLib
{
namespace Utility
{
#ifdef __GNUC__
typedef __gnu_cxx::__mt_alloc<uint8> CAllocatorUint8;
#else
typedef std::allocator<uint8> CAllocatorUint8;
#endif
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_ALLOCATOR_H__
