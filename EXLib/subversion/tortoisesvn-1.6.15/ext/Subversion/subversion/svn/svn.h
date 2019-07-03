//////////////////////////////////////////////////////////////////////
//  created:    2012/02/08
//  filename:   EXLib/subversion/tortoisesvn-1.6.15/ext/Subversion/subversion/svn/svn.h
//  author:     League of Perfect
//  brief
//
//////////////////////////////////////////////////////////////////////
#ifndef __TDLIB_SUBVERSION_SVN_H__
#define __TDLIB_SUBVERSION_SVN_H__

namespace TDLib
{

namespace Subversion
{

#ifdef TDLIB_SVN_EXPORTS
#	define TDLIB_SVN_API _declspec(dllexport)
#elif defined(TDLIB_SVN_IMPORTS)
#	define TDLIB_SVN_API _declspec(dllimport)
#else
#	define TDLIB_SVN_API
#endif

class TDLIB_SVN_API CSvn
{
public:

	CSvn();
	virtual ~CSvn();
	int exe(int argc, const char *argv[]);

};
}//Subversion

}//TDLIB

#endif//__TDLIB_SUBVERSION_SVN_H__