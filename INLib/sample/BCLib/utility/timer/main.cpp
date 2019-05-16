//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/timer/main.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <BCLib/utility/thread/thread.h>
#include <BCLib/utility/dateTime.h>
#include <BCLib/utility/timer.h>
#include <BCLib/utility/thread/thread.h>

#include <iostream>

#define TIMER_MAX 1000
#define THREAD_MAX 1

int g_testDate = 0;
class CTestTimer
{
public:
    CTestTimer()
    :m_a(0)
    {
    }
    void show(BCLib::Utility::CTimer* timer, void* param)
    {
        ++g_testDate;
        printf("[%d][%I64d][%d]\n", m_a, m_test.milliseconds(), g_testDate);
        m_test.reset();
        m_a += 1;
    }

    void showNothing(BCLib::Utility::CTimer* timer, void* param)
    {
        ++g_testDate;
        printf("[%d][%I64d][%d]\n", m_a, m_test.milliseconds(), g_testDate);
        m_test.reset();
    }


private:
    int m_a;
    BCLib::Utility::CSurveyTimer m_test;
};

void testShow(BCLib::Utility::CTimer* timer, void* param)
{
    ++g_testDate;
}

BCLib::ResThread testFun(void* param)
{
    BCLib::Utility::CTimerServer* timerServer = (BCLib::Utility::CTimerServer*)param;

    if(timerServer == NULL)
    {
        return BCLIB_THREAD_RES(0);
    }

    while(true)
    {
        timerServer->synRun();
        BCLib::Utility::CThread::msleep(10);
    }
    return BCLIB_THREAD_RES(0);
}

int main(int argc, char* argv[])
{
    CTestTimer test1;
    BCLib::Utility::CTimerServer mainTimerServer;
    BCLib::Utility::CTimer* timer[TIMER_MAX];
    /*for (int i=0; i<TIMER_MAX; ++i) {
        timer[i] = new BCLib::Utility::CTimer(&mainTimerServer, 1, 0,10);
        if (timer[i] != NULL) {
            timer[i]->setTriggerCb(&CTestTimer::showNothing, &test1, NULL);
            timer[i]->start();
        }
    }*/
    BCLib::Utility::CTimer maintimer1(&mainTimerServer, 10000, 0);
    //CTimerCallBack timerCB;
    //maintimer1.setTriggerCb(&timerCB);
    maintimer1.setDelay(5000);
    maintimer1.setTriggerCb(&CTestTimer::showNothing, &test1, NULL);
    maintimer1.start();
    while(true)
    {
        /*if (g_testDate) {
        }*/
        mainTimerServer.synRun();
    }
    /*BCLib::Utility::CTimerServer timeServer[THREAD_MAX];
    BCLib::Utility::CHndThread hndThread[THREAD_MAX];
    BCLib::Utility::CTimer* timer[TIMER_MAX];
    CTestTimer test1;
    for (int i=0; i<THREAD_MAX; ++i) {
        hndThread[i] = BCLib::Utility::CHndThread::createThread(testFun, &timeServer[i]);
    }
    for (int i=0; i<TIMER_MAX; ++i) {
        timer[i] = new BCLib::Utility::CTimer(&timeServer[i%THREAD_MAX], 50, 0);
        if (timer[i] != NULL) {
            timer[i]->setTriggerCb(testShow, NULL);
            timer[i]->start();
        }
    }*/

    /*CTestTimer test1;
    BCLib::Utility::CTimerServer mainTimerServer;
    BCLib::Utility::CTimer maintimer1(&mainTimerServer, 1000, false);
    maintimer1.setTriggerCb(&CTestTimer::show, &test1, NULL);
    maintimer1.start();
    while (true) {
        mainTimerServer.synRun();
        BCLib::Utility::CThread::msleep(1);
    }*/

    return 0;
}