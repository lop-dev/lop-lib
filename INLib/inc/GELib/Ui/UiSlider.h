//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiSlider.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UISLIDER_H__
#define __GELIB_UI_UISLIDER_H__

#include "UiRangeBase.h"

namespace GELib
{


class CUiThumb;

class CUiSlider : public CUiRangeBase
{
public:

	UICLASS_DECLARE(CUiSlider);

	CUiSlider(CUiSystem *system);
	virtual ~CUiSlider();

	void SetOrientation(EOrientation o);
	EOrientation GetOrientation();

	virtual void SetValue(int v);

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void ApplyStyle(CUiStyle *s);
	virtual void DoLayout();
	virtual void ChooseStyleState();
	virtual void DebugDraw(IPlotter2D &canvas);

protected:

	int ThumbPositionToValue(float p);
	float ValueToThumbPosition(int v);

	virtual void EnabledChanged(bool enable);

	virtual void OnEvent(CUiEvent &e);
	virtual void OnMouseMove(CUiMouseEvent &e);
	virtual void OnMouseDown(CUiMouseEvent &e);
	virtual void OnMouseUp(CUiMouseEvent &e);

	EOrientation m_eOrientation;
	float m_fThumbSize;
	float m_fDragOffset;
	float m_fDragThumbPosition;
	bool m_bDragging;
	CUiThumb *m_pThumb;
};


}//GELib

#endif//__GELIB_UI_UISLIDER_H__
