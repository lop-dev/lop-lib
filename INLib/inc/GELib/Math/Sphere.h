//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Math/Sphere.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_MATH_SPHERE_H__
#define __GELIB_MATH_SPHERE_H__

#include <GELib/GeConfig.h>
#include <GELib/Math/Vector.h>

namespace GELib
{

class CSphere
{
public:
    GELIB_API CSphere() {}
    GELIB_API CSphere(const CVector3 &center, float radius)
    {
        m_Center = center;
        m_fRadius = radius;
    }

    GELIB_API void Set(const CVector3 &center, float radius)
    {
        m_Center = center;
        m_fRadius = radius;
    }

    GELIB_API bool IsContainPoint(const CVector3 &p) const;
    GELIB_API bool IsContainSphere(const CSphere &s) const;

    CVector3 m_Center;
    float m_fRadius;
};



}//GELib

#endif//__GELIB_MATH_SPHERE_H__
