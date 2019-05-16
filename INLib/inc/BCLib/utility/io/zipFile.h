//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/io/zipFile.h
//  author:     League of Perfect
/// @brief      读写zip文件的类
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_IO_ZIPFILE_H__
#define __BCLIB_UTILITY_IO_ZIPFILE_H__

#if defined(_WIN32)

#include <BCLib/utility/baseDef.h>

namespace BCLib
{
namespace Utility
{
class CZipFileImp;
class BCLIB_UTILITY_API CZipFile
{
public:
    CZipFile(void);
    virtual ~CZipFile(void);

    bool Create(const std::string& filename, const std::string& password = "");
    bool Open(const std::string& filename, const std::string& password = "");

    bool AddFolder(const std::string& dirname);
    bool AddFilesInFolder(const std::string& dirname, const std::string& savename);
    bool AddFile(const std::string& filename, const std::string& savename);
    bool AddFile(const std::string& filename);
    bool AddStream(const std::string& savename, const std::string& stream);

    bool RemoveFile(const std::string& filename);//效率不好，慎用。
    bool UnzipTo(const std::string& dirname);
    bool UnzipFile(const std::string& srcFile, const std::string& dstFile);

    void Close();
private:
    CZipFileImp* m_ZipImp;
};
}//Utility
}//BCLib

#endif//_WIN32

#endif//__BCLIB_UTILITY_IO_ZIPFILE_H__
