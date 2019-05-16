//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/FpsCounter.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_FPSCOUNTER_H__
#define __GELIB_UTILITY_FPSCOUNTER_H__

namespace GELib
{

class CFpsCounter
{
public:
	CFpsCounter();

	void Update(double now);
	void Update();

	float GetFps() { return m_fFps; }

private:

    float m_fFps;
    double m_fLastResetTime;
    unsigned int m_iNumFrames;
};



}//GELib


#endif//__GELIB_UTILITY_FPSCOUNTER_H__
