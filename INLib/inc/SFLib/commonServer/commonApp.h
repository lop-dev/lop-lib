//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/commonApp.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_COMMONAPP_H__
#define __SFLIB_COMMONSERVER_COMMONAPP_H__

#include <SFLib/commonDefine/baseDef.h>
#include <BCLib/utility/thread/thread.h>
#include <BCLib/utility/dateTime.h>

namespace SFLib
{
namespace CommonServer
{

class CCommonApp : public BCLib::Utility::CThread
{
public:
    CCommonApp();
    virtual ~CCommonApp();

    static CCommonApp* getCommonApp()
    {
        return m_commonApp;
    }

public:
    bool isRunning() { return m_isRunning; }
    void terminate() { m_isRunning = false; }

protected:
    virtual void _run();

    virtual bool _init();
    virtual bool _callback();
    virtual int _final();

private:
    static CCommonApp* m_commonApp;

    bool m_isRunning;
};

}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_COMMONAPP_H__
