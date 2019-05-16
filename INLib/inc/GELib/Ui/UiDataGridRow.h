//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiDataGridRow.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIDATAGRIDROW_H__
#define __GELIB_UI_UIDATAGRIDROW_H__

#include "UiListItem.h"
#include "UiDataGridCell.h"
#include <vector>

namespace GELib
{


class CUiDataGridCell;

class CUiDataGridRow : public CUiListItem
{
public:

	UICLASS_DECLARE(CUiDataGridRow);

	CUiDataGridRow(CUiSystem *system);
	virtual ~CUiDataGridRow();

	CUiDataGridCell* AddCell(const wchar_t *txt);
	CUiDataGridCell* InsertCell(const wchar_t *txt, int index);
	void AddCells(int n);
	void RemoveCell(int index);
	int GetNumCells();
	CUiDataGridCell* GetCell(int index);

	void SetRowIndex(int idx) { m_iRowIndex = idx; }
	int GetRowIndex() { return m_iRowIndex; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

protected:

	int m_iRowIndex;
	std::vector<CUiDataGridCell*> m_Cells;

};


}//GELib

#endif//__GELIB_UI_UIDATAGRIDROW_H__
