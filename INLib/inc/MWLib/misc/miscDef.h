//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/misc/miscDef.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_MISC_MISCDEF_H__
#define __MWLIB_MISC_MISCDEF_H__

#include <BCLib/utility/baseDef.h>


#ifdef MWLIB_MISC_EXPORTS
#    define MWLIB_MISC_API _declspec(dllexport)
#elif defined(MWLIB_MISC_IMPORTS)
#    define MWLIB_MISC_API _declspec(dllimport)
#else
#    define MWLIB_MISC_API
#endif


namespace MWLib
{
namespace Misc
{

}//Misc
}//MWLib


#endif//__MWLIB_MISC_MISCDEF_H__
