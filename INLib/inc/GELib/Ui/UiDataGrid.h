//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiDataGrid.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIDATAGRID_H__
#define __GELIB_UI_UIDATAGRID_H__

#include "UiSelector.h"
#include "UiDataGridColumn.h"
#include "UiDataGridRow.h"
#include <vector>

namespace GELib
{

class CUiScrollViewer;
class CUiThumb;
class CUiDataGridPainter;
class CUiDataGrid;

class IDataGridCompare
{
public:

	virtual bool Less(CUiDataGridRow *r1, CUiDataGridRow *r2, int column) = 0;
};

class CUiDataGrid : public CUiSelector
{
public:

	UICLASS_DECLARE(CUiDataGrid);

	CUiDataGrid(CUiSystem *system);
	virtual ~CUiDataGrid();

	CUiDataGridColumn* AddColumn(const wchar_t *txt);
	CUiDataGridColumn* InsertColumn(const wchar_t *txt, int index);
	void RemoveColumn(int index);
	int GetNumColumns();
	CUiDataGridColumn* GetColumn(int index);
	void RemoveAll();

	CUiDataGridRow* AddRow();
	CUiDataGridRow* InsertRow(int index);
	void RemoveRow(int index);
	int GetNumRows();
	CUiDataGridRow* GetRow(int index);
	void RemoveAllRows();

	CUiDataGridCell* GetCell(int row, int column);
	void SetCellText(int row, int column, const wchar_t *txt);
	void SetCellTextA(int row, int column, const char *txt);
	const std::wstring& GetCellText(int row, int column);
	std::string GetCellTextA(int row, int column);

	void SetSelectionMode(ESelectionMode m);
	ESelectionMode GetSelectionMode();

	// single selection
	virtual void Select(int index);
	virtual int GetSelectedIndex();
	CUiDataGridRow* GetSelectedRow();

	// multiple selection
	void SetSelected(int index, bool select);
	bool IsSelected(int index);

	// sort
	void SetCompareFunction(IDataGridCompare *c);
	void Sort(int column, bool increase);

	// resize column
	void EnableResizeColumn(bool e);
	bool IsEnableResizeColumn();
	void SetMinColumnWidth(float w);
	float GetMinColumnWidth();

	// scroll
	CUiScrollViewer* GetScrollViewer();
	void SetHScrollBarVisible(EScrollBarVisible v);
	EScrollBarVisible GetHScrollBarVisible();
	void SetVScrollBarVisible(EScrollBarVisible v);
	EScrollBarVisible GetVScrollBarVisible();
	void ScrollToRow(int index);
	void EnsureRowVisible(int index);
	void EnsureSelectedRowVisible();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void ApplyStyle(CUiStyle *s);
	virtual void DoLayout();
	virtual void DebugDraw(IPlotter2D &canvas);

protected:

	virtual void OnEvent(CUiEvent &e);
	virtual void OnKeyDown(CUiKeyEvent &e);

	int ComputeNumRowsInView();
	void RemoveDataContent();
	void DragResizeColumn(int index);

	std::vector<CUiDataGridColumn*> m_Columns;
	std::vector<CUiDataGridRow*> m_Rows;
	std::vector<CUiThumb*> m_ResizeThumbs;

	ESelectionMode m_eSelectionMode;
	CUiDataGridRow *m_pSelectedRow;
	int m_iSelectedIndex;

	IDataGridCompare *m_pCompare;

	float m_fHeaderHeight;
	float m_fRowHeight;

	CUiStyle *m_pHeaderStyle;
	CUiStyle *m_pRowStyle;
	CUiStyle *m_pCellStyle;
	CUiStyle *m_pResizeThumbStyle;

	CUiObject *m_pHeaderViewer;
	CUiScrollViewer *m_pDataViewer;
	CUiDataGridPainter *m_pGridPainter;

	bool m_bEnableResizeColumn;
	bool m_bDraging;
	float m_fDragX;
	float m_fDragOffset;
	float m_fMinColumnWidth;
};



}//GELib

#endif//__GELIB_UI_UIDATAGRID_H__
