//////////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   BCLib\utility\io\zipFile.cpp
//    author:     League of Perfect
///   @brief 
///
//////////////////////////////////////////////////////////////////////////
#include <CLLib/BCLib/utility/io/zipFile.h>
#include <CLLib/Utility/convert.h>
#include <BCLib/utility/io/zipFile.h>

namespace CLLib
{
    namespace BCLib_Utility
    {
        CZipFile::CZipFile(void)
        {
            m_zipFile = new BCLib::Utility::CZipFile();
        }

        CZipFile::~CZipFile()
        {
            delete m_zipFile;
            m_zipFile = 0;
        }

        void CZipFile::Create(System::String^ filename, System::String^ password)
        {
            if (m_zipFile != NULL)
            {
                m_zipFile->Create(CLLib::Utility::CConvert::DotNetStringToSTLString(filename), CLLib::Utility::CConvert::DotNetStringToSTLString(password));
            }
        }

        void CZipFile::Open(System::String^ filename, System::String^ password)
        {
            if (m_zipFile != NULL)
            {
                m_zipFile->Open(CLLib::Utility::CConvert::DotNetStringToSTLString(filename), CLLib::Utility::CConvert::DotNetStringToSTLString(password));
            }
        }

        void CZipFile::AddFolder(System::String^ dirname)
        {
            if (m_zipFile != NULL)
            {
                m_zipFile->AddFolder(CLLib::Utility::CConvert::DotNetStringToSTLString(dirname));
            }
        }

        void CZipFile::AddFile(System::String^ filename, System::String^ savename)
        {
            if (m_zipFile != NULL)
            {
                m_zipFile->AddFile(CLLib::Utility::CConvert::DotNetStringToSTLString(filename), CLLib::Utility::CConvert::DotNetStringToSTLString(savename));
            }
        }

        void CZipFile::AddFile(System::String^ filename)
        {
            if (m_zipFile != NULL)
            {
                m_zipFile->AddFile(CLLib::Utility::CConvert::DotNetStringToSTLString(filename));
            }
        }

        void CZipFile::UnzipTo(System::String^ dirname)
        {
            if (m_zipFile != NULL)
            {
                m_zipFile->UnzipTo(CLLib::Utility::CConvert::DotNetStringToSTLString(dirname));
            }
        }

        void CZipFile::UnzipFile(System::String^ srcFile, System::String^ dstFile)
        {
            if (m_zipFile != NULL)
            {
                m_zipFile->UnzipFile(CLLib::Utility::CConvert::DotNetStringToSTLString(srcFile),
                    CLLib::Utility::CConvert::DotNetStringToSTLString(dstFile));
            }
        }

        void CZipFile::Close()
        {
            if (m_zipFile != NULL)
            {
                m_zipFile->Close();
            }
        }
    }//BCLib_Utility
}//CLLib