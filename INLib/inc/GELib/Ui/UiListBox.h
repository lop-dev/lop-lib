//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiListBox.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UILISTBOX_H__
#define __GELIB_UI_UILISTBOX_H__

#include "UiSelector.h"
#include "UiListItem.h"
#include <vector>

namespace GELib
{

class CUiScrollViewer;
class CUiListItem;

class IListCompare
{
public:

	virtual bool Less(CUiListItem *item1, CUiListItem *item2) = 0;
};


class CUiListBox : public CUiSelector
{
public:

	UICLASS_DECLARE(CUiListBox);

	CUiListBox(CUiSystem *system);
	virtual ~CUiListBox();

	CUiListItem* AddItem(const wchar_t *txt, int value=0);
	CUiListItem* AddItemA(const char *txt, int value=0);
	CUiListItem* InsertItem(const wchar_t *txt, int value, int index);
	CUiListItem* InsertItemA(const char *txt, int value, int index);
	void RemoveItem(int index);
	void RemoveAll();
	int GetNumItems();

	const std::wstring& GetItemText(int index);
	std::string GetItemTextA(int index);
	int GetItemValue(int index);
	CUiListItem* GetItem(int index);
	int GetItemIndex(CUiListItem *item);

	void SetSelectionMode(ESelectionMode m);
	ESelectionMode GetSelectionMode();

	// single selection
	virtual void Select(int index);
	virtual int GetSelectedIndex();
	void SelectItem(CUiListItem *item);
	void SelectByValue(int value);

	const std::wstring& GetSelectedText();
	std::string GetSelectedTextA();
	int GetSelectedValue();
	CUiListItem* GetSelectedItem();

	// multiple selection
	void SetSelected(int index, bool select);
	bool IsSelected(int index);

	void Sort(IListCompare *compare=0);

	// appearance
	float GetItemHeight() { return m_fItemHeight; }

	// scroll
	CUiScrollViewer* GetScrollViewer();
	void SetScrollBarVisible(EScrollBarVisible v);
	EScrollBarVisible GetScrollBarVisible();
	void ScrollToItem(int index);
	void EnsureItemVisible(int index);
	void EnsureSelectedItemVisible();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void ApplyStyle(CUiStyle *s);
	virtual void DoLayout();
	virtual void DebugDraw(IPlotter2D &canvas);

protected:

	virtual void OnEvent(CUiEvent &e);
	virtual void OnKeyDown(CUiKeyEvent &e);

	int ComputeNumItemsInView();
	void RemoveContent();

	ESelectionMode m_eSelectionMode;
	CUiListItem *m_pSelectedItem;
	int m_iSelectedIndex;

	float m_fItemHeight;
	CUiStyle *m_pItemStyle;

	std::vector<CUiListItem*> m_Items;
	CUiScrollViewer *m_pViewer;
};



}//GELib

#endif//__GELIB_UI_UILISTBOX_H__
