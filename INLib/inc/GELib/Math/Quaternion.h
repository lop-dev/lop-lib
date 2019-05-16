//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Math/Quaternion.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_MATH_QUATERNION_H__
#define __GELIB_MATH_QUATERNION_H__

#include <GELib/GeConfig.h>
#include <GELib/Math/MathImpl.h>
#include <GELib/Math/Matrix.h>

namespace GELib
{


class CQuaternion
{
public:
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
        float m_v[4];
    };

    CQuaternion() {}
    CQuaternion(float v0, float v1, float v2, float w) : x(v0), y(v1), z(v2), w(w) {}
    CQuaternion(const float v[4]) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}
    CQuaternion(const CQuaternion &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

    CQuaternion(const CMatrix &m16);
    ~CQuaternion() {}

    void Set(float x, float y, float z, float w);
    void Set(const CVector3 &vAxis, float fRadius);
    void Set(const CMatrix &m16);

    void SetZero();
    bool IsZero() const;
    bool IsNearZero() const;
    void SetIdentity();

	float Length() const;
	float LengthSqr() const;
	float Normalize();
    void FastNormalize();

    CQuaternion Conjugate() const;
    void ConjugateSelf();

    CQuaternion operator+(const CQuaternion &v) const;
    CQuaternion operator-(const CQuaternion &v) const;
    CQuaternion operator*(const CQuaternion &v) const;
    CQuaternion & operator+=(const CQuaternion &v);
    CQuaternion & operator-=(const CQuaternion &v);
    CQuaternion & operator*=(const CQuaternion &v);
    CQuaternion operator*(float f) const;
    CQuaternion operator/(float f) const;
    CQuaternion operator*=(float f);
    CQuaternion operator/=(float f);

    CQuaternion operator-() const;

    bool operator==(const CQuaternion &v) const;
    bool operator!=(const CQuaternion &v) const;

    void ConvertToMatrix(CMatrix &m16) const;
    void ConvertFromMatrix(const CMatrix &m16);
};

inline CQuaternion::CQuaternion(const CMatrix &m16)
{
	ConvertFromMatrix(m16);
}

inline void CQuaternion::Set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

inline void CQuaternion::Set(const CMatrix &m16)
{
	ConvertFromMatrix(m16);
}

inline void CQuaternion::SetZero()
{
	x = y = z = w = 0;
}

inline bool CQuaternion::IsZero() const
{
	return (x == 0) && (y == 0) && (z == 0) && (w == 0);
}

inline bool CQuaternion::IsNearZero() const
{
	return (fabs(x) <= GMH_EPSILON) && (fabs(y) <= GMH_EPSILON) && (fabs(z) <= GMH_EPSILON) && (fabs(w) <= GMH_EPSILON);
}

inline void CQuaternion::SetIdentity()
{
	x = y = z = 0;
	w = 1.0f;
}

inline CQuaternion CQuaternion::Conjugate() const
{
	return CQuaternion(-x, -y, -z, w);
}

inline void CQuaternion::ConjugateSelf()
{
	x = -x;
	y = -y;
	z = -z;
}

inline CQuaternion CQuaternion::operator*(const CQuaternion &v) const
{
	CQuaternion q;
	QuaternionMul(q.m_v, m_v, v.m_v);
	return q;
}

inline CQuaternion & CQuaternion::operator*=(const CQuaternion &v)
{
	QuaternionMul(m_v, m_v, v.m_v);
	return *this;
}

inline float CQuaternion::Length() const
{
    return sqrtf(LengthSqr());
}

inline float CQuaternion::LengthSqr() const
{
	return x * x + y * y + z * z + w * w;
}

inline void CQuaternion::FastNormalize()
{
    float invLen = FastInvSqrt(LengthSqr());
	x *= invLen;
	y *= invLen;
	z *= invLen;
	w *= invLen;
}

inline CQuaternion CQuaternion::operator+(const CQuaternion &v) const
{
	return CQuaternion(x + v.x, y + v.y, z + v.z, w + v.w);
}

inline CQuaternion CQuaternion::operator-(const CQuaternion &v) const
{
	return CQuaternion(x - v.x, y - v.y, z - v.z, w - v.w);
}

inline CQuaternion & CQuaternion::operator+=(const CQuaternion &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

inline CQuaternion & CQuaternion::operator-=(const CQuaternion &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

inline CQuaternion CQuaternion::operator*(float f) const
{
	return CQuaternion(x * f, y * f, z * f, w * f);
}

inline CQuaternion CQuaternion::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

inline CQuaternion CQuaternion::operator/(float f) const
{
	return CQuaternion(x / f, y / f, z / f, w / f);
}

inline CQuaternion CQuaternion::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this;
}

inline CQuaternion CQuaternion::operator-(void) const
{
	return CQuaternion(-x, -y, -z, -w);
}

inline bool CQuaternion::operator==(const CQuaternion &v) const
{
	return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w);
}

inline bool CQuaternion::operator!=(const CQuaternion &v) const
{
	return (x != v.x) || (y != v.y) || (z != v.z) || (w != v.w);
}

inline void CQuaternion::ConvertToMatrix(CMatrix &m16) const
{
	QuaternionToMatrix(m16.m_m16, m_v);
}

inline void CQuaternion::ConvertFromMatrix(const CMatrix &m16)
{
	QuaternionFromMatrix(m_v, m16.m_m16);
}

inline float DotProduct(const CQuaternion &a, const CQuaternion &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline CQuaternion Lerp(const CQuaternion &a, const CQuaternion &b, float t)
{
    if (DotProduct(a, b) < 0)
        return a - (b + a) * t;
    else
        return a + (b - a) * t;
}

inline CQuaternion Nlerp(const CQuaternion &a, const CQuaternion &b, float t)
{
    CQuaternion q = Lerp(a, b, t);
	q.Normalize();
    return q;
}

GELIB_API CQuaternion Slerp(const CQuaternion &a, const CQuaternion &b, float t);

}//GELib

#endif//__GELIB_MATH_QUATERNION_H__
