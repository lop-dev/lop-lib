//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Math/AABox.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_MATH_AABOX_H__
#define __GELIB_MATH_AABOX_H__

#include <GELib/GeConfig.h>
#include <GELib/Math/Vector.h>

namespace GELib
{

class CAABox
{
public:
    GELIB_API CAABox() {}
    GELIB_API CAABox(const CVector3 &bmin, const CVector3 &bmax)
    {
        m_Min = bmin;
        m_Max = bmax;
    }

    GELIB_API void Set(const CVector3 &bmin, const CVector3 &bmax)
    {
        m_Min = bmin;
        m_Max = bmax;
    }

    GELIB_API bool IsContainPoint(const CVector3 &p) const;
    GELIB_API bool IsContainBox(const CAABox &b) const;

    GELIB_API void SetEmpty();
	GELIB_API bool IsEmpty() const { return m_Min.x >= m_Max.x; }

    GELIB_API void Expand(const CVector3 &p);
    GELIB_API void Expand(const CAABox &box);

	GELIB_API CVector3 Center() const { return (m_Min + m_Max) * 0.5f; }
	GELIB_API float Radius() const { return (m_Max - m_Min).Length() * 0.5f; }
	GELIB_API float Diameter() const { return (m_Max - m_Min).Length(); }

    CVector3 m_Min;
    CVector3 m_Max;
};


}//GELib

#endif//__GELIB_MATH_AABOX_H__
