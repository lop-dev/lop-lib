//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiTexture.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UITEXTURE_H__
#define __GELIB_UI_UITEXTURE_H__

#include <string>

namespace GELib
{

class CUiResourceManager;

class CUiTexture
{
public:
	CUiTexture(CUiResourceManager *manager=0);
	virtual ~CUiTexture();

	std::string m_Name;
	std::string m_CacheKey;
	int m_iResourceId;
    int m_iRefCount;
	int m_iWidth;
	int m_iHeight;
	bool m_bExternalResource;
	CUiResourceManager *m_pManager;
};


}//GELib

#endif//__GELIB_UI_UITEXTURE_H__
