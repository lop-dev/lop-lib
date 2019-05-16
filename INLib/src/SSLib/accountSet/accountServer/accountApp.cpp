//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SSLib/accountSet/accountServer/accountApp.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/dateTime.h>
#include "./databaseTask/databaseTaskMgr.h"
#include "./databaseMgr/databaseMgr.h"
#include "accountApp.h"

namespace SSLib
{
namespace AccountServer
{

BCLIB_SINGLETON_DEFINE(CAccountApp);

CAccountApp::CAccountApp()
:m_isRunning(false)
,m_dLastTime(.0)
,m_fTimeScale(1.0f)
{

}

CAccountApp::~CAccountApp()
{
    final();
}

bool CAccountApp::init()
{
    return true;
}

void CAccountApp::final()
{
    m_isRunning = false;
    join();
}

void CAccountApp::_run()
{
    m_isRunning = true;
    BCLib::Utility::CSurveyTimer surveyTimer;
    while (m_isRunning){
        surveyTimer.reset();

        double dCurrentTime = BCLib::Utility::CDateTime::getMilliseconds();
        float fFrameTime = float(dCurrentTime - m_dLastTime) * 0.001f;
        m_dLastTime = dCurrentTime;
        fFrameTime = fFrameTime * m_fTimeScale;
        //CDatabaseMgr::singleton().update(fFrameTime);
        //PW::CMsgHandle::singleton().parseMessage();
        CDatabaseTaskMgr::singleton().processTaskResults();
        //CCacheManager::singleton().callback();

        BCLib::int64 tempMs = surveyTimer.milliseconds();
        if (tempMs < 10){
            BCLib::Utility::CThread::msleep((BCLib::int32)(10 - tempMs));
        } else {
            BCLib::Utility::CThread::msleep(1);
        }
        if (tempMs > 500){
            //BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "Ñ­»·³¬³ö[%d]milliseconds", tempMs);
        }
    }
}

}//AccountServer
}//SSLib
