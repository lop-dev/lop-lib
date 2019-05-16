//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/io/exeFile.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_IO_EXEFILE_H__
#define __BCLIB_UTILITY_IO_EXEFILE_H__

#if defined(_WIN32)

#include <BCLib/utility/baseDef.h>

namespace BCLib
{
namespace Utility
{
class BCLIB_UTILITY_API CExeFile
{
public:
    static bool embedResource(const std::string& exePath, const std::string& resPath, int resID);
    static bool extractResource(const std::string& exePath, const std::string& resPath, int resID);

private:
};
}//Utility
}//BCLib

#endif//_WIN32

#endif//__BCLIB_UTILITY_IO_EXEFILE_H__
