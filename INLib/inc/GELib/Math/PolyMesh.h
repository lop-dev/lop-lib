//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Math/PolyMesh.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_MATH_POLYMESH_H__
#define __GELIB_MATH_POLYMESH_H__

#include <GELib/GeConfig.h>
#include <GELib/Math/GeMath.h>

#include <vector>

namespace GELib
{

class CConvexVolume;

class CPolyMesh
{
public:

    GELIB_API void BuildFromBox(const CAABox &box);
    GELIB_API void BuildFromBox(const COBox &box);
    GELIB_API void BuildFromTriMesh(const CVector3 *vertices, const int *indices, int numIndices);

    GELIB_API bool IntersectRay(const CVector3 &orig, const CVector3 &dir, bool ignoreBackFace, float *t=0, CVector3 *normal=0) const;
    GELIB_API bool IntersectLine(const CVector3 &p1, const CVector3 &p2, bool ignoreBackFace, float *t=0, CVector3 *normal=0) const;
    GELIB_API bool IntersectConvexVolume(const CConvexVolume &c) const;

    class Polygon
    {
    public:
        short m_iFirstIndex;
        short m_iNumSides;
    };

    std::vector<CVector3> m_Vertices;
    std::vector<short> m_Indices;
    std::vector<Polygon> m_Polygons;
};


}//GELib

#endif//__GELIB_MATH_POLYMESH_H__
