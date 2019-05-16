//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Math/Frustum.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_MATH_FRUSTUM_H__
#define __GELIB_MATH_FRUSTUM_H__

#include <GELib/GeConfig.h>
#include <GELib/Math/GeMath.h>
#include <GELib/Math/ConvexVolume.h>

namespace GELib
{


class GELIB_API CFrustum : public CConvexVolume
{
public:
	CFrustum();
	CFrustum(const CMatrix &viewProjMat);

	void Set(const CMatrix &viewProjMat);

    void GetCorners(CVector3 p[8]) const;

	enum
	{
		LEFT_PLANE  = 0,
		RIGHT_PLANE = 1,
		TOP_PLANE   = 2,
		BOTTOM_PLANE= 3,
		NEAR_PLANE  = 4,
		FAR_PLANE   = 5
	};
};

inline CFrustum::CFrustum()
{
    m_Planes.resize(6);
}

inline CFrustum::CFrustum(const CMatrix &viewProjMat)
{
	Set(viewProjMat);
}



}//GELib


#endif//__GELIB_MATH_FRUSTUM_H__
