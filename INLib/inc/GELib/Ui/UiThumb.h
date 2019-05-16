//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiThumb.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UITHUMB_H__
#define __GELIB_UI_UITHUMB_H__

#include "UiControl.h"

namespace GELib
{

enum EThumbFunction
{
	THUMB_FUNCTION_NONE,
	THUMB_FUNCTION_SCROLLBAR,
	THUMB_FUNCTION_SLIDER,
	THUMB_FUNCTION_MOVE_WINDOW,
	THUMB_FUNCTION_RESIZE_WINDOW_LEFT,
	THUMB_FUNCTION_RESIZE_WINDOW_RIGHT,
	THUMB_FUNCTION_RESIZE_WINDOW_TOP,
	THUMB_FUNCTION_RESIZE_WINDOW_BOTTOM,
	THUMB_FUNCTION_RESIZE_WINDOW_TOP_LEFT,
	THUMB_FUNCTION_RESIZE_WINDOW_TOP_RIGHT,
	THUMB_FUNCTION_RESIZE_WINDOW_BOTTOM_LEFT,
	THUMB_FUNCTION_RESIZE_WINDOW_BOTTOM_RIGHT,
};

class CUiThumb : public CUiControl
{
public:

	UICLASS_DECLARE(CUiThumb);

	CUiThumb(CUiSystem *system);
	virtual ~CUiThumb();

	bool IsDragging() { return m_bDragging; }

	void SetFunction(EThumbFunction f) { m_eFunction = f; }
	EThumbFunction GetFunction() { return m_eFunction; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void ChooseStyleState();
	virtual void DebugDraw(IPlotter2D &canvas);

protected:

	virtual void EnabledChanged(bool enable);

	virtual void OnEvent(CUiEvent &e);
	virtual void OnMouseDown(CUiMouseEvent &e);
	virtual void OnMouseUp(CUiMouseEvent &e);
	virtual void OnMouseMove(CUiMouseEvent &e);
	virtual void OnThumbBeginDrag(CUiEvent &e);
	virtual void OnThumbDrag(CUiEvent &e);
	virtual void OnThumbEndDrag(CUiEvent &e);

	EThumbFunction m_eFunction;
	bool m_bDragging;
};



}//GELib

#endif//__GELIB_UI_UITHUMB_H__
