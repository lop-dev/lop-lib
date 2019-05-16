//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiListItem.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UILISTITEM_H__
#define __GELIB_UI_UILISTITEM_H__

#include "UiCheckBox.h"

namespace GELib
{


class CUiListBox;

class CUiListItem : public CUiCheckBox
{
public:

	UICLASS_DECLARE(CUiListItem);

	CUiListItem(CUiSystem *system);
	virtual ~CUiListItem();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void DebugDraw(IPlotter2D &canvas);

protected:

	virtual void OnEvent(CUiEvent &e);
};



}//GELib

#endif//__GELIB_UI_UILISTITEM_H__
