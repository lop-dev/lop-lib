//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiGroup.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIGROUP_H__
#define __GELIB_UI_UIGROUP_H__

#include "UiObject.h"

namespace GELib
{


class CUiGroup : public CUiObject
{
public:

	UICLASS_DECLARE(CUiGroup);

	CUiGroup(CUiSystem *system);
	virtual ~CUiGroup();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);
};



}//GELib

#endif//__GELIB_UI_UIGROUP_H__
