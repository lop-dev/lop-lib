//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/Ime.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_IME_H__
#define __GELIB_UI_IME_H__

#include <GELib/Math/GeMath.h>

namespace GELib
{

class IPlotter2D;
class ITextureFont;
class CUiFont;

class CImeStyle
{
public:
	void SetDefault();
	CRGBAColor backgroundColor;
	CRGBAColor borderColor;
	CRGBAColor selectionColor;
	CRGBAColor caretColor;
	CRGBAColor underlineColor;
	CRGBAColor textColor;
	CRGBAColor selectedTextColor;
	CRGBAColor errorTextColor;
};

enum EImeUiMode
{
	IMEUI_SYSTEM,			// use Windows ime ui, don't work in full screen
	IMEUI_DRAW,				// use Plotter2D to draw all ime ui
};

class CIme
{
public:
	CIme();
	virtual ~CIme();

	bool Initialize(void *hWnd);
	void Uninitialize();

	void SetScreenSize(float w, float h);
	void SetFont(CUiFont *font);
	void SetStyle(const CImeStyle &style);
	void GetStyle(CImeStyle &style);

	void SetUiMode(EImeUiMode m);
	EImeUiMode GetUiMode();

	long ProcessMessage(void *hWnd, unsigned int msg, WParam wParam, LParam lParam, bool &processed);
	void Draw(IPlotter2D &canvas);

	// called by UI system
	void Enable(bool e=true);
	bool IsEnabled();
	void SetCaretPosition(int x, int y, int h);

private:
	void *m_hWnd;
	CUiFont *m_pFont;
	CVector2 m_Position;
	CVector2 m_ScreenSize;
	bool m_bInitialized;
	bool m_bDrawCaret;
	unsigned long m_iCaretBlinkTime;
	int m_iCaretPosition;
	CImeStyle m_Style;
	EImeUiMode m_eUiMode;
};


}//GELib


#endif//__GELIB_UI_IME_H__
