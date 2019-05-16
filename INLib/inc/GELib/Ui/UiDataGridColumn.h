//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiDataGridColumn.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIDATAGRIDCOLUMN_H__
#define __GELIB_UI_UIDATAGRIDCOLUMN_H__

#include "UiButton.h"

namespace GELib
{


enum ESortState
{
	SORT_NONE,
	SORT_INCREASE,
	SORT_DECREASE
};

class CUiDataGridColumn : public CUiButton
{
public:

	UICLASS_DECLARE(CUiDataGridColumn);

	CUiDataGridColumn(CUiSystem *system);
	virtual ~CUiDataGridColumn();

	void SetSortable(ESortState s);
	ESortState GetSortable();

	void SetSortState(ESortState s);
	ESortState GetSortState();

	void SetCellStyle(const char *name);
	const std::string& GetCellStyleName() { return m_CellStyleName; }
	CUiStyle* GetCellStyle() { return m_pCellStyle; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void ChooseStyleState();
	virtual void DebugDraw(IPlotter2D &canvas);

protected:

	ESortState m_eSortable;
	ESortState m_eSortState;
	std::string m_CellStyleName;
	CUiStyle *m_pCellStyle;

};


}//GELib

#endif//__GELIB_UI_UIDATAGRIDCOLUMN_H__
