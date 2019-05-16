//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Math/Capsule.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_MATH_CAPSULE_H__
#define __GELIB_MATH_CAPSULE_H__

#include <GELib/GeConfig.h>
#include <GELib/Math/Vector.h>

namespace GELib
{


class CCapsule
{
public:

	GELIB_API CCapsule() {}
	GELIB_API CCapsule(const CVector3 &p0, const CVector3 &p1, float radius)
	{
		m_P[0] = p0;
		m_P[1] = p1;
		m_fRadius = radius;
	}

	GELIB_API void Set(const CVector3 &p0, const CVector3 &p1, float radius)
	{
		m_P[0] = p0;
		m_P[1] = p1;
		m_fRadius = radius;
	}

    GELIB_API bool IsContainPoint(const CVector3 &p) const;


	CVector3 m_P[2];
	float m_fRadius;
};


}//GELib

#endif//__GELIB_MATH_CAPSULE_H__
