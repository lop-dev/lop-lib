//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Math/RandomNumber.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_MATH_RANDOMNUMBER_H__
#define __GELIB_MATH_RANDOMNUMBER_H__

#include <GELib/GeConfig.h>

namespace GELib
{


class CRandomNumber
{
public:

	CRandomNumber();
	~CRandomNumber();

    void SetSeed(unsigned int seed) { m_iSeed = seed; }
    unsigned int GetSeed() const { return m_iSeed; }

	unsigned int Rand();
	float Randf();	// return 0 ~ 1
	float Rand(float minValue, float maxValue);

private:
	unsigned int m_iSeed;
};



}//GELib

#endif//__GELIB_MATH_RANDOMNUMBER_H__
