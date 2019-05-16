//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/Window.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_WINDOW_H__
#define __GELIB_UTILITY_WINDOW_H__

#include <GELib/GeConfig.h>

#include <string>
#include <vector>
#include <stdarg.h>

namespace GELib
{


class CWindowSpec
{
public:
	CWindowSpec();
	int m_iWidth;
	int m_iHeight;
	void *m_hParentWindow;
	bool m_bVisible;

	bool m_bMaximizeBox;
	bool m_bMinimizeBox;
	bool m_bSizeBox;
};

class CWindow
{
public:
	CWindow();
	virtual ~CWindow();

	bool Create(const CWindowSpec &spec);
	void* GetHandle();

	virtual long WindowProc(unsigned int message, WParam wParam, LParam lParam, bool &processed);

	void SetVisible(bool v);
	void SetPosition(int x, int y, int w, int h);
	void GetPosition(int &x, int &y, int &w, int &h);
	void BringToTop();

	void SetTitle(const char *text);
	void SetTitle(const wchar_t *text);

	bool IsCursorOnWindow();

protected:
	void *m_hWindow;
};


}//GELib

#endif//__GELIB_UTILITY_WINDOW_H__
