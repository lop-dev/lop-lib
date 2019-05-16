//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiTextureSet.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UITEXTURESET_H__
#define __GELIB_UI_UITEXTURESET_H__

#include <string>
#include <GELib/Utility/StringMap.h>

namespace GELib
{

class CUiResourceManager;
class CUiTextureSet;
class CIOStream;
class CUiTexture;

class CUiSubTexture
{
public:
	CUiSubTexture(CUiTextureSet *ts);
	virtual ~CUiSubTexture();

	CUiTexture* GetTexture();
	CUiSubTexture* GetNeighbor(int index, float time=0);

	std::string m_Name;
	short m_iIndex;
	short m_iX, m_iY;
	short m_iWidth, m_iHeight;
	short m_iNumFrames;
	short m_iFps;

	CUiTextureSet *m_pTextureSet;
};

class CUiTextureSet
{
public:

	CUiTextureSet(CUiResourceManager *manager);
	virtual ~CUiTextureSet();

    bool Save(CIOStream &stream);
    bool Load(CIOStream &stream, bool loadTexture=true);
    void Unload();

	CUiSubTexture* Get(const char *name);
	void Add(CUiSubTexture *st);
	void Delete(CUiSubTexture *st);
	void BuildIndex();

	std::string m_Name;
	CUiTexture *m_pTexture;
	CUiResourceManager *m_pManager;

    typedef StringVectorMap<CUiSubTexture*> SubTextureMap;
    SubTextureMap m_SubTextures;
};


}//GELib

#endif//__GELIB_UI_UITEXTURESET_H__
