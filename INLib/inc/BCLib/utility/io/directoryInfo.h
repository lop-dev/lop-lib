//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/io/directoryInfo.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_IO_DIRECTORYINFO_H__
#define __BCLIB_UTILITY_IO_DIRECTORYINFO_H__

#include <BCLib/utility/baseDef.h>

namespace BCLib
{
namespace Utility
{
class BCLIB_UTILITY_API CDirectoryInfo
{
public:
    CDirectoryInfo(const std::string& dir = "");
    virtual ~CDirectoryInfo();

    void standardization();
    bool isExist();
    int makeDir();
    int getDirectories(std::vector<std::string>& ret);
    int getFiles(std::vector<std::string>& ret);

public:
    /// @brief 目录标准化,将'\\'统一转换成'/'
    /// @return void
    /// @param dir
    static void standardization(std::string& dir);

    /// @brief 判断目录是否存在
    /// @return bool
    /// @param dir
    static bool isExist(const std::string& dir);

    /// @brief 创建目录
    /// @return int ,创建失败返回 -1，创建成功返回 0，已经存在返回 1
    /// @param dir
    static int makeDir(const std::string& dir);

    /// @brief 获取dir目录的所有子目录
    /// @return int
    /// @param dir
    /// @param ret
    static int getDirectories(const std::string& dir, std::vector<std::string>& ret);

    /// @brief 获取dir目录的所有文件
    /// @return int
    /// @param dir
    /// @param ret
    static int getFiles(const std::string& dir, std::vector<std::string>& ret);

    /// @brief 删除dir目录的所有文件
    /// @return int
    /// @param dir
    static int delFiles(const std::string& dir);

    /// @brief 设置当前目录
    /// @return bool
    /// @param dir
    static bool setCurDirectory(const std::string& dir);

    /// @brief 获取当前目录
    /// @return std::string
    static std::string getCurDirectory();

    /// @brief 获取当前可执行程序的目录
    /// @return std::string
    static std::string getExeDirectory();

private:
    std::string m_dirName;
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_IO_DIRECTORYINFO_H__
