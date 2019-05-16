//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Math/Curve.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_MATH_CURVE_H__
#define __GELIB_MATH_CURVE_H__

#include <GELib/Math/Vector.h>


namespace GELib
{

template<class T> T CurveHermit(const T &p0, const T &p1, const T &p2, const T &p3, float t)
{
	float t2 = t * t;
	float t3 = t2 * t;
	return p0*(2*t3-3*t2+1) + (p1-p0)*(t3-2*t2+t) + (p3-p2)*(t3-t2) + p3*(-2*t3+3*t2);
}

template<class T> T CurveBezier(const T &p0, const T &p1, const T &p2, const T &p3, float t)
{
	float t2 = t * t;
	float t3 = t2 * t;
	return p0*(-t3+3*t2-3*t+1) + p1*(3*t3-6*t2+3*t) + p2*(-3*t3+3*t2) + p3*(t3);
}

template<class T> T CurveCatmullRom(const T &p0, const T &p1, const T &p2, const T &p3, float t)
{
    if (t == 0)
        return p1;
	float t2 = t * t;
	float t3 = t2 * t;
	return (p0*(-t3+2*t2-t) + p1*(3*t3-5*t2+2) + p2*(-3*t3+4*t2+t) + p3*(t3-t2))*0.5f;
}

// First derivative

template<class T> T CurveHermitD1(const T &p0, const T &p1, const T &p2, const T &p3, float t)
{
	float t2 = t * t;
	return p0*(6*t2-6*t) + (p1-p0)*(3*t2-4*t+1) + (p3-p2)*(3*t2-2*t) + p3*(-6*t2+6*t);
}

template<class T> T CurveBezierD1(const T &p0, const T &p1, const T &p2, const T &p3, float t)
{
	float t2 = t * t;
	return p0*(-3*t2+6*t-3) + p1*(9*t2-12*t+3) + p2*(-9*t2+6*t) + p3*(3*t2);
}

template<class T> T CurveCatmullRomD1(const T &p0, const T &p1, const T &p2, const T &p3, float t)
{
	float t2 = t * t;
	return p0*(-1.5f*t2+2*t-0.5f) + p1*(4.5f*t2-5*t) + p2*(-4.5f*t2+4*t+0.5f) + p3*(1.5f*t2-t);
}



}//GELib

#endif//__GELIB_MATH_CURVE_H__
