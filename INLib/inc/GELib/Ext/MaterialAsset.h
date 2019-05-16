//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/MaterialAsset.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_MATERIALASSET_H__
#define __GELIB_EXT_MATERIALASSET_H__

#include <vector>
#include <string>

#include <GELib/Renderer/StdMaterial.h>
#include "BaseAsset.h"

namespace GELib
{

class CIOStream;
class CAssetHelper;
class CMaterialManager;
class CTextureAsset;

const int ASSETCLASS_MATERIAL = 3;

class CMaterialAsset : public CBaseAsset
{
public:

    CMaterialAsset(CMaterialManager *manager=0, const char *cacheKey=0);
    virtual ~CMaterialAsset();

	virtual int GetClassId() { return ASSETCLASS_MATERIAL; }
    CMaterialManager* GetManager() const { return m_pManager; }
    virtual void ManagerDelete();

    bool Save(CIOStream &stream);
	bool Load(CIOStream &stream, const CLoadParameter &param);
    void Unload();
	virtual bool CreateLoadedResource();

    void SetTexture(int index, CTextureAsset *tex, bool deleteOld=true);      // SetTexture(index, 0) to clear
    int GetNumTextures();
    CTextureAsset* GetTexture(int index);
    CStdMaterial* GetRenderMaterial() { return &m_RenderMaterial; }

    CMaterialAsset* GetTemplateMaterial() const { return m_pTemplateMaterial; }
    void SetTemplateMaterial(CMaterialAsset *m, bool deleteOld=true);
    void CopyTemplateMaterialProperties();    // used by tool

	void SetDescription(const char *c) { if (c) m_Description = c; }
	const char * GetDescription() const { return m_Description.c_str(); }

    bool HasSpecular();
    bool HasNormal();
	bool HasLighting();
	bool CheckShaderTexture();

    void GetAssets(CAssetHelper &helper);

private:

    CMaterialAsset *m_pTemplateMaterial;
    std::vector<CTextureAsset*> m_Textures;
	std::string m_Description;
    CStdMaterial m_RenderMaterial;
    CMaterialManager *m_pManager;
	bool m_bNeedCreateLoadedResource;
};

inline int CMaterialAsset::GetNumTextures()
{
    return (int)m_Textures.size();
}

inline CTextureAsset* CMaterialAsset::GetTexture(int index)
{
    if (index>=0 && index<(int)m_Textures.size())
        return m_Textures[index];
    else
        return 0;
}


}//GELib


#endif//__GELIB_EXT_MATERIALASSET_H__
