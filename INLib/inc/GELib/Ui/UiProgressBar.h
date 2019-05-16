//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiProgressBar.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIPROGRESSBAR_H__
#define __GELIB_UI_UIPROGRESSBAR_H__

#include "UiRangeBase.h"

namespace GELib
{

class CUiProgressBar : public CUiRangeBase
{
public:

	UICLASS_DECLARE(CUiProgressBar);

	CUiProgressBar(CUiSystem *system);
	virtual ~CUiProgressBar();

	virtual void SetValue(int v);

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void ChooseStyleState();
	virtual void Update(float t);
	virtual void DebugDraw(IPlotter2D &canvas);

protected:

};



}//GELib

#endif//__GELIB_UI_UIPROGRESSBAR_H__
