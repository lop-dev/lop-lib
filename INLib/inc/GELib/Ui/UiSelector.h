//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiSelector.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UISELECTOR_H__
#define __GELIB_UI_UISELECTOR_H__

#include "UiControl.h"


namespace GELib
{

class CUiSelector : public CUiControl
{
public:

	UICLASS_DECLARE(CUiSelector);

	CUiSelector(CUiSystem *system);
	virtual ~CUiSelector();

	virtual void Select(int index);
	virtual int GetSelectedIndex();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

protected:

	virtual void OnEvent(CUiEvent &e);
	virtual void OnSelectionChanged(CUiEvent &e);
	virtual void OnCommitSelection(CUiEvent &e);
};


}//GELib


#endif//__GELIB_UI_UISELECTOR_H__
