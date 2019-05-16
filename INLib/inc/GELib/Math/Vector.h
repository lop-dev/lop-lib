//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Math/Vector.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_MATH_VECTOR_H__
#define __GELIB_MATH_VECTOR_H__

#include <GELib/GeConfig.h>
#include <GELib/Math/MathImpl.h>
#include <cmath>

namespace GELib
{

class CMatrix;
class CVector2;
class CVector3;
class CVector4;

class CVector2
{
public:
	union
	{
		struct
		{
			float x;
			float y;
		};
		float m_v[2];
	};

	CVector2() {}
	CVector2(float v0, float v1) : x(v0), y(v1) {}
	CVector2(int v0, int v1) : x((float)v0), y((float)v1) {}
	CVector2(const float v[2]) : x(v[0]), y(v[1]) {}
	CVector2(const CVector2 &v) : x(v.x), y(v.y) {}
	~CVector2() {}

    void Set(float x, float y);

	void SetZero();
	bool IsZero()  const;
	bool IsNearZero() const;

	float Length() const;
	float LengthSqr() const;
	float Normalize();
	void FastNormalize();

	float Angle(const CVector2 &v) const;


	CVector2 operator+(const CVector2 &v) const;
	CVector2 operator-(const CVector2 &v) const;
	CVector2 operator*(float f) const;
	CVector2 operator*(const CVector2 &v) const;
	CVector2 operator/(float f) const;
	CVector2 operator/(const CVector2 &v) const;
	CVector2 &operator+=(const CVector2 &v);
	CVector2 &operator-=(const CVector2 &v);
	CVector2 &operator*=(float f);
	CVector2 &operator*=(const CVector2 &v);
	CVector2 &operator/=(float f);
	CVector2 &operator/=(const CVector2 &v);

	CVector2 operator-() const;

	bool operator==(const CVector2 &v) const;
	bool operator!=(const CVector2 &v) const;
};


class CVector3
{
public:
    union
    {
        struct
        {
            float x;
            float y;
            float z;
        };
		float m_v[3];
    };

    CVector3() {}
    CVector3(float v0, float v1, float v2) : x(v0), y(v1), z(v2) {}
    CVector3(const float v[3]) : x(v[0]), y(v[1]), z(v[2]) {}
    CVector3(const CVector3 &v) : x(v.x), y(v.y), z(v.z) {}
    ~CVector3() {}

    void Set(float x, float y, float z);

    void SetZero();
    bool IsZero() const;
	bool IsNearZero() const;

    float Length() const;
    float LengthSqr() const;
    float Normalize();
    void FastNormalize();

	float Angle(const CVector3 &v) const;

    CVector3 operator+(const CVector3 &v) const;
    CVector3 operator-(const CVector3 &v) const;
    CVector3 operator*(float f) const;
    CVector3 operator*(const CVector3 &v) const;
    CVector3 operator*(const CMatrix &m16) const;
    CVector3 operator/(float f) const;
    CVector3 operator/(const CVector3 &v) const;
    CVector3 & operator+=(const CVector3 &v);
    CVector3 & operator-=(const CVector3 &v);
    CVector3 & operator*=(float f);
    CVector3 & operator*=(const CVector3 &v);
    CVector3 & operator*=(const CMatrix &m16);
    CVector3 & operator/=(float f);
    CVector3 & operator/=(const CVector3 &v);

    CVector3 operator-() const;

    bool operator==(const CVector3 &v) const;
    bool operator!=(const CVector3 &v) const;
};


class CVector4
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

    CVector4() {}
    CVector4(float v0, float v1, float v2, float v3) { x = v0; y = v1; z = v2; w = v3; }
    CVector4(const float v[4]) { x = v[0]; y = v[1]; z = v[2]; w = v[3]; }
    CVector4(const CVector4 &v) { x = v.x; y = v.y; z = v.z; w = v.w; }
    ~CVector4() {}

    void Set(float x, float y, float z, float w);

    bool IsZero() const;
    bool IsNearZero() const;
    void SetZero();

    float Length() const;
    float LengthSqr() const;
    float Normalize();
    void FastNormalize();

	float Angle(const CVector4 &v) const;

    CVector4 operator+(const CVector4 &v) const;
    CVector4 operator-(const CVector4 &v) const;
    CVector4 operator*(float f) const;
    CVector4 operator*(const CVector4 &v) const;
    CVector4 operator*(const CMatrix &m16) const;
    CVector4 operator/(float f) const;
    CVector4 operator/(const CVector4 &v) const;
    CVector4 & operator+=(const CVector4 &v);
    CVector4 & operator-=(const CVector4 &v);
    CVector4 & operator*=(float f);
    CVector4 & operator*=(const CVector4 &v);
    CVector4 & operator*=(const CMatrix &m16);
    CVector4 & operator/=(float f);
    CVector4 & operator/=(const CVector4 &v);

    CVector4 operator-() const;
    bool operator==(const CVector4 &v) const;
    bool operator!=(const CVector4 &v) const;
};

inline void CVector2::Set(float x, float y)
{
	this->x = x;
	this->y = y;
}

inline void CVector2::SetZero()
{
	x = y = 0;
}

inline bool CVector2::IsZero()  const
{
	return (x == 0) && (y == 0);
}

inline bool CVector2::IsNearZero() const
{
	return (fabs(x) <= GMH_EPSILON) && (fabs(y) <= GMH_EPSILON);
}

inline float CVector2::Length() const
{
    return sqrtf(LengthSqr());
}

inline float CVector2::LengthSqr() const
{
	return x * x + y * y;
}

inline void CVector2::FastNormalize()
{
    float invLen = FastInvSqrt(LengthSqr());
	x *= invLen;
	y *= invLen;
}

inline CVector2 CVector2::operator+(const CVector2 &v) const
{
	return CVector2(x + v.x, y + v.y);
}

inline CVector2 CVector2::operator-(const CVector2 &v) const
{
	return CVector2(x - v.x, y - v.y);
}

inline CVector2 & CVector2::operator+=(const CVector2 &v)
{
	x += v.x;
	y += v.y;
	return *this;
}

inline CVector2 & CVector2::operator-=(const CVector2 &v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

inline CVector2 CVector2::operator*(float f) const
{
	return CVector2(x * f, y * f);
}

inline CVector2 CVector2::operator*(const CVector2 &v) const
{
	return CVector2(x * v.x, y * v.y);
}

inline CVector2 & CVector2::operator*=(float f)
{
	x *= f;
	y *= f;
    return *this;
}

inline CVector2 & CVector2::operator*=(const CVector2 &v)
{
	x *= v.x;
	y *= v.y;
    return *this;
}

inline CVector2 CVector2::operator/(float f) const
{
	return CVector2(x / f, y/ f);
}

inline CVector2 CVector2::operator/(const CVector2 &v) const
{
	return CVector2(x / v.x, y / v.y);
}

inline CVector2 & CVector2::operator/=(float f)
{
	x /= f;
	y /= f;
    return *this;
}

inline CVector2 & CVector2::operator/=(const CVector2 &v)
{
	x /= v.x;
	y /= v.y;
    return *this;
}

inline CVector2 CVector2::operator-() const
{
	return CVector2(-x, -y);
}

inline bool CVector2::operator==(const CVector2 &v) const
{
	return (x == v.x) && (y == v.y);
}

inline bool CVector2::operator!=(const CVector2 &v) const
{
	return (x != v.x) || (y != v.y);
}

inline void CVector3::Set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

inline void CVector3::SetZero()
{
	x = y = z = 0;
}

inline bool CVector3::IsZero() const
{
	return (x == 0) && (y == 0) && (z == 0);
}

inline bool CVector3::IsNearZero() const
{
	return (fabs(x) <= GMH_EPSILON) && (fabs(y) <= GMH_EPSILON) && (fabs(z) <= GMH_EPSILON);
}

inline float CVector3::Length() const
{
    return sqrtf(LengthSqr());
}

inline float CVector3::LengthSqr() const
{
	return x * x + y * y + z * z;
}

inline void CVector3::FastNormalize()
{
    float invLen = FastInvSqrt(LengthSqr());
	x *= invLen;
	y *= invLen;
	z *= invLen;
}

inline CVector3 CVector3::operator+(const CVector3 &v) const
{
	return CVector3(x + v.x, y + v.y, z + v.z);
}

inline CVector3 CVector3::operator-(const CVector3 &v) const
{
	return CVector3(x - v.x, y - v.y, z - v.z);
}

inline CVector3 & CVector3::operator+=(const CVector3 &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
    return *this;
}

inline CVector3 & CVector3::operator-=(const CVector3 &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
    return *this;
}

inline CVector3 CVector3::operator*(float f) const
{
	return CVector3(x * f, y * f, z * f);
}

inline CVector3 CVector3::operator*(const CVector3 &v) const
{
	return CVector3(x * v.x, y * v.y, z * v.z);
}

inline CVector3 & CVector3::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
    return *this;
}

inline CVector3 & CVector3::operator*=(const CVector3 &v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
    return *this;
}

inline CVector3 CVector3::operator/(float f) const
{
	return CVector3(x / f, y / f, z / f);
}

inline CVector3 CVector3::operator/(const CVector3 &v) const
{
	return CVector3(x / v.x, y / v.y, z / v.z);
}

inline CVector3 & CVector3::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;
    return *this;
}

inline CVector3 & CVector3::operator/=(const CVector3 &v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
    return *this;
}

inline CVector3 CVector3::operator-() const
{
    return CVector3(-x, -y, -z);
}

inline bool CVector3::operator==(const CVector3 &v) const
{
	return (x == v.x) && (y == v.y) && (z == v.z);
}

inline bool CVector3::operator!=(const CVector3 &v) const
{
	return (x != v.x) || (y != v.y) || (z != v.z);
}

inline void CVector4::Set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

inline void CVector4::SetZero()
{
	x = y = z = w = 0;
}

inline bool CVector4::IsZero() const
{
	return (x == 0) && (y == 0) && (z == 0) && (w == 0);
}

inline bool CVector4::IsNearZero() const
{
	return (fabs(x) <= GMH_EPSILON) && (fabs(y) <= GMH_EPSILON) && (fabs(z) <= GMH_EPSILON) && (fabs(w) <= GMH_EPSILON);
}

inline float CVector4::Length() const
{
    return sqrtf(LengthSqr());
}

inline float CVector4::LengthSqr() const
{
	return x * x + y * y + z * z + w * w;
}

inline void CVector4::FastNormalize()
{
    float invLen = FastInvSqrt(LengthSqr());
	x *= invLen;
	y *= invLen;
	z *= invLen;
	w *= invLen;
}

inline CVector4 CVector4::operator+(const CVector4 &v) const
{
	return CVector4(x + v.x, y + v.y, z + v.z, w + v.w);
}


inline CVector4 CVector4::operator-(const CVector4 &v) const
{
	return CVector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

inline CVector4 & CVector4::operator+=(const CVector4 &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

inline CVector4 & CVector4::operator-=(const CVector4 &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

inline CVector4 CVector4::operator*(float f) const
{
	return CVector4(x * f, y * f, z * f, w * f);
}

inline CVector4 CVector4::operator*(const CVector4 &v) const
{
	return CVector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

inline CVector4 & CVector4::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

inline CVector4 & CVector4::operator*=(const CVector4 &v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return *this;
}

inline CVector4 CVector4::operator/(float f) const
{
	return CVector4(x / f, y / f, z / f, w / f);
}

inline CVector4 CVector4::operator/(const CVector4 &v) const
{
	return CVector4(x / v.x, y / v.y, z / v.z, w / v.w);
}

inline CVector4 & CVector4::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this;
}

inline CVector4 & CVector4::operator/=(const CVector4 &v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
	return *this;
}

inline CVector4 CVector4::operator-() const
{
   return CVector4(-x, -y, -z, -w);
}

inline bool CVector4::operator==(const CVector4 &v) const
{
	return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w);
}

inline bool CVector4::operator!=(const CVector4 &v) const
{
	return (x != v.x) || (y != v.y) || (z != v.z) || (w != v.w);
}

inline float DotProduct(const CVector2 &a, const CVector2 &b)
{
	return a.x * b.x + a.y * b.y;
}

inline float DotProduct(const CVector3 &a, const CVector3 &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float DotProduct(const CVector4 &a, const CVector4 &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline float CrossProduct(const CVector2 &a, const CVector2 &b)
{
	return a.x * b.y - a.y * b.x;
}

inline CVector3 CrossProduct(const CVector3 &a, const CVector3 &b)
{
	return CVector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline CVector3 Reflect(const CVector3 &v, const CVector3 &n)
{
    return v - n * (DotProduct(v, n) * 2);
}

inline CVector3 CVector3::operator*(const CMatrix &m16) const
{
    CVector3 dest;
	Vector3MulM16(dest.m_v, m_v, (const float*)&m16);
    return dest;
}

inline CVector3 & CVector3::operator*=(const CMatrix &m16)
{
    Vector3MulM16(m_v, m_v, (const float*)&m16);
    return *this;
}

inline CVector4 CVector4::operator*(const CMatrix &m16) const
{
    CVector4 dest;
	Vector4MulM16(dest.m_v, m_v, (const float*)&m16);
    return dest;
}

inline CVector4 & CVector4::operator*=(const CMatrix &m16)
{
    Vector4MulM16(m_v, m_v, (const float*)&m16);
    return *this;
}

}//GELib

#endif//__GELIB_MATH_VECTOR_H__
