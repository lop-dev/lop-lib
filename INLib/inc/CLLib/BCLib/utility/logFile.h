//////////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   BCLib\utility\logFile.h
//    author:     League of Perfect
///   @brief 
///
//////////////////////////////////////////////////////////////////////////
#ifndef __CLLIB_UTILITY_TRACER_H__
#define __CLLIB_UTILITY_TRACER_H__

namespace CLLib
{

public enum class ELogType
{
    //特殊功能
    ELOGMODULE_ALL = 0,
    ELOGMODULE_DEFAULT,
    ELOGMODULE_ASSERT,

    //BCLib 使用
    ELOGMODULE_BCLIB_UTILITY = 8,
    ELOGMODULE_BCLIB_NETWORK,
    ELOGMODULE_BCLIB_FRAMEWORK,
    ELOGMODULE_BCLIB_P2PCLIENT,
    ELOGMODULE_BCLIB_P2PSERVER,
    ELOGMODULE_BCLIB_MATH,
    ELOGMODULE_BCLIB_DATABASE,
    ELOGMODULE_BCLIB_LUASCRIPT,
    ELOGMODULE_BCLIB_SECURITY,

    //BCLib 以外的使用
    ELOGMODULE_EXTEND = 128,

    ELOGMODULE_MAX = BCLIB_UINT16_MAX,
};

namespace Utility
{

public ref class CLogFile
{
public:

    CLogFile();
    virtual ~CLogFile();
    
    void setLevel(System::UInt16 type,System::String^ logLevel);
    void setOutFile(System::String^ logFlie);
    void setOutConsole(bool isConsole);

private:
    //BCLib::Utility::CLogFile* m_logFile;
};

}//Utility

}//CLLib

#endif//__CLLIB_UTILITY_TRACER_H__