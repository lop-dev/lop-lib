//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiButton.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIBUTTON_H__
#define __GELIB_UI_UIBUTTON_H__

#include "UiControl.h"

namespace GELib
{


enum EClickMode
{
	CLICK_MODE_PRESS,
	CLICK_MODE_RELEASE,
	CLICK_MODE_PROGRAM,
};

class CUiButton : public CUiControl
{
public:

	UICLASS_DECLARE(CUiButton)

	CUiButton(CUiSystem *system);
	virtual ~CUiButton();

	void SetClickMode(EClickMode m);
	EClickMode GetClickMode();

	void UseMouseInside(bool use);
	bool IsUseMouseInside();

	void SetPressed(bool pressed);
	bool IsPressed();

	void Click();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void ChooseStyleState();
	virtual void DebugDraw(IPlotter2D &canvas);

protected:

	virtual void EnabledChanged(bool enable);

	virtual void OnEvent(CUiEvent &e);
	virtual void OnMouseDown(CUiMouseEvent &e);
	virtual void OnMouseUp(CUiMouseEvent &e);
	virtual void OnKeyDown(CUiKeyEvent &e);
	virtual void OnKeyUp(CUiKeyEvent &e);
	virtual void OnClick(CUiEvent &e);

	bool IsMouseEnterOrInside();
	bool IsPressedAndMouseEnterOrInside();

	bool m_bPressed;
	bool m_bSpaceKeyDown;
	bool m_bUseMouseInside;
	EClickMode m_eClickMode;
};



}//GELib

#endif//__GELIB_UI_UIBUTTON_H__
