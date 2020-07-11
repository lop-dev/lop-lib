//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/externalServer/externalApp.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_EXTERNALSERVER_EXTERNALAPP_H__
#define __SFLIB_EXTERNALSERVER_EXTERNALAPP_H__

#include <SFLib/commonServer/commonApp.h>

namespace SFLib
{
namespace External
{

class SFLIB_EXTERNAL_API CExternalApp : public SFLib::CommonServer::CCommonApp
{
public:
    CExternalApp();
    virtual ~CExternalApp();

protected:
    virtual bool _init();
    virtual bool _callback();
    virtual int _final();

private:
    static BCLib::Utility::CSteadyTime s_SteadyTime;
    static BCLib::Utility::CDelayTimer s_OneSecond;
};

}//External
}//SFLib

#endif//__SFLIB_EXTERNALSERVER_EXTERNALAPP_H__
