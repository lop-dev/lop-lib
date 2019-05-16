//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/sysConfig.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_SYSCONFIG_H__
#define __BCLIB_FRAMEWORK_SYSCONFIG_H__

#include <BCLib/utility/io/iniFile.h>
#include <BCLib/utility/singleton.h>
#include <BCLib/framework/baseDef.h>

namespace BCLib
{
namespace Framework
{
class BCLIB_FRAMEWORK_API CSysConfig : public Utility::CIniFile
{
    BCLIB_SINGLETON_DECLARE(CSysConfig);

public:
    CSysConfig();
    virtual ~CSysConfig();

    void initMain(char* lpCmdLine);
    void initMain(int argc, char* argv[]);
    const std::vector<char*>& argv()
    {
        return m_argv;
    }

protected:

private:
    void _initCmd(std::vector<std::string>& cmdList);
    std::vector<char*> m_argv;
    char* m_cmdLine;
};
}//Framework
}//BCLib

#endif//__BCLIB_FRAMEWORK_SYSCONFIG_H__
