//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/AssetCache.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_ASSETCACHE_H__
#define __GELIB_EXT_ASSETCACHE_H__

#include "MeshAsset.h"

#include <string>
#include <vector>

namespace GELib
{


class CCollisionShape;
class CFxAsset;
class CSkinMeshAsset;
class CAnimationAsset;
class CAnimationSet;
class CCharacter;
class CScene;
class CTextureAsset;
class CMaterialAsset;
class CTerrainAsset;
class CActAsset;

class CAssetCache
{
public:

    CAssetCache();
    virtual ~CAssetCache();

    void Clear();

    CTextureAsset* CacheTexture(const char *name, bool gammaCorrection, CLoadParameter &param);
    CMaterialAsset* CacheMaterial(const char *name, CLoadParameter &param);
    CMeshAsset* CacheMesh(const char *name, CMeshAsset::ERenderType rt, CLoadParameter &param);
    CTerrainAsset* CacheTerrain(const char *name, CLoadParameter &param);
    CCollisionShape* CacheCollisionShape(const char *name, CLoadParameter &param);
    CFxAsset* CacheFx(const char *name, CLoadParameter &param);
    CSkinMeshAsset* CacheSkin(const char *name, CLoadParameter &param);
    CAnimationAsset* CacheAnimation(const char *name, CLoadParameter &param);
    CAnimationSet* CacheAnimationSet(const char *name, CLoadParameter &param);
    CCharacter* CacheCharacter(const char *name, CLoadParameter &param);
    CScene* CacheScene(const char *name, CLoadParameter &param);
    CActAsset* CacheAct(const char *name, CLoadParameter &param);

private:

    std::vector<CTextureAsset*> m_Textures;
    std::vector<CMaterialAsset*> m_Materials;
    std::vector<CMeshAsset*> m_Meshes;
    std::vector<CTerrainAsset*> m_Terrains;
    std::vector<CCollisionShape*> m_CollisionShapes;
    std::vector<CFxAsset*> m_Fxs;
    std::vector<CSkinMeshAsset*> m_Skins;
    std::vector<CAnimationAsset*> m_Animations;
    std::vector<CAnimationSet*> m_AnimationSets;
    std::vector<CCharacter*> m_Characters;
    std::vector<CScene*> m_Scenes;
    std::vector<CActAsset*> m_Acts;
};


}//GELib

#endif//__GELIB_EXT_ASSETCACHE_H__
