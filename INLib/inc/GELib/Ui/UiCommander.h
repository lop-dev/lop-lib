//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiCommander.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UICOMMANDER_H__
#define __GELIB_UI_UICOMMANDER_H__

#include "UiObject.h"

namespace GELib
{


class CUiCommander : public CUiObject
{
public:

	UICLASS_DECLARE(CUiCommander);

	CUiCommander(CUiSystem *system);
	virtual ~CUiCommander();

	void SetCommand(const char *cmd) { m_Command = cmd; }
	const std::string& GetCommand() { return m_Command; }

	virtual void ApplyAnimationTrack(CUiAnimationTrack &tk, float t);

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

protected:

	virtual void OnEvent(CUiEvent &e);
	virtual void OnAnimationCommand(CUiCommandEvent &e);

	std::string m_Command;
};



}//GELib

#endif//__GELIB_UI_UICOMMANDER_H__
