//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiRangeBase.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIRANGEBASE_H__
#define __GELIB_UI_UIRANGEBASE_H__

#include "UiControl.h"

namespace GELib
{


class CUiRangeBase : public CUiControl
{
public:

	UICLASS_DECLARE(CUiRangeBase);

	CUiRangeBase(CUiSystem *system);
	virtual ~CUiRangeBase();

	void SetRange(int rmin, int rmax);
	void GetRange(int &rmin, int &rmax);

	virtual void SetValue(int v);
	int GetValue();

	virtual void SetValueRatio(float r);	 // a = 0 ~ 1
	virtual float GetValueRatio();		// return 0 ~ 1

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

protected:

	virtual void OnEvent(CUiEvent &e);
	virtual void OnValueChanged(CUiEvent &e);

	int m_iRangeMin;
	int m_iRangeMax;
	int m_iValue;
};



}//GELib

#endif//__GELIB_UI_UIRANGEBASE_H__
