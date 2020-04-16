//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/logicServer/logicApp.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_LOGICSERVER_LOGICAPP_H__
#define __SFLIB_LOGICSERVER_LOGICAPP_H__

#include <SFLib/commonServer/commonApp.h>

namespace SFLib
{
namespace Logic
{

class CLogicApp : public SFLib::CommonServer::CCommonApp
{
public:
    CLogicApp();
    virtual ~CLogicApp();

protected:
    virtual bool _init();
    virtual bool _callback();
    virtual int _final();

private:
    static BCLib::Utility::CSteadyTime s_SteadyTime;
    static BCLib::Utility::CDelayTimer s_OneSecond;

    BCLib::uint32 m_uBrokenServerInterval;
};

}//Logic
}//SFLib

#endif//__SFLIB_LOGICSERVER_LOGICAPP_H__
