//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiScrollViewer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UISCROLLVIEWER_H__
#define __GELIB_UI_UISCROLLVIEWER_H__

#include "UiControl.h"

namespace GELib
{

class CUiScrollBar;

class CUiScrollViewer : public CUiControl
{
public:

	UICLASS_DECLARE(CUiScrollViewer);

	CUiScrollViewer(CUiSystem *system);
	virtual ~CUiScrollViewer();

	void SetContent(CUiObject *c, bool restoreScroll=false);
	CUiObject* GetContent();

	void SetHScrollBarVisible(EScrollBarVisible v);
	EScrollBarVisible GetHScrollBarVisible() { return m_eScrollBarVisible[0]; }
	void SetVScrollBarVisible(EScrollBarVisible v);
	EScrollBarVisible GetVScrollBarVisible() { return m_eScrollBarVisible[1]; }

	void SetHScrollBarSide(bool top);	// true: top, false: bottom
	bool GetHScrollBarSide() { return m_bScrollBarSide[0]; }
	void SetVScrollBarSide(bool left);	// true: left, false: right
	bool GetVScrollBarSide() { return m_bScrollBarSide[1]; }

	void SetHScrollBarSize(float s);
	float GetHScrollBarSize() { return m_fScrollBarSize[0]; }
	void SetVScrollBarSize(float s);
	float GetVScrollBarSize() { return m_fScrollBarSize[1]; }

	void SetHScrollUnit(float u);
	float GetHScrollUnit() { return m_fScrollUnit[0]; }
	void SetVScrollUnit(float u);
	float GetVScrollUnit() { return m_fScrollUnit[1]; }

	void SetHScrollLineStep(int s);
	int GetHScrollLineStep();
	void SetVScrollLineStep(int s);
	int GetVScrollLineStep();

	void SetHScrollBarValue(int v);
	int GetHScrollBarValue();
	void SetVScrollBarValue(int v);
	int GetVScrollBarValue();

	void SetHScrollBarValueRatio(float v);
	float GetHScrollBarValueRatio();
	void SetVScrollBarValueRatio(float v);
	float GetVScrollBarValueRatio();

	void SetHScrollKeepScrollRatio(bool keep) { m_bKeepScrollRatio[0] = keep; }
	bool IsHScrollKeepScrollRatio() { return m_bKeepScrollRatio[0]; }
	void SetVScrollKeepScrollRatio(bool keep) { m_bKeepScrollRatio[1] = keep; }
	bool IsVScrollKeepScrollRatio() { return m_bKeepScrollRatio[1]; }

	CUiScrollBar* GetHScrollBar();
	CUiScrollBar* GetVScrollBar();

	// scroll so rectangle is visible, in content coordinates
	void ScrollInView(const CVector2 &pos, const CVector2 &size);

	virtual CUiObject* GetContainer();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);
    virtual bool PostLoad();

	virtual void ApplyStyle(CUiStyle *s);
	virtual void DoLayout();

protected:

	virtual void OnEvent(CUiEvent &e);
	virtual void OnMouseWheel(CUiMouseEvent &e);
	virtual void OnScrollViewChanged(CUiEvent &e);

	void ScrollView();

	CUiControl *m_pView;
	CUiScrollBar *m_pScrollBar[2];
	EScrollBarVisible m_eScrollBarVisible[2];
	bool m_bScrollBarSide[2];
	float m_fScrollBarSize[2];
	float m_fScrollUnit[2];
	bool m_bKeepScrollRatio[2];
	bool m_bLayouting;
};



}//GELib

#endif//__GELIB_UI_UISCROLLVIEWER_H__
