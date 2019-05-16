//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/TextureAsset.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_TEXTUREASSET_H__
#define __GELIB_EXT_TEXTUREASSET_H__

#include <string>
#include <vector>

#include <GELib/GraphicDevice/GraphicDeviceTypes.h>
#include <GELib/Utility/DataBuffer.h>
#include "BaseAsset.h"

namespace GELib
{

class CTextureManager;

const int ASSETCLASS_TEXTURE = 1;
const int ASSETCLASS_ANIMATED_TEXTURE = 2;


class CTextureAsset : public CBaseAsset
{
public:
    CTextureAsset(CTextureManager *manager, const char *cacheKey=0);
    virtual ~CTextureAsset();

	virtual int GetClassId() { return ASSETCLASS_TEXTURE; }
    CTextureManager* GetManager() { return m_pManager; }
    virtual void ManagerDelete();


	virtual bool CreateLoadedResource();

    int m_iResourceId;
    short m_iWidth;
    short m_iHeight;
    EResourceType m_eResourceType;
    bool m_bGammaCorrection;

	class CLoadedResource
	{
	public:
		short m_iSkipMipLevels;
		CDataBuffer m_FileData;
	};
	CLoadedResource *m_pLoadedResource;

private:
    CTextureManager *m_pManager;
};

class CAnimatedTextureAsset : public CTextureAsset
{
public:
    CAnimatedTextureAsset(CTextureManager *manager=0, const char *cacheKey=0);
    virtual ~CAnimatedTextureAsset();

	virtual int GetClassId() { return ASSETCLASS_ANIMATED_TEXTURE; }

	void Unload();
	bool ParseLoadedResource(const CLoadParameter &param);
	virtual bool CreateLoadedResource();

    float m_fFps;
    std::vector<CTextureAsset*> m_Textures;
};


}//GELib


#endif//__GELIB_EXT_TEXTUREASSET_H__
