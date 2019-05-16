//////////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   BCLib\framework\sysConfig.h
//    author:     League of Perfect
///   @brief 
///
//////////////////////////////////////////////////////////////////////////
#ifndef __CLLIB_FRAMEWORK_SYSCONFIG_H__
#define __CLLIB_FRAMEWORK_SYSCONFIG_H__

#include <CLLib/BCLib/utility/io/iniFile.h>
#include <BCLib/utility/io/iniFile.h>
#include <BCLib/utility/singleton.h>

namespace CLLib
{
namespace Framework
{

class CNativeSysConfig : public BCLib::Utility::CIniFile
{
public:
    BCLIB_SINGLETON_DECLARE(CNativeSysConfig);

    CNativeSysConfig();

    virtual ~CNativeSysConfig();

protected:

};

public ref class CSysConfig : CLLib::Utility::CIniFile
{
public:
    static CSysConfig^ singleton()
    {
        return m_Instance;
    }

    CSysConfig();
    virtual ~CSysConfig();

    virtual void Init() override;

private:
    static CSysConfig^ m_Instance = gcnew CSysConfig();
};

}//Framework

}//CLLib

#endif//__CLLIB_FRAMEWORK_SYSCONFIG_H__