//////////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   BCLib\framework\sysConfig.cpp
//    author:     League of Perfect
///   @brief 
///
//////////////////////////////////////////////////////////////////////////
#include <BCLib/utility/logFile.h>
#include <CLLib/BCLib/framework/sysConfig.h>

namespace CLLib
{
    namespace BCLib_Framework
    {
        BCLIB_SINGLETON_DEFINE(CNativeSysConfig);

        CNativeSysConfig::CNativeSysConfig()
        {

        }

        CNativeSysConfig::~CNativeSysConfig()
        {

        }

        CSysConfig::CSysConfig()
        {

        }

        CSysConfig::~CSysConfig()
        {

        }

        void CSysConfig::Init()
        {
            m_iniFile = &CNativeSysConfig::singleton();
        }
    }//BCLib_Framework
}//CLLib