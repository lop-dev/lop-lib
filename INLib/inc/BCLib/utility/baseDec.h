//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/baseDec.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_BASEDEC_H__
#define __BCLIB_UTILITY_BASEDEC_H__

#include <BCLib/utility/baseInc.h>


namespace BCLib
{
namespace Utility
{
class CStringW;
class CStringA;

class CXmlFile;
class CShell;
}//Utility

#ifdef _UNICODE
typedef Utility::CStringW CString;
#else
typedef Utility::CStringA CString;
#endif
}//BCLib


#endif//__BCLIB_UTILITY_BASEDEC_H__
