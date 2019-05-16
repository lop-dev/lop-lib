//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/crashDump/crashDump.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_CRASHDUMP_CRASHDUMP_H__
#define __MWLIB_CRASHDUMP_CRASHDUMP_H__

#include <windows.h>
#include <DbgHelp.h>
#include <string>

#ifdef MWLIB_CRASHDUMP_EXPORTS
#    define MWLIB_CRASHDUMP_API _declspec(dllexport)
#elif defined(MWLIB_CRASHDUMP_IMPORTS)
#    define MWLIB_CRASHDUMP_API _declspec(dllimport)
#else
#    define MWLIB_CRASHDUMP_API
#endif


namespace MWLib
{
namespace CrashDump
{
typedef std::string (*GetDumpFileNameCallback)(const std::string& moduleName);
typedef std::string (*DumpFileCreatedCallback)(const std::string& dumpFileName);
extern MWLIB_CRASHDUMP_API void SetupCrashDump(GetDumpFileNameCallback getDumpFileNameCb, DumpFileCreatedCallback dumpFileCreatedCb, MINIDUMP_TYPE miniDumpType = MiniDumpNormal);
}//CrashDump
}//MWLib

#endif//__MWLIB_CRASHDUMP_CRASHDUMP_H__
