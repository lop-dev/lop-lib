//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiTreeViewItem.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UITREEVIEWITEM_H__
#define __GELIB_UI_UITREEVIEWITEM_H__

#include "UiControl.h"

namespace GELib
{


class CUiTreeView;
class CUiCheckBox;
class CUiTreeViewItem;

class ITreeViewCompare
{
public:

	virtual bool Less(CUiTreeViewItem *item1, CUiTreeViewItem *item2) = 0;
};


class CUiTreeViewItem : public CUiControl
{
public:

	UICLASS_DECLARE(CUiTreeViewItem);

	CUiTreeViewItem(CUiSystem *system);
	virtual ~CUiTreeViewItem();

	void SetTreeView(CUiTreeView *tv);
	CUiTreeView* GetTreeView();

	CUiTreeViewItem* AddItem(const wchar_t *txt);
	CUiTreeViewItem* AddItemA(const char *txt);
	CUiTreeViewItem* InsertItem(const wchar_t *txt, CUiTreeViewItem *after);
	CUiTreeViewItem* InsertItemA(const char *txt, CUiTreeViewItem *after);
	void RemoveItem(CUiTreeViewItem *item);
	void RemoveAll();

	int GetNumItems();
	CUiTreeViewItem* GetItem(int index);
	CUiTreeViewItem* GetFirstItem();
	CUiTreeViewItem* GetLastItem();
	CUiTreeViewItem* GetParentItem() { return m_pParentItem; }

	void SetSelected(bool selected);
	bool IsSelected();

	bool HasSubTree();
	bool IsExpanded();
	void Expand();
	void Collapse();

	void Sort(ITreeViewCompare *compare=0);

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void ApplyStyle(CUiStyle *s);
	virtual void DoLayout();

	virtual void ChooseStyleState();
	virtual void DebugDraw(IPlotter2D &canvas);

protected:

    virtual bool SaveItem(CIOStream &stream);
    virtual bool LoadItem(CIOStream &stream);

	virtual void OnEvent(CUiEvent &e);
	virtual void OnMouseDown(CUiMouseEvent &e);
	virtual void OnMouseUp(CUiMouseEvent &e);
	virtual void OnExpandTree(CUiEvent &e);

	bool IsRootTree();
	CUiTreeViewItem* InsertNewItem(CUiObject *after);
	bool IsMouseInsideSelf();

	bool m_bPressed;
	bool m_bSelected;
	CUiTreeView *m_pTreeView;
	CUiTreeViewItem *m_pParentItem;
	CUiControl *m_pSubTree;
	CUiCheckBox *m_pExpandButton;
};



}//GELib

#endif//__GELIB_UI_UITREEVIEWITEM_H__
