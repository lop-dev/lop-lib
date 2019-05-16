//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiAniSlider.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIANISLIDER_H__
#define __GELIB_UI_UIANISLIDER_H__

#include "UiRangeBase.h"

namespace GELib
{


class CUiAniSlider : public CUiRangeBase
{
public:

	UICLASS_DECLARE(CUiAniSlider);

	CUiAniSlider(CUiSystem *system);
	virtual ~CUiAniSlider();

	void SetOrientation(EOrientation o);
	EOrientation GetOrientation();

	virtual void SetValue(int v);

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void ApplyStyle(CUiStyle *s);
	virtual void ChooseStyleState();
	virtual void Update(float t);
	virtual void DebugDraw(IPlotter2D &canvas);

protected:

	virtual void EnabledChanged(bool enable);

	virtual void OnMouseMove(CUiMouseEvent &e);
	virtual void OnMouseDown(CUiMouseEvent &e);
	virtual void OnMouseUp(CUiMouseEvent &e);

	int ThumbPositionToValue(float p);
	float ValueToThumbPosition(int v);

	EOrientation m_eOrientation;
	float m_fThumbSize;
	float m_fDragOffset;
	bool m_bDragging;
};



}//GELib

#endif//__GELIB_UI_UIANISLIDER_H__
