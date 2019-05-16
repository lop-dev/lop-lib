//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiMenuItem.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIMENUITEM_H__
#define __GELIB_UI_UIMENUITEM_H__

#include "UiControl.h"

namespace GELib
{

class CUiMenu;

class CUiMenuItem : public CUiControl
{
public:

	UICLASS_DECLARE(CUiMenuItem);

	CUiMenuItem(CUiSystem *system);
	virtual ~CUiMenuItem();

	CUiMenuItem* AddItem(const wchar_t *txt);
	CUiMenuItem* AddItemA(const char *txt);
	CUiMenuItem* AddSeparator();
	CUiMenuItem* InsertItem(const wchar_t *txt, CUiMenuItem *after);
	CUiMenuItem* InsertItemA(const char *txt, CUiMenuItem *after);
	CUiMenuItem* InsertSeparator(CUiMenuItem *after);
	void RemoveItem(CUiMenuItem *item);
	void RemoveAll();

	int GetNumItems();
	CUiMenuItem* GetItem(int index);
	CUiMenuItem* GetParentItem() { return m_pParentItem; }

	void SetChecked(bool checked);
	bool IsChecked();

	bool IsSeparator();

	bool HasSubMenu();
	bool IsSubMenuOpened();
	void OpenSubMenu();
	void CloseSubMenu(bool closeAll);

	void SetShortcut(const wchar_t *txt);
	const std::wstring& GetShortcut();

	virtual const std::wstring& GetDisplayText(int index=0);
	virtual CUiTextFormat& GetTextFormat(int index);
	virtual CUiRichTextFormat& GetRichTextFormat(int index);

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void ApplyStyle(CUiStyle *s);
	virtual void ClosePopup();
	virtual void DoLayout();
	virtual void ChooseStyleState();

	virtual void DebugDraw(IPlotter2D &canvas);

protected:

    virtual bool SaveItem(CIOStream &stream);
    virtual bool LoadItem(CIOStream &stream);

	virtual void OnMouseDown(CUiMouseEvent &e);
	virtual void OnMouseUp(CUiMouseEvent &e);
	virtual void OnSubMenuKeyDown(CUiEvent &e);

	bool IsRootMenu();
	CUiMenuItem* InsertNewItem(CUiObject *after);
	float ComputeTextWidth();
	float ComputeSubMenuWidth();
	bool IsMouseInsideSelf();

	bool m_bPressed;
	bool m_bChecked;
	CUiTextFormat *m_pShortcutTextFormat;
	CUiRichTextFormat *m_pShortcutRichTextFormat;
	std::wstring m_ShortcutText;
	CUiMenu *m_pRootMenu;
	CUiMenuItem *m_pParentItem;
	CUiControl *m_pSubMenu;
};


}//GELib

#endif//__GELIB_UI_UIMENUITEM_H__
