//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiPageTab.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIPAGETAB_H__
#define __GELIB_UI_UIPAGETAB_H__

#include "UiCheckBox.h"

namespace GELib
{

class CUiPageTab : public CUiCheckBox
{
public:

	UICLASS_DECLARE(CUiPageTab);

	CUiPageTab(CUiSystem *system);
	virtual ~CUiPageTab();

	void SetPageName(const char *name) { m_PageName = name; }
	void SetPageName(const std::string &name) { m_PageName = name; }
	const std::string& GetPageName() { return m_PageName; }

	virtual void SetChecked(bool checked);

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void DebugDraw(IPlotter2D &canvas);

protected:

	void ShowPage(bool show);

	std::string m_PageName;
};



}//GELib

#endif//__GELIB_UI_UIPAGETAB_H__
