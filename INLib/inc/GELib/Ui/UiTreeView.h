//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiTreeView.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UITREEVIEW_H__
#define __GELIB_UI_UITREEVIEW_H__

#include "UiTreeViewItem.h"

namespace GELib
{


class CUiScrollViewer;

class CUiTreeView : public CUiControl
{
public:

	UICLASS_DECLARE(CUiTreeView);

	friend class CUiTreeViewItem;

	CUiTreeView(CUiSystem *system);
	virtual ~CUiTreeView();

	CUiTreeViewItem* AddItem(const wchar_t *txt);
	CUiTreeViewItem* AddItemA(const char *txt);
	CUiTreeViewItem* InsertItem(const wchar_t *txt, CUiTreeViewItem *after);
	CUiTreeViewItem* InsertItemA(const char *txt, CUiTreeViewItem *after);
	void RemoveItem(CUiTreeViewItem *item);
	void RemoveAll();

	int GetNumItems();		// not including sub items
	CUiTreeViewItem* GetItem(int index);
	CUiTreeViewItem* GetRootItem();

	void SelectItem(CUiTreeViewItem *item);
	CUiTreeViewItem* GetSelectedItem();

	// scroll
	CUiScrollViewer* GetScrollViewer();
	void SetHScrollBarVisible(EScrollBarVisible v);
	EScrollBarVisible GetHScrollBarVisible();
	void SetVScrollBarVisible(EScrollBarVisible v);
	EScrollBarVisible GetVScrollBarVisible();
	void EnsureItemVisible(CUiTreeViewItem *item);

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void ApplyStyle(CUiStyle *s);
	virtual void DoLayout();

	virtual void DebugDraw(IPlotter2D &canvas);

protected:

	virtual void OnEvent(CUiEvent &e);
	virtual void OnSelectionChanged(CUiEvent &e);
	virtual void OnKeyDown(CUiKeyEvent &e);

	// called by CUiTreeViewItem
	void ItemRemoved(CUiTreeViewItem *next);

	CUiTreeViewItem* GetNextVisibleItem(CUiTreeViewItem *u);
	CUiTreeViewItem* GetPrevVisibleItem(CUiTreeViewItem *u);
	int ComputeNumItemsInView();

	CUiTreeViewItem *m_pSelectedItem;
	CUiTreeViewItem *m_pRootItem;
	CUiStyle *m_pItemStyle;
	CUiStyle *m_pExpandButtonStyle;
	CUiScrollViewer *m_pViewer;
	float m_fItemWidth;
	float m_fItemHeight;
	float m_fIndent;
};


}//GELib

#endif//__GELIB_UI_UITREEVIEW_H__
