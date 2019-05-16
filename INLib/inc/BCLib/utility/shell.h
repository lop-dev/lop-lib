//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/shell.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_SHELL_H__
#define __BCLIB_UTILITY_SHELL_H__

#include <BCLib/utility/baseDef.h>

namespace BCLib
{
namespace Utility
{
class BCLIB_UTILITY_API CShell
{
public:
    CShell();
    virtual ~CShell();

    int execute(const std::string& command, std::stringbuf& consoleLog);

private:
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_SHELL_H__
