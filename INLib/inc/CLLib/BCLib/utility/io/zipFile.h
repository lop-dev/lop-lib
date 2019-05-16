//////////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   BCLib\utility\io\zipFile.h
//    author:     League of Perfect
///   @brief 
///
//////////////////////////////////////////////////////////////////////////
#ifndef __CLLIB_UTILITY_ZIPFILE_H__
#define __CLLIB_UTILITY_ZIPFILE_H__

namespace BCLib
{
    namespace Utility
    {
        class CZipFile;
    }
}

namespace CLLib
{
namespace Utility
{

public ref class CZipFile
{
public:
    CZipFile(void);
    ~CZipFile();

    void Create(System::String^ filename, System::String^ password);
    void Open(System::String^ filename, System::String^ password);

    void AddFolder(System::String^ dirname);
    void AddFile(System::String^ filename, System::String^ savename);
    void AddFile(System::String^ filename);

    void UnzipTo(System::String^ dirname);
    void UnzipFile(System::String^ srcFile, System::String^ dstFile);
    void Close();
private:
    BCLib::Utility::CZipFile* m_zipFile;
};

}//Utility
}//CLLib

#endif