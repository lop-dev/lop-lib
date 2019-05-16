//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Math/OBox.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_MATH_OBOX_H__
#define __GELIB_MATH_OBOX_H__

#include <GELib/GeConfig.h>
#include <GELib/Math/Vector.h>
#include <GELib/Math/Matrix.h>

namespace GELib
{

class COBox
{
public:
    GELIB_API COBox() {}
    GELIB_API COBox(const CMatrix &matrix, const CVector3 &extents)
    {
        m_Matrix = matrix;
        m_Extents = extents;
    }

    GELIB_API void Set(const CMatrix &matrix, const CVector3 &extents)
    {
        m_Matrix = matrix;
        m_Extents = extents;
    }

    GELIB_API bool IsContainPoint(const CVector3 &p) const;

    CMatrix m_Matrix;
    CVector3 m_Extents;
};


}//GELib

#endif//__GELIB_MATH_OBOX_H__
