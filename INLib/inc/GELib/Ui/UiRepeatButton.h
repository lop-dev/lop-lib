//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiRepeatButton.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIREPEATBUTTON_H__
#define __GELIB_UI_UIREPEATBUTTON_H__

#include "UiButton.h"

namespace GELib
{


class CUiRepeatButton : public CUiButton
{
public:

	UICLASS_DECLARE(CUiRepeatButton)

	CUiRepeatButton(CUiSystem *system);
	virtual ~CUiRepeatButton();

	void SetRepeatDelay(float t) { m_fRepeatDelay = t; }
	float GetRepeatDelay() { return m_fRepeatDelay; }
	void SetRepeatInterval(float t) { m_fRepeatInterval = t; }
	float GetRepeatInterval() { return m_fRepeatInterval; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void Update(float t);

protected:

	virtual void EnabledChanged(bool enable);

	virtual void OnMouseDown(CUiMouseEvent &e);
	virtual void OnKeyDown(CUiKeyEvent &e);

	float m_fRepeatDelay;
	float m_fRepeatInterval;
	float m_fRepeatTimer;
};



}//GELib

#endif//__GELIB_UI_UIREPEATBUTTON_H__
