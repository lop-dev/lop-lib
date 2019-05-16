//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SSLib/accountSet/accountServer/accountApp.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SSLIB_ACCOUNTSET_ACCOUNTSERVER_ACCOUNTAPP_H__
#define __SSLIB_ACCOUNTSET_ACCOUNTSERVER_ACCOUNTAPP_H__

#include <BCLib/utility/thread/thread.h>
#include <BCLib/utility/singleton.h>
#include <BCLib/database/dbInfo.h>

namespace SSLib
{
namespace AccountServer
{

class CAccountApp : public BCLib::Utility::CThread
{
    BCLIB_SINGLETON_DECLARE(CAccountApp);

public:
    CAccountApp();
    virtual ~CAccountApp();

    bool init();
    void final();

    bool isRunning() { return m_isRunning; }
    void terminate() { m_isRunning = false; }

protected:
    virtual void _run();

private:
    bool m_isRunning;
    double m_dLastTime;
    float m_fTimeScale;
    static const BCLib::uint32 CREATE_THREAD = 10;
};

}//AccountServer
}//SSLib

#endif//__SSLIB_ACCOUNTSET_ACCOUNTSERVER_ACCOUNTAPP_H__
