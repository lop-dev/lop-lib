//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiCaret.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UICARET_H__
#define __GELIB_UI_UICARET_H__

#include "UiControl.h"

namespace GELib
{


class CUiCaret : public CUiControl
{
public:

	UICLASS_DECLARE(CUiCaret);

	CUiCaret(CUiSystem *system);
	virtual ~CUiCaret();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

protected:

};



}//GELib

#endif//__GELIB_UI_UICARET_H__
