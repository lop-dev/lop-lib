//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Math/Plane.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_MATH_PLANE_H__
#define __GELIB_MATH_PLANE_H__

#include <GELib/GeConfig.h>
#include <GELib/Math/Vector.h>
#include <GELib/Math/Matrix.h>

// ax + by + cz + d = 0
// a = m_Normal.x, b = m_Normal.y, c = m_Normal.z, d = m_fDistance

namespace GELib
{



class CPlane
{
public:
	GELIB_API CPlane();
	GELIB_API CPlane(float a, float b, float c, float d);
	GELIB_API CPlane(const CVector4 &v);
	GELIB_API CPlane(const CVector3 &n, float d);
	GELIB_API CPlane(const CVector3 &v1, const CVector3 &v2, const CVector3 &v3);

	GELIB_API void Set(float a, float b, float c, float d);
	GELIB_API void Set(const CVector4 &v);
	GELIB_API void Set(const CVector3 &n, float d);
	GELIB_API void Set(const CVector3 &p1, const CVector3 &p2, const CVector3 &p3);

	GELIB_API float Distance(const CVector3 &p) const;
	GELIB_API float Normalize();
	GELIB_API void  Transform(const CMatrix &matrix);

    // compute a matrix to project point onto plane
    GELIB_API void  ComputeProjectionMatrix(CMatrix &matrix);

    GELIB_API CPlane operator-() const;

	CVector3 m_Normal;
	float m_fDistance;
};

inline CPlane::CPlane()
{
}

inline CPlane::CPlane(float a, float b, float c, float d)
{
	Set(a, b, c, d);
}

inline CPlane::CPlane(const CVector4 &v)
{
	Set(v);
}

inline CPlane::CPlane(const CVector3 &n, float d)
{
	Set(n, d);
}

inline CPlane::CPlane(const CVector3 &p1, const CVector3 &p2, const CVector3 &p3)
{
	Set(p1, p2, p3);
}

inline void CPlane::Set(float a, float b, float c, float d)
{
	m_Normal.Set(a, b, c);
	m_fDistance = d;
}

inline void CPlane::Set(const CVector4 &v)
{
	m_Normal.Set(v.x, v.y, v.z);
	m_fDistance = v.w;
}

inline void CPlane::Set(const CVector3 &n, float d)
{
	m_Normal = n;
	m_fDistance = d;
}

inline void CPlane::Set(const CVector3 &p1, const CVector3 &p2, const CVector3 &p3)
{
	CVector3 e1 = p2 - p1;
	CVector3 e2 = p3 - p1;
	m_Normal = CrossProduct(e1, e2);
	m_Normal.Normalize();
	m_fDistance = -DotProduct(m_Normal, p1);
}

inline float CPlane::Distance(const CVector3 &p) const
{
	return DotProduct(m_Normal, p) + m_fDistance;
}

inline float CPlane::Normalize()
{
	float len =m_Normal.Normalize();
	if (len)
		m_fDistance /= len;
	return len;
}

inline CPlane CPlane::operator-() const
{
    return CPlane(-m_Normal.x, -m_Normal.y, -m_Normal.z, -m_fDistance);
}



}//GELib


#endif//__GELIB_MATH_PLANE_H__
