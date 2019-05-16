//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiCheckBox.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UICHECKBOX_H__
#define __GELIB_UI_UICHECKBOX_H__

#include "UiButton.h"

namespace GELib
{


enum ECheckMode
{
	CHECK_MODE_NORMAL,		// check box
	CHECK_MODE_EXCLUSIVE,	// radio button
	CHECK_MODE_PROGRAM,		// don't change checked state by click
};

class CUiCheckBox : public CUiButton
{
public:

	UICLASS_DECLARE(CUiCheckBox);

	CUiCheckBox(CUiSystem *system);
	virtual ~CUiCheckBox();

	void SetCheckMode(ECheckMode m);
	ECheckMode GetCheckMode();

	virtual void SetChecked(bool checked);
	bool IsChecked();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void ChooseStyleState();
	virtual void DebugDraw(IPlotter2D &canvas);

protected:

	virtual void OnEvent(CUiEvent &e);
	virtual void OnClick(CUiEvent &e);
	virtual void OnCheckedChanged(CUiEvent &e);

	ECheckMode m_eCheckMode;
	bool m_bChecked;
};



}//GELib

#endif//__GELIB_UI_UICHECKBOX_H__
