//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/AssetHelper.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_ASSETHELPER_H__
#define __GELIB_EXT_ASSETHELPER_H__

#include <string>
#include <set>
#include <GELib/Renderer/RendererTypes.h>

namespace GELib
{

class CRenderMesh;
class CLightMapInfo;
class CCollisionShape;
class CFxAsset;
class CSkinMeshAsset;
class CAnimationAsset;
class CAnimationSet;
class CCharacterDesc;
class CCharacter;
class CBaseAsset;
class CTextureAsset;
class CMaterialAsset;
class CMeshAsset;
class CTerrainAsset;
class CNavMeshAsset;
class CActAsset;
class CScene;

class CAssetHelper
{
public:
    enum
    {
        ASSET_TEXTURE = 1 << 0,
        ASSET_MATERIAL = 1 << 1,
        ASSET_MESH = 1 << 2,
        ASSET_TERRAIN = 1 << 3,
        ASSET_NAVMESH = 1 << 4,
        ASSET_COLLISION_SHAPE = 1 << 5,
        ASSET_FX = 1 << 6,
        ASSET_SKIN = 1 << 7,
        ASSET_ANIMATION = 1 << 8,
        ASSET_ANIMATION_SET = 1 << 9,
        ASSET_CHARACTER = 1 << 10,
        ASSET_CHARACTER_DESC = 1 << 11,
        ASSET_SCENE = 1 << 12,
        ASSET_ACT = 1 << 13,
        ASSET_ALL = -1,
    };

	enum EPurpose
	{
		PURPOSE_NONE,
		PURPOSE_SAVE,
		PURPOSE_CACHE_SHADERS
	};

    CAssetHelper();
    virtual ~CAssetHelper();

	void SetPurpose(EPurpose p) { m_ePurpose = p; }
	EPurpose GetPurpose() { return m_ePurpose; }

	void IgnoreAdd(int assetTypes) { m_iIgnoreAdd = assetTypes; }

    void Add(CTextureAsset *texture);
    void Add(CMaterialAsset *material, CRenderMesh *mesh=0, CLightMapInfo *lightmap=0, EDrawMeshMode drawMeshMode=DRAW_MESH_NORMAL);
    void Add(CMeshAsset *mesh, CLightMapInfo *lightmap=0, EDrawMeshMode drawMeshMode=DRAW_MESH_NORMAL);
    void Add(CTerrainAsset *terrain);
    void Add(CNavMeshAsset *nm);
    void Add(CCollisionShape *col);
    void Add(CFxAsset *fx);
    void Add(CSkinMeshAsset *skin);
    void Add(CAnimationAsset *ani);
    void Add(CAnimationSet *as);
    void Add(CCharacterDesc *c);
	void Add(CCharacter *c);
	void Add(CScene *s);
	void Add(CActAsset *a);
    void Add(CAssetHelper &a);
    void AddLightMaterial(CMaterialAsset *material, int projectorLayer);

    void Clear();
	int GetNumAssets(int assetTypes=ASSET_ALL);

    void Save(const char *path, bool saveToPath=true);

    void CacheShaders(bool forceRecache=false);

    void IncreaseReferenceCounters();
    void DecreaseReferenceCounters();

private:

    class CMaterialMeshPair
    {
    public:
        CMaterialMeshPair();
        CMaterialMeshPair(CMaterialAsset *material, CRenderMesh *mesh, CLightMapInfo *lightmap, EDrawMeshMode drawMeshMode);
        bool operator<(const CMaterialMeshPair &m) const;

        CMaterialAsset *m_pMaterial;
        CRenderMesh *m_pMesh;
        CLightMapInfo *m_pLightMap;
		EDrawMeshMode m_eDrawMeshMode;
    };

	class CLightMaterial
	{
	public:
		CLightMaterial();
		CLightMaterial(CMaterialAsset *material, int projectorlayer);
        bool operator<(const CLightMaterial &m) const;

        CMaterialAsset *m_pMaterial;
		int m_iProjectorLayer;
	};

	EPurpose m_ePurpose;
	int m_iIgnoreAdd;

    std::set<CTextureAsset*> m_Textures;
    std::set<CMaterialMeshPair> m_Materials;
    std::set<CLightMaterial> m_LightMaterials;
    std::set<CMeshAsset*> m_Meshes;
    std::set<CTerrainAsset*> m_Terrains;
    std::set<CNavMeshAsset*> m_NavMeshes;
    std::set<CCollisionShape*> m_CollisionShapes;
    std::set<CFxAsset*> m_Fxs;
    std::set<CSkinMeshAsset*> m_Skins;
    std::set<CAnimationAsset*> m_Animations;
    std::set<CAnimationSet*> m_AnimationSets;
    std::set<CCharacterDesc*> m_CharacterDescs;
    std::set<CScene*> m_Scenes;
    std::set<CActAsset*> m_Acts;
    std::set<CBaseAsset*> m_Assets;
};


}//GELib

#endif//__GELIB_EXT_ASSETHELPER_H__
