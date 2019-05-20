//////////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   BCLib\utility\iniFile.h
//    author:     League of Perfect
///   @brief 
///
//////////////////////////////////////////////////////////////////////////
#ifndef __CLLIB_UTILITY_INIFILE_H__
#define __CLLIB_UTILITY_INIFILE_H__

namespace BCLib
{
    namespace Utility
    {
        class CIniFile;
    }
}

namespace CLLib
{
    namespace BCLib_Utility
    {
        public ref class CIniFile
        {
        public:
            CIniFile();
            virtual ~CIniFile();

            virtual void Init();

            bool Open(System::String^ strFile);

            System::String^ GetValue(System::String^ key);
            void SetValue(System::String^ key, System::String^ value);

            void Remove(System::String^ key);
            bool Find(System::String^ key);
            void Clear();
            System::UInt32 Size();

            int GetValueInt(System::String^ key);
            int GetValueInt(System::String^ appName, System::String^ paramName);
            System::String^ GetValueStr(System::String^ key);
            System::String^ GetValueStr(System::String^ appName, System::String^ paramName);

            void ShowAllData();

        protected:
            BCLib::Utility::CIniFile* m_iniFile;
        };

    }//BCLib_Utility
}//CLLib

#endif//__CLLIB_UTILITY_INIFILE_H__