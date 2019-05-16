//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/io/dllFile.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_IO_DLLFILE_H__
#define __BCLIB_UTILITY_IO_DLLFILE_H__

#include <BCLib/utility/baseDef.h>

namespace BCLib
{
namespace Utility
{
//////////////////////////////////////////////////////////////////////////
/// @brief 对动态库文件的操作
//////////////////////////////////////////////////////////////////////////
class BCLIB_UTILITY_API CDllFile
{
public:
    CDllFile();
    ~CDllFile();
    bool open(const char* dllFile);
    /// @brief 获取函数地址
    /// @return void*
    /// @param funName
    void* getFun(const char* funName);
    void close();

private:
    //加载进进程的地址
    void* m_dllFile;
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_IO_DLLFILE_H__
