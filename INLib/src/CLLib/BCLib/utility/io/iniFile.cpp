//////////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   BCLib\utility\iniFile.cpp
//    author:     League of Perfect
///   @brief 
///
//////////////////////////////////////////////////////////////////////////
#include <BCLib/utility/io/iniFile.h>
#include <CLLib/BCLib/utility/io/iniFile.h>
#include <CLLib/Utility/convert.h>

namespace CLLib
{
    namespace BCLib_Utility
    {
        CIniFile::CIniFile() : m_iniFile(NULL)
        {
        }

        CIniFile::~CIniFile()
        {
            if (m_iniFile != NULL)
            {
                BCLIB_SAFE_DELETE(m_iniFile);
            }
        }

        void CIniFile::Init()
        {
            m_iniFile = new BCLib::Utility::CIniFile();
        }

        bool CIniFile::Open(System::String^ strFile)
        {
            if (m_iniFile != NULL)
            {
                std::string filename = CLLib::Utility::CConvert::DotNetStringToSTLString(strFile);
                return m_iniFile->open(filename);
            }
            return false;
        }

        System::String^ CIniFile::GetValue(System::String^ key)
        {
            if (m_iniFile != NULL)
            {
                std::string nativekey = CLLib::Utility::CConvert::DotNetStringToSTLString(key);
                std::string nativevalue = m_iniFile->getValue(nativekey);
                return CLLib::Utility::CConvert::STLStringToDotNetString(nativevalue);
            }
            return "";
        }

        void CIniFile::SetValue(System::String^ key, System::String^ value)
        {
            if (m_iniFile != NULL)
            {
                std::string nativekey = CLLib::Utility::CConvert::DotNetStringToSTLString(key);
                std::string nativevalue = CLLib::Utility::CConvert::DotNetStringToSTLString(value);
                m_iniFile->setValue(nativekey, nativevalue);
            }
        }

        int CIniFile::GetValueInt(System::String^ key)
        {
            if (m_iniFile != NULL)
            {
                std::string nativekey = CLLib::Utility::CConvert::DotNetStringToSTLString(key);
                return m_iniFile->getValueInt(nativekey);
            }
            return System::Int32::MinValue;
        }

        int CIniFile::GetValueInt(System::String^ appName, System::String^ paramName)
        {
            if (m_iniFile != NULL)
            {
                std::string nativeAppName = CLLib::Utility::CConvert::DotNetStringToSTLString(appName);
                std::string nativeParamName = CLLib::Utility::CConvert::DotNetStringToSTLString(paramName);
                return m_iniFile->getValueInt(nativeAppName, nativeParamName);
            }
            return System::Int32::MinValue;
        }

        void CIniFile::Remove(System::String^ key)
        {
            if (m_iniFile != NULL)
            {
                std::string nativekey = CLLib::Utility::CConvert::DotNetStringToSTLString(key);
                m_iniFile->remove(nativekey);
            }
        }

        bool CIniFile::Find(System::String^ key)
        {
            if (m_iniFile != NULL)
            {
                std::string nativekey = CLLib::Utility::CConvert::DotNetStringToSTLString(key);
                return m_iniFile->find(nativekey);
            }
            return false;
        }

        void CIniFile::Clear()
        {
            if (m_iniFile != NULL)
            {
                m_iniFile->clear();
            }
        }

        System::UInt32 CIniFile::Size()
        {
            if (m_iniFile != NULL)
            {
                return m_iniFile->size();
            }
            return 0;
        }

        System::String^ CIniFile::GetValueStr(System::String^ key)
        {
            if (m_iniFile != NULL)
            {
                std::string nativekey = CLLib::Utility::CConvert::DotNetStringToSTLString(key);
                std::string nativevalue = m_iniFile->getValueStr(nativekey);
                return CLLib::Utility::CConvert::STLStringToDotNetString(nativevalue);
            }
            return "";
        }

        System::String^ CIniFile::GetValueStr(System::String^ appName, System::String^ paramName)
        {
            if (m_iniFile != NULL)
            {
                std::string nativeAppName = CLLib::Utility::CConvert::DotNetStringToSTLString(appName);
                std::string nativeParamName = CLLib::Utility::CConvert::DotNetStringToSTLString(paramName);
                std::string nativevalue = m_iniFile->getValueStr(nativeAppName, nativeParamName);
                return CLLib::Utility::CConvert::STLStringToDotNetString(nativevalue);
            }
            return "";
        }

        void CIniFile::ShowAllData()
        {
            if (m_iniFile != NULL)
            {
                m_iniFile->showAllData();
            }
        }
    }//BCLib_Utility
}//CLLib