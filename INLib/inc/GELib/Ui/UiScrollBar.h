//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiScrollBar.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UISCROLLBAR_H__
#define __GELIB_UI_UISCROLLBAR_H__

#include "UiRangeBase.h"
#include "UiTypes.h"


namespace GELib
{

class CUiRepeatButton;
class CUiThumb;

class CUiScrollBar : public CUiRangeBase
{
public:

	UICLASS_DECLARE(CUiScrollBar);

	CUiScrollBar(CUiSystem *system);
	virtual ~CUiScrollBar();

	void SetOrientation(EOrientation o);
	EOrientation GetOrientation();

	void SetThumbSize(float s);		// 0: auto size
	float GetThumbSize();

	void SetPageStep(int s);
	int GetPageStep();
	void SetLineStep(int s);
	int GetLineStep();

	virtual void SetValue(int v);

	virtual void SetValueRatio(float r);
	virtual float GetValueRatio();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void ApplyStyle(CUiStyle *s);
	virtual void DoLayout();
	virtual void DebugDraw(IPlotter2D &canvas);

protected:

	virtual void OnEvent(CUiEvent &e);

	bool IsScrollable();

	float ComputeButtonSize();
	float ComputeTrackSize();
	float ComputeThumbSize();
	int ThumbPositionToValue(float p);
	float ValueToThumbPosition(int v);

	int m_iPageStep;
	int m_iLineStep;
	float m_fThumbSize;
	float m_fDragOffset;
	float m_fDragThumbPosition;
	EOrientation m_eOrientation;

	CUiRepeatButton *m_pLineUpButton;
	CUiRepeatButton *m_pLineDownButton;
	CUiRepeatButton *m_pPageUpButton;
	CUiRepeatButton *m_pPageDownButton;
	CUiThumb *m_pThumb;
};



}//GELib

#endif//__GELIB_UI_UISCROLLBAR_H__
