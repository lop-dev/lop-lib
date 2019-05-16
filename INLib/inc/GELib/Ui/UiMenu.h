//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiMenu.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIMENU_H__
#define __GELIB_UI_UIMENU_H__

#include "UiMenuItem.h"

namespace GELib
{

class CUiMenu : public CUiMenuItem
{
public:

	UICLASS_DECLARE(CUiMenu);

	friend class CUiMenuItem;

	CUiMenu(CUiSystem *system);
	virtual ~CUiMenu();

	void SetDropDownWidth(float w);
	float GetDropDownWidth() { return m_fDropDownWidth; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void ApplyStyle(CUiStyle *s);

protected:

	float m_fItemWidth;
	float m_fItemHeight;
	float m_fItemHeight2;
	float m_fDropDownWidth;
	CVector2 m_DropDownOffset;
	float m_fPaddingLeft;
	float m_fPaddingTop;
	float m_fPaddingRight;
	float m_fPaddingBottom;

	CUiStyle *m_pItemStyle;
	CUiStyle *m_pSeparatorStyle;
	CUiStyle *m_pPopupStyle;
};



}//GELib

#endif//__GELIB_UI_UIMENU_H__
