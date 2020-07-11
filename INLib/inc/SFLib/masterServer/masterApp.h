//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/masterServer/masterApp.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_MASTERSERVER_MASTERAPP_H__
#define __SFLIB_MASTERSERVER_MASTERAPP_H__

#include <SFLib/commonServer/commonApp.h>

namespace SFLib
{
namespace Master
{

class SFLIB_MASTER_API CMasterApp : public SFLib::CommonServer::CCommonApp
{
public:
    CMasterApp();
    virtual ~CMasterApp();

protected:
    virtual bool _init();
    virtual bool _callback();
    virtual int _final();

private:
    static BCLib::Utility::CSteadyTime s_SteadyTime;
    static BCLib::Utility::CDelayTimer s_OneMinute;
    static BCLib::Utility::CDelayTimer s_FiveMinute;
};

}//Master
}//SFLib

#endif//__SFLIB_MASTERSERVER_MASTERAPP_H__
