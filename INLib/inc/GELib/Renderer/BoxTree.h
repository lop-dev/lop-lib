//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/BoxTree.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_BOXTREE_H__
#define __GELIB_RENDERER_BOXTREE_H__

#include <vector>
#include <GELib/GeConfig.h>
#include <GELib/Math/GeMath.h>

namespace GELib
{

class CConvexVolume;

class CBoxTree
{
public:
    CBoxTree();
    ~CBoxTree();

    typedef short NodeIndex;        // NodeIndex < 0 means node is complete inside frustum
    typedef std::vector<NodeIndex> NodeIndices;

    class BoxNodes
    {
    public:
        BoxNodes() { m_iCount = 0; }
        unsigned char m_iCount;
        NodeIndex m_Indices[4];
        void Clear() { m_iCount = 0; }
        bool IsEmpty() const { return m_iCount <= 0; }
        void PushBack(NodeIndex index) { m_Indices[m_iCount++] = index; }
    };

    class Node
    {
    public:
        CAABox m_Box;

        bool IsContainBox(const CAABox &box) const
        {
            return box.m_Min.x>=m_Box.m_Min.x && box.m_Max.x<=m_Box.m_Max.x &&
                   box.m_Min.y>=m_Box.m_Min.y && box.m_Max.y<=m_Box.m_Max.y;
        }

        bool IsContainBoxXYZ(const CAABox &box) const
        {
            return box.m_Min.x>=m_Box.m_Min.x && box.m_Max.x<=m_Box.m_Max.x &&
                   box.m_Min.y>=m_Box.m_Min.y && box.m_Max.y<=m_Box.m_Max.y &&
                   box.m_Min.z>=m_Box.m_Min.z && box.m_Max.z<=m_Box.m_Max.z;
        }

        bool IsIntersectBox(const CAABox &box) const
        {
            return box.m_Min.x<=m_Box.m_Max.x && box.m_Max.x>=m_Box.m_Min.x &&
                   box.m_Min.y<=m_Box.m_Max.y && box.m_Max.y>=m_Box.m_Min.y;
        }

        bool IsIntersectBoxXYZ(const CAABox &box) const
        {
            return box.m_Min.x<=m_Box.m_Max.x && box.m_Max.x>=m_Box.m_Min.x &&
                   box.m_Min.y<=m_Box.m_Max.y && box.m_Max.y>=m_Box.m_Min.y &&
                   box.m_Min.z<=m_Box.m_Max.z && box.m_Max.z>=m_Box.m_Min.z;
        }

        void Clear() { m_Box.m_Min.z = 1e10f; m_Box.m_Max.z = -1e10f; }
        bool IsEmpty() const { return m_Box.m_Min.z > m_Box.m_Max.z; }
    };

    void Initialize(const CAABox &rootBox, const CVector3 &minBoxSize);

    void GetBoxNode(const CAABox &box, BoxNodes &result);
    void GetBoxNodes(const CAABox &box, BoxNodes &result);

    void IntersectConvexVolume(const CConvexVolume &v, NodeIndices &result);
    void IntersectBox(const CAABox &box, NodeIndices &result);

    Node& GetNode(int index) { return m_pNodes[index]; }
    int GetNumNodes() { return (int)m_iSize; }

    bool IsNodeIndex(NodeIndex index)
    {
        return index < m_iSize;
    }

    bool IsNodeHasChildren(NodeIndex index)
    {
        return index * 4 <= m_iSize;
    }

    static inline NodeIndex GetChildIndex(NodeIndex index)
    {
        return index * 4 + 1;
    }

    static inline NodeIndex GetParentIndex(NodeIndex index)
    {
        return (index - 1) / 4;
    }

private:

    void SplitNode_R(NodeIndex index, const CVector2 &bmin, const CVector2 &bmax);
    NodeIndex PushBox_R(NodeIndex index, const CAABox &box);
    void PushBox_R(NodeIndex index, const CAABox &box, BoxNodes &result);
    void IntersectConvexVolume_R(NodeIndex index, const CConvexVolume &v, NodeIndices &result);
    void IntersectBox_R(NodeIndex index, const CAABox &box, NodeIndices &result);
    void SetInsideNodes_R(NodeIndex index, NodeIndices &result);

	NodeIndex m_iSize;
    Node *m_pNodes;
    NodeIndices m_Stack;
};



}//GELib

#endif//__GELIB_RENDERER_BOXTREE_H__
