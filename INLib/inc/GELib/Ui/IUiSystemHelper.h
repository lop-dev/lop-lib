//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/IUiSystemHelper.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_IUISYSTEMHELPER_H__
#define __GELIB_UI_IUISYSTEMHELPER_H__

#include <string>
#include <vector>
#include <GELib/Utility/DataBuffer.h>

namespace GELib
{

class CUiObject;

class IUiSystemHelper
{
public:
	IUiSystemHelper() {}
	virtual ~IUiSystemHelper() {}

	virtual void PlaySound(const char *name) = 0;
	virtual bool TranslateString(CUiObject *u, const std::wstring &key, std::wstring &value) = 0;
};


}//GELib

#endif//__GELIB_UI_IUISYSTEMHELPER_H__
