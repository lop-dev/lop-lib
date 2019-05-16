//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/ProfileViewer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_PROFILEVIEWER_H__
#define __GELIB_UTILITY_PROFILEVIEWER_H__

#include <vector>
#include "CircularBuffer.h"
#include "Profiler.h"
#include "FpsCounter.h"

namespace GELib
{

class IPlotter2D;

class CProfileViewer
{
public:
    CProfileViewer();
    ~CProfileViewer();

    void Update();
    void Draw(GELib::IPlotter2D &canvas);

    void MouseClick(int x, int y);

    class History
    {
    public:
        short m_iCategoryIndex;
        float m_fTime;
    };

private:

    ProfileCategories m_Categories;
    ProfileSamples m_Samples;

	typedef std::vector<History> FrameHistroy;
    CircularBuffer<FrameHistroy> m_History;

    int m_iClickX;
    int m_iClickY;
    int m_iCategoryChangeIndex;
    int m_iCategoryChangeData;

	CFpsCounter m_FpsCounter;
};


}//GELib


#endif//__GELIB_UTILITY_PROFILEVIEWER_H__
