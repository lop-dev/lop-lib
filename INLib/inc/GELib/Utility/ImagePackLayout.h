//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/ImagePackLayout.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_IMAGEPACKLAYOUT_H__
#define __GELIB_UTILITY_IMAGEPACKLAYOUT_H__

#include <vector>
#include <GELib/GeConfig.h>

namespace GELib
{


class CImagePackLayout
{
public:
    GELIB_API CImagePackLayout();
    GELIB_API virtual ~CImagePackLayout();

    class CRect
    {
    public:
        CRect() {};
        CRect(int x, int y, int w, int h) { this->x=x; this->y=y; this->w=w; this->h=h; }
        int x, y, w, h;
    };

    GELIB_API void Initialize(int width, int height);
    GELIB_API bool Insert(int width, int height, CRect &result);

private:

    friend class CNode;

    class CNode
    {
    public:
        CNode() {}
        CNode* Insert(int w, int h);

        CImagePackLayout *m_pLayout;
        CNode *m_pChildren[2];
        CRect m_Rect;
        bool m_bUsed;
    };

    CNode* AddNode(const CRect &r);
    CNode *m_pRoot;
    int m_iFreeNodes;
    std::vector<CNode*> m_Nodes;
};



}//GELib

#endif//__GELIB_UTILITY_IMAGEPACKLAYOUT_H__
