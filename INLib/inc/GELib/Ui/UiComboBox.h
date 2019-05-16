//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiComboBox.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UICOMBOBOX_H__
#define __GELIB_UI_UICOMBOBOX_H__

#include "UiSelector.h"


namespace GELib
{


class CUiButton;
class CUiListBox;
class CUiListItem;
class IListCompare;

class CUiComboBox : public CUiSelector
{
public:

	UICLASS_DECLARE(CUiComboBox);

	CUiComboBox(CUiSystem *system);
	virtual ~CUiComboBox();

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

	virtual void Select(int index);
	virtual int GetSelectedIndex();
	void SelectItem(CUiListItem *item);
	void SelectByValue(int value);

	const std::wstring& GetSelectedText();
	std::string GetSelectedTextA();
	int GetSelectedValue();
	CUiListItem* GetSelectedItem();

	void Sort(IListCompare *compare=0);

	// appearance
	void SetDropDownWidth(float w);
	float GetDropDownWidth() { return m_fDropDownWidth; }
	void SetMaxDropDownItems(int n);
	int GetMaxDropDownItems() { return m_iMaxDropDownItems; }

	// scroll
	void SetScrollBarVisible(EScrollBarVisible v);
	EScrollBarVisible GetScrollBarVisible();
	void ScrollToItem(int index);
	void EnsureItemVisible(int index);
	void EnsureSelectedItemVisible();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void ApplyStyle(CUiStyle *s);
	virtual void ClosePopup();
	virtual void DoLayout();

protected:

	virtual void OnEvent(CUiEvent &e);
	virtual void OnMouseDown(CUiMouseEvent &e);
	virtual void OnMouseUp(CUiMouseEvent &e);

	void OpenDropDown();
	void CloseDropDown();
	bool IsDropDownOpened();
	void OnListSelect(CUiEvent &e);
	void OnListKeyDown(CUiEvent &e);

	float m_fDropDownWidth;
	int m_iMaxDropDownItems;
	CUiButton *m_pButton;
	CUiListBox *m_pListBox;
};



}//GELib

#endif//__GELIB_UI_UICOMBOBOX_H__
