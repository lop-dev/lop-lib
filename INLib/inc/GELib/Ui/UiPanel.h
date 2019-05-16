//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiPanel.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIPANEL_H__
#define __GELIB_UI_UIPANEL_H__

#include "UiControl.h"

namespace GELib
{


enum ELayoutMode
{
	LAYOUT_MODE_NONE,
	LAYOUT_MODE_STACK,
	LAYOUT_MODE_FLOW,
};

class CUiPanel : public CUiControl
{
public:
	UICLASS_DECLARE(CUiPanel);

	CUiPanel(CUiSystem *system);
	virtual ~CUiPanel();

	void SetLayoutMode(ELayoutMode m);
	ELayoutMode GetLayoutMode() { return m_eLayoutMode; }

	void SetLayoutOrientation(EOrientation o);
	EOrientation GetLayoutOrientation() { return m_eLayoutOrientation;  }

	void SetHorizontalSpacing(float s);
	float GetHorizontalSpacing() { return m_fHorizontalSpacing; }
	void SetVerticalSpacing(float s);
	float GetVerticalSpacing() { return m_fVerticalSpacing; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void DoLayout();
	virtual void DoAutoSize();

protected:

	ELayoutMode  m_eLayoutMode;
	EOrientation m_eLayoutOrientation;
	float m_fHorizontalSpacing;
	float m_fVerticalSpacing;
};




}//GELib

#endif//__GELIB_UI_UIPANEL_H__
