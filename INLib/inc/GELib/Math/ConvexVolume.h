//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Math/ConvexVolume.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_MATH_CONVEXVOLUME_H__
#define __GELIB_MATH_CONVEXVOLUME_H__

#include <GELib/GeConfig.h>
#include <GELib/Math/GeMath.h>

#include <vector>

namespace GELib
{

class CConvexVolume
{
public:

	GELIB_API bool IsIntersectSphere(const CSphere &s) const;
    // IsIntersectBox() is conservative test, may return true but not really intersect
	GELIB_API bool IsIntersectBox(const CAABox &box) const;
	GELIB_API bool IsIntersectBox(const COBox &box) const;
	GELIB_API bool IsIntersectPolygon(const CVector3 *p, int num) const;	///FIXME: not accurate
    GELIB_API bool IsIntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0) const;
    GELIB_API bool IsContainPoint(const CVector3 &p) const;
    GELIB_API bool IsContainPoints(const CVector3 *p, int numPoints) const;
    GELIB_API bool IsContainSphere(const CSphere &s) const;
    GELIB_API bool IsContainBox(const CAABox &box) const;
    GELIB_API bool IsExcludePoints(const CVector3 *p, int numPoints) const;
    GELIB_API int  GetBoxSide(const CAABox &box) const;
    GELIB_API int  GetSphereSide(const CSphere &s) const;
	GELIB_API void Transform(const CMatrix &matrix);	// planes *= matrix.Inverse().Transpose()
	GELIB_API void TransformT(const CMatrix &matrix);	// planes *= matrix.Transpose()

    enum
    {
        OUTSIDE = 0,
        INSIDE = 1,
        INTERSECT = 2
    };

	// all planes face outward
    // if m_Planes is empty, this volume will intersect or contain everything (always return true)
    std::vector<CPlane> m_Planes;
};



}//GELib

#endif//__GELIB_MATH_CONVEXVOLUME_H__
