//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiDataGridPainter.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIDATAGRIDPAINTER_H__
#define __GELIB_UI_UIDATAGRIDPAINTER_H__

#include "UiControl.h"

namespace GELib
{


class CUiDataGrid;

class CUiDataGridPainter : public CUiControl
{
public:

	UICLASS_DECLARE(CUiDataGridPainter);

	CUiDataGridPainter(CUiSystem *system);
	virtual ~CUiDataGridPainter();

	void SetDataGrid(CUiDataGrid *dg) { m_pDataGrid = dg; }

	void SetLineColor(CRGBAColor c1, CRGBAColor c2);
	void SetVisibleRows(int from, int to) { m_iFromRow = from; m_iToRow = to; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void Draw(IPlotter2D &canvas);

protected:

	int m_iFromRow;
	int m_iToRow;
	CUiDataGrid *m_pDataGrid;
	CRGBAColor m_LineColor[2];

};


}//GELib

#endif//__GELIB_UI_UIDATAGRIDPAINTER_H__
