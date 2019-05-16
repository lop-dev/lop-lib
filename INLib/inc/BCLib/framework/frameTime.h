//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/frameTime.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_FRAMETIME_H__
#define __BCLIB_FRAMEWORK_FRAMETIME_H__

#include <BCLib/utility/thread/thread.h>
#include <BCLib/framework/threadFrame.h>
#include <BCLib/utility/dateTime.h>

namespace BCLib
{
namespace Framework
{
class BCLIB_FRAMEWORK_API CFrameTime
{
public:
    CFrameTime();
    virtual ~CFrameTime();

    void setClampVal(float fClampMin, float fClampMax);
    void setScaleVal(float fScale) { m_fScaleVal = fScale; }
    void setFPSLimit(float fLimit);
    float getFPSTimes() { return m_fFPSTimes; }
    void setSmoothTime(bool bSmoothTime);

    void update();

    float getFPS() { return m_fFPS; }

	bool getPrintFPS() { return m_bPrintFPS; }
	void setPrintFPS(bool bPrintFPS) { m_bPrintFPS = bPrintFPS; }

    float getFrameTime() { return m_fCurFrameTime; }

    float getSleepTime();

private:
    BCLib::Utility::CSteadyTime m_steadyTime;

    BCLib::uint64 m_uFrameNum;
    BCLib::uint32 m_uFrameFtp;

    float m_fClampMin;
    float m_fClampMax;
    float m_fScaleVal;
    float m_fFPSLimit;
    float m_fFPSTimes;

    BCLib::int64 m_nPreTime;
    BCLib::int64 m_nFtpTime;

    float m_fFPS;
	bool m_bPrintFPS;

    float m_fCurFrameTime;
    float m_fMaxFrameTime;

    bool m_bSmoothTime;
    static const BCLib::uint8 NUM_FRAME_TIME_HISTORY = 3;
    float m_aFrameTimeHistory[NUM_FRAME_TIME_HISTORY];
};
}//Framework
}//BCLib


#endif//__BCLIB_FRAMEWORK_FRAMETIME_H__
