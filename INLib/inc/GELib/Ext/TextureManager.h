//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/TextureManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_TEXTUREMANAGER_H__
#define __GELIB_EXT_TEXTUREMANAGER_H__

#include <string>
#include <map>

#include "BaseAssetManager.h"
#include "TextureAsset.h"
#include <GELib/Utility/StringMap.h>

namespace GELib
{

class CTextureSpec;



class CTextureManager : public CBaseAssetManager
{
public:

    CTextureManager();
    virtual ~CTextureManager();

    void SetSkipMipLevels(int skip);
    int  GetSkipMipLevels();

    CTextureAsset* Load(const char *fname, bool gammaCorrection, const CLoadParameter &param);

    CTextureAsset* Create(CTextureSpec &spec, const char *name, const char *cacheKey=0);
    CTextureAsset* Create(CRenderTargetSpec &spec, const char *name, const char *cacheKey=0);
    void Delete(CTextureAsset *tex);
    void DeleteLater(CTextureAsset *tex);

    typedef StringMap<CTextureAsset*> TextureCache;
	TextureCache& GetTextures() { return m_Cache[0]; }
	TextureCache& GetAnimatedTextures() { return m_Cache[1]; }

private:

	CTextureAsset* CreateTextureAsset(const char *name, const char *cacheKey=0);
	CAnimatedTextureAsset* CreateAnimatedTextureAsset(const char *name, const char *cacheKey=0);

    int m_iSkipMipLevels;

    TextureCache m_Cache[2];
};

void SetTextureManager(CTextureManager *m);
CTextureManager* GetTextureManager();


}//GELib

#endif//__GELIB_EXT_TEXTUREMANAGER_H__
