//////////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   BCLib\utility\logFile.cpp
//    author:     League of Perfect
///   @brief 
///
//////////////////////////////////////////////////////////////////////////
#include <BCLib/utility/logFile.h>
#include <CLLib/BCLib/utility/logFile.h>
#include <CLLib/Utility/convert.h>

namespace CLLib
{
    namespace BCLib_Utility
    {
        CLogFile::CLogFile()
        {
        }

        CLogFile::~CLogFile()
        {
        }

        void CLogFile::setLevel(System::UInt16 type, System::String^ logLevel)
        {
            std::string logLevelString = CLLib::Utility::CConvert::DotNetStringToSTLString(logLevel);
            BCLib::Utility::CLogFile::singleton().setModuleLevel(type, logLevelString.c_str());
        }

        void CLogFile::setOutFile(System::String^ logFlie)
        {
            std::string logFlieString = CLLib::Utility::CConvert::DotNetStringToSTLString(logFlie);
            BCLib::Utility::CLogFile::singleton().setOutFile(logFlieString.c_str());
        }

        void CLogFile::setOutConsole(bool outConsole)
        {
            BCLib::Utility::CLogFile::singleton().setOutConsole(outConsole);
        }
    }//BCLib_Utility
}//CLLib