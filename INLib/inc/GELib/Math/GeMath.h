//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Math/GeMath.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_MATH_GEMATH_H__
#define __GELIB_MATH_GEMATH_H__

#include <GELib/GeConfig.h>
#include <GELib/Math/Vector.h>
#include <GELib/Math/Quaternion.h>
#include <GELib/Math/Matrix.h>
#include <GELib/Math/RGBAColor.h>
#include <GELib/Math/Plane.h>
#include <GELib/Math/AABox.h>
#include <GELib/Math/OBox.h>
#include <GELib/Math/Sphere.h>
#include <GELib/Math/Capsule.h>
#include <vector>

namespace GELib
{


class CFrustum;
class CConvexVolume;

#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif

template<class T> bool IsNearZero(T f)
{
    return (f>(T)0 ? f : -f) < GMH_EPSILON;
}

inline float Lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

inline CVector2 Lerp(const CVector2 &a, const CVector2 &b, float t)
{
	return a + (b - a) * t;
}

inline CVector3 Lerp(const CVector3 &a, const CVector3 &b, float t)
{
	return a + (b - a) * t;
}

inline CVector4 Lerp(const CVector4 &a, const CVector4 &b, float t)
{
	return a + (b - a) * t;
}

inline float DegToRad(float a)
{
	return a * float(M_PI) / 180.0f;
}

inline float RadToDeg(float a)
{
	return a * 180.0f / float(M_PI);
}

inline float RoundDegree(float a)
{
    a = fmodf(a, 360.0f);
    if (a < 0)
        a += 360.0f;
    return a;
}

inline float Round(float a)
{
	return floorf(a + 0.5f);
}

inline float Saturate(float a)
{
    if (a < 0)
        return 0;
    if (a > 1.0f)
        return 1.0f;
    return a;
}

inline float Sign(float a)
{
    if (a > 0)
        return 1.0f;
    if (a < 0)
        return -1.0f;
    return 0;
}

inline float SmoothStep(float x)
{
    return x * x * (3 - 2 * x);
}

inline float SmootherStep(float x)
{
    return x * x * x * (x * (x * 6 - 15) + 10);
}


GELIB_API float Toward(float v0, float v1, float step);

GELIB_API CVector3 GammaToLinear(const CVector3 &color, float gamma=2.2f);
GELIB_API CVector3 LinearToGamma(const CVector3 &color, float gamma=2.2f);

template<typename T> T Clamp(T a, T minValue, T maxValue)
{
    if (a < minValue)
        return minValue;
    if (a > maxValue)
        return maxValue;
    return a;
}

int NearestPower2(int n);
int NextPower2(int n);
bool IsPower2(int n);
bool IsNan(float f);
bool IsFinite(float f);

class CTriangle
{
public:
	CVector3 v[3];
};

GELIB_API float HalfToFloat(unsigned short h);

GELIB_API float Snap(float a, float grid);

GELIB_API void GetMin(CVector3 &bmin, const CVector3 &a);
GELIB_API void GetMax(CVector3 &bmax, const CVector3 &a);

GELIB_API void PointsToBox(const CVector3 *p, int numPoints, CAABox &box);
GELIB_API void PointsToBox(const CMatrix &matrix, const CVector3 *p, int numPoints, CAABox &box);
GELIB_API void OBoxToAABox(const COBox &obox, CAABox &aabox);
GELIB_API void OBoxToAABox(const CAABox &localBox, const CMatrix &matrix, CAABox &aabox);
GELIB_API void AABoxToOBox(const CAABox &aabox, COBox &obox);
GELIB_API void AABoxToOBox(const CAABox &localBox, const CMatrix &matrix, COBox &obox);

GELIB_API bool IsPointsBehindPlane(const CVector3 *p, int numPoints, const CPlane &plane);
GELIB_API bool IsBoxBehindPlane(const CAABox &box, const CPlane &plane);

GELIB_API float PointLineDistance(const CVector3 &p, const CVector3 &p1, const CVector3 &p2);
GELIB_API CVector3 ProjectPointOnLine(const CVector3 &p, const CVector3 &p1, const CVector3 &p2);

GELIB_API bool IsPointInPolygon(const CVector2 &p, const CVector2 *polygon, int sides);
GELIB_API bool IsPointInTriangle(float Px, float Py, float Ax, float Ay, float Bx, float By, float Cx, float Cy);

GELIB_API float BoxBoxDistance(const CAABox &b1, const CAABox &b2);

GELIB_API int GetBoxCornerIndexMaxDistanceToPlane(const CPlane &plane);
GELIB_API int GetBoxCornerIndexMinDistanceToPlane(const CPlane &plane);
GELIB_API CVector3 GetBoxCorner(const CAABox &box, int cornerIndex);
GELIB_API void GetBoxCorners(const CAABox &box, CVector3 *corners);

GELIB_API void  ProjectPoint(const CVector3 &p, const CMatrix &viewProjMatrix, const CVector2 &viewSize, CVector3 &projectedPoint);
GELIB_API void  UnprojectPoint(const CVector3 &p, const CMatrix &viewProjMatrix, const CVector2 &viewSize, CVector3 &unprojectedPoint);
GELIB_API void  PickRayDir(const CVector2 &p, const CMatrix &viewMatrix, const CMatrix &projMatrix, const CVector2 &viewSize, CVector3 &dir);

GELIB_API void ProjectPointsToScreen(const CVector3 *p, int numPoints, const CMatrix &viewProjMatrix, const CVector2 &screenSize, CVector2 &outMin, CVector2 &outMax);
GELIB_API void ProjectBoxToScreen(const CAABox &box, const CMatrix &viewProjMatrix, const CVector2 &screenSize, CVector2 &outMin, CVector2 &outMax);
GELIB_API void ProjectBoxToScreen(const COBox &box, const CMatrix &viewProjMatrix, const CVector2 &screenSize, CVector2 &outMin, CVector2 &outMax);
GELIB_API void ProjectSphereToScreen(const CSphere &s, const CMatrix &viewProjMatrix, const CVector2 &screenSize, CVector2 &outMin, CVector2 &outMax);
GELIB_API void ProjectSphereToScreen(const CSphere &s, const CMatrix &cameraMatrix, const CMatrix &viewProjMatrix, const CVector2 &screenSize, CVector2 &outMin, CVector2 &outMax);
GELIB_API void ProjectFrustumToScreen(const CFrustum &f, const CMatrix &viewProjMatrix, const CVector2 &screenSize, CVector2 &outMin, CVector2 &outMax);

GELIB_API float ComputePolygonArea(const CVector2 *polygon, int sides);

GELIB_API CVector3 DistributePointOnSphere(int num, int index, float angle = float(M_PI * 2));

GELIB_API float PerlinNoise(float x, int num);

GELIB_API bool  IsRayIntersectPlane(const CVector3 &orig, const CVector3 &dir, const CPlane &plane, float *t=0);

inline bool IsRayIntersectBox(const CVector3 &orig, const CVector3 &dir, const CAABox &box, float *t=0)
{
    return GMhIsRayHitWithAABB(orig.m_v, dir.m_v, box.m_Min.m_v, box.m_Max.m_v, 0, t);
}

GELIB_API bool IsRayIntersectBox(const CVector3 &orig, const CVector3 &dir, const COBox &box, float *t=0);

inline bool IsRayIntersectSphere(const CVector3 &orig, const CVector3 &dir, const CSphere &sphere, float *t=0)
{
    return GMhIsRayHitSphere(orig.m_v, dir.m_v, sphere.m_Center.m_v, sphere.m_fRadius, 0, t);
}

inline bool IsRayIntersectTriangle(const CVector3 &orig, const CVector3 &dir, const CVector3 tri[3], bool ignoreBackFace, float *t=0)
{
    return GMhIsRayHitTriangle(orig.m_v, dir.m_v, tri[0].m_v, ignoreBackFace, 0, t);
}

GELIB_API bool IsLineIntersectBox(const CVector3 &p1, const CVector3 &p2, const COBox &box, float *t=0);

inline bool IsLineIntersectBox(const CVector3 &p0, const CVector3 &p1, const CAABox &box, float *t=0)
{
    return GMhIsLineIntersectWithAABB(p0.m_v, p1.m_v, box.m_Min.m_v, box.m_Max.m_v, 0, t);
}

inline bool IsLineIntersectTriangle(const CVector3 &l0, const CVector3 &l1, const CVector3 tri[3], bool ignoreBackFace, float *t=0)
{
    return GMhIsLineIntersectTriangle(l0.m_v, l1.m_v, tri[0].m_v, ignoreBackFace, 0, t);
}

GELIB_API bool IsLineIntersectSphere(const CVector3 &p1, const CVector3 &p2, const CSphere &sphere, float *t=0);


GELIB_API bool IsBoxIntersectSphere(const CAABox &box, const CSphere &sphere);
GELIB_API bool IsBoxIntersectSphere(const COBox &box, const CSphere &sphere);

inline bool IsSphereIntersectSphere(const CSphere &s1, const CSphere &s2)
{
    float r = s1.m_fRadius + s2.m_fRadius;
    return (s1.m_Center - s2.m_Center).LengthSqr() <= r * r;
}

GELIB_API bool IsBoxIntersectBox(const COBox &b1, const COBox &b2);

inline bool IsBoxIntersectBox(const CAABox &b1, const CAABox &b2)
{
    return b1.m_Min.x<=b2.m_Max.x && b1.m_Max.x>=b2.m_Min.x &&
           b1.m_Min.y<=b2.m_Max.y && b1.m_Max.y>=b2.m_Min.y &&
           b1.m_Min.z<=b2.m_Max.z && b1.m_Max.z>=b2.m_Min.z;
}


GELIB_API bool IsBoxIntersectTriangle(const CAABox &box, const CVector3 tri[3]);

GELIB_API void ClipPolygonByVolume(const std::vector<CVector3> &polygon, const CConvexVolume &volume, std::vector<CVector3> &result, float threshold=0);

GELIB_API bool TriangulatePolygon(const CVector2 *polygon, int sides, std::vector<short> &result);

}//GELib

#endif//__GELIB_MATH_GEMATH_H__
