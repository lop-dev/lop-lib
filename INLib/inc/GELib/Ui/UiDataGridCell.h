//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiDataGridCell.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIDATAGRIDCELL_H__
#define __GELIB_UI_UIDATAGRIDCELL_H__

#include "UiControl.h"

namespace GELib
{


class CUiDataGridCell : public CUiControl
{
public:

	UICLASS_DECLARE(CUiDataGridCell);

	CUiDataGridCell(CUiSystem *system);
	virtual ~CUiDataGridCell();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void DebugDraw(IPlotter2D &canvas);

protected:

};



}//GELib

#endif//__GELIB_UI_UIDATAGRIDCELL_H__
