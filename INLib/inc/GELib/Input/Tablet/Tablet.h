//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Input/Tablet/Tablet.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_INPUT_TABLET_TABLET_H__
#define __GELIB_INPUT_TABLET_TABLET_H__

#include <GELib/GeConfig.h>

GELIB_API bool TabletInit(void *hWnd);
GELIB_API void TabletShutdown();

class CTabletData
{
public:
	float m_fX;
	float m_fY;
	float m_fPressure;
};

GELIB_API bool IsTabletMessage(unsigned int msg);
GELIB_API bool GetTabletData(WParam wParam, LParam lParam, CTabletData &data);



#endif//__GELIB_INPUT_TABLET_TABLET_H__
