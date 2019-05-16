//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/CollisionMesh.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_COLLISIONMESH_H__
#define __GELIB_EXT_COLLISIONMESH_H__

#include <vector>
#include <GELib/Math/GeMath.h>

namespace GELib
{

class CIOStream;
class IPlotter3D;

// store in system memory
// no texture coordinate
// use aabb tree to speedup intersect query
// not used by physics

class CConvexVolume;
class CMeshUtility;
class CLoadParameter;

class CCollisionMesh
{
public:
    CCollisionMesh();
    virtual ~CCollisionMesh();

    bool Save(CIOStream &stream);
    bool Load(CIOStream &stream, const CLoadParameter &param);
    bool Load(CMeshUtility &meshUtil);
    void Unload();

    void DrawTree(IPlotter3D &plotter, const CMatrix &matrix);

    bool IntersectRay(const CVector3 &orig, const CVector3 &dir, bool ignoreBackFace, float *t=0, CVector3 *normal=0);
    bool IntersectLine(const CVector3 &p1, const CVector3 &p2, bool ignoreBackFace, float *t=0, CVector3 *normal=0);
    bool IntersectConvexVolume(const CConvexVolume &c);
	void GetTrianglesInBox(const CAABox &box, std::vector<CTriangle> &triangles);

    int  GetNumVertices();
    void GetVertexPosition(int vertexIndex, CVector3 &p);
    int  GetNumTriangles();
    void GetTriangle(int triangleIndex, CVector3 tri[3]);

private:

    class Vertex
    {
    public:
        CVector3 m_Position;
    };

    class CAABBTreeNode
    {
    public:
        CAABox m_Box;
        int m_iChildren[2];

        bool IsLeaf() const { return m_iChildren[0] <= 0; }
        void SetNumIndices(int n) { m_iChildren[1] = -n ; }
        int  GetNumIndices() const { return -m_iChildren[1]; }
        void SetFirstTriangleIndex(int i) { m_iChildren[0] = -i; }
        int  GetFirstTriangleIndex() const { return -m_iChildren[0]; }
    };

    typedef std::vector<unsigned short> TriangleIndices;

    int AppendVertex(const CVector3 &position);
    void ComputeBox(const TriangleIndices &indices, CAABox &box);
    void Divide_R(int nodeIndex, const TriangleIndices &indices, int maxDepth, int maxTriangles);

    std::vector<Vertex> m_Vertices;
    TriangleIndices m_Indices;
    std::vector<CAABBTreeNode> m_AABBTree;
};



}//GELib

#endif//__GELIB_EXT_COLLISIONMESH_H__
