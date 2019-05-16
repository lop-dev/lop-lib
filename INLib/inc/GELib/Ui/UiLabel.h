//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiLabel.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UILABEL_H__
#define __GELIB_UI_UILABEL_H__

#include "UiControl.h"

namespace GELib
{


class CUiLabel : public CUiControl
{
public:

	UICLASS_DECLARE(CUiLabel);

	CUiLabel(CUiSystem *system);
	virtual ~CUiLabel();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void DebugDraw(IPlotter2D &canvas);

protected:

};


}//GELib

#endif//__GELIB_UI_UILABEL_H__
