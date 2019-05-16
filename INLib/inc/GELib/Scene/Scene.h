//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Scene/Scene.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_SCENE_SCENE_H__
#define __GELIB_SCENE_SCENE_H__

#include <vector>
#include <string>
#include <GELib/Math/RandomNumber.h>
#include <GELib/Renderer/Renderer.h>
#include <GELib/Ext/BaseAsset.h>
#include "SceneEntity.h"
#include "PortalSceneEntity.h"
#include "EnvironmentSceneEntity.h"
#include "GroupSceneEntity.h"
#include "GameObjectSceneEntity.h"
#include "InstanceMeshSceneEntity.h"
#include "TerrainSceneEntity.h"
// modified by bob -- start
#include "WaterSceneEntity.h"
// modified by bob -- end
#include "SceneLayerOption.h"

namespace GELib
{

class CIOStream;
class CLoadTask;
class CObjectPropertyTable;
class IPlotter3D;
class IPlotter2D;
class CPhysicsWorld;
class CFxPlayer;
class CRenderWorld;
class CTextureAsset;
class CNavMeshAsset;


const int SCENE_DRAWFLAG_EDITOR = 1 << 0;
const int SCENE_DRAWFLAG_RANGE = 1 << 1;
const int SCENE_DRAWFLAG_COLLISION = 1 << 2;
const int SCENE_DRAWFLAG_COLLISION_SOLID = 1 << 3;
const int SCENE_DRAWFLAG_COLLISION_MASK = SCENE_DRAWFLAG_COLLISION | SCENE_DRAWFLAG_COLLISION_SOLID;
const int SCENE_DRAWFLAG_NAVIGATION = 1 << 8;

class CSceneManager;

class CLightMapSettings
{
public:
    CLightMapSettings();
	void Reset();

    ELightMapMode m_eMode;
    float m_fLightMapResolutionFactor;
    float m_fShadowDepthResolutionFactor;

    bool m_bAmbientOcclusion;
    bool m_bAreaLight;
    float m_fAmbientOcclusionQuality;
    float m_fAmbientOcclusionShadowDepthBias;
    float m_fAreaLightQuality;
};

class CSceneEntityLightMap
{
public:
    CSceneEntityLightMap() { m_iEntityId = 0; m_iLod = 0; m_iTextureIndex[0] = m_iTextureIndex[1] = -1; }
    int m_iEntityId;
    unsigned short m_iLod;
    int m_iTextureIndex[2];
    CLightMapInfo m_Map;
};

class CShadowDepthMapInfoSettings
{
public:
    CShadowDepthMapInfoSettings();
	void Reset();

	float m_fResolutionFactor;
};

class CSceneEntityShadowDepthMap
{
public:
	CSceneEntityShadowDepthMap() { m_iEntityId = 0; }
    int m_iEntityId;
    CShadowDepthMapInfo m_Map;
	std::vector<unsigned short> m_Depth;
};

class CNavigationSettings
{
public:
	CNavigationSettings();
	void Reset();

	bool m_bEnable;

	float m_fActorWidth;
	float m_fActorHeight;
	float m_fClimbHeight;
	float m_fClimbSlope;

	int m_iTileSize;
	float m_fCellSize;
	float m_fCellHeight;
	float m_fDetailSampleDist;
	float m_fDetailSampleMaxError;
};

const int MAX_NAVIGATION_MESHES = 8;

typedef std::vector<CSceneEntity*> SceneEntityList;

const int ASSETCLASS_SCENE = 11;

class CScene : public CBaseAsset
{
public:
    CScene(CSceneManager *manager=0, const char *cacheKey=0);
    virtual ~CScene();

    void Initialize(CRenderWorld *renderWorld, CPhysicsWorld *physicsWorld, CFxPlayer *fxPlayer);
    void Uninitialize();

	virtual int GetClassId() { return ASSETCLASS_SCENE; }
    CSceneManager* GetManager() { return m_pManager; }
    virtual void ManagerDelete();

    CRenderWorld* GetRenderWorld() { return m_pRenderWorld; }
    CPhysicsWorld* GetPhysicsWorld() { return m_pPhysicsWorld; }
    CFxPlayer* GetFxPlayer() { return m_pFxPlayer; }
    CRandomNumber& GetRandom() { return m_Random; }

    // save / load
    bool Save(const char *fname);
    bool Load(const char *fname, const CLoadParameter &param);
    void Unload();
	virtual bool CreateLoadedResource();

    bool Save(CIOStream &stream);
    bool Load(CIOStream &stream, const CLoadParameter &param);

    bool Preload(const char *fname, const CLoadParameter &param);
    void ShowPreloaded();

    // assets include: texture, material, mesh, collision
    bool SaveAll(const char *fname, bool gatherAssets=false, bool gatherExternalAssets=false);    // save scene file and all asset files
    bool SaveEntities(SceneEntityList &entities, const char *fname, bool gatherAssets=false, bool gatherExternalAssets=false);    // save entitis and their asset files

    void Update(float t);

    // editor draw
    void SetDrawFlag(int f);
    int GetDrawFlag();
    void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    // collision (for editor, use render mesh instead of physics mesh)
    CSceneEntity* RayHit(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0, int classIds=-1, bool checkLayer=true);
    CSceneEntity* LineHit(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0, int classIds=-1, bool checkLayer=true);
	int IntersectConvexVolume(const GELib::CConvexVolume &c, SceneEntityList &entities, int classIds=-1, bool checkLayer=true);

    // create entity
    CMeshSceneEntity* CreateMesh(const char *fname, bool replaceExist=false);
    CCharacterSceneEntity* CreateCharacter(const char *fname);
    CDecalSceneEntity* CreateDecal(const char *fname, bool replaceExist=false);
    CTreeSceneEntity* CreateTree(const char *fname, bool replaceExist=false);
    CInstanceMeshSceneEntity* CreateInstanceMesh(const char *fname, bool replaceExist=false);
    CTerrainSceneEntity* CreateTerrain(int size, float scale, float maxHeight);
    CLightSceneEntity* CreateLight();
    CFxSceneEntity* CreateFx(const char *fname);
    CSoundSceneEntity* CreateSound(const char *name);
    CEnvironmentSceneEntity* CreateEnvironment();
    COccluderSceneEntity* CreateOccluder(const char *fname);
	COccluderSceneEntity* CreateBoxOccluder(const CAABox &box);
    CZoneSceneEntity* CreateZone(const char *fname);
	CZoneSceneEntity* CreateBoxZone(const CAABox &box);
    CPortalSceneEntity* CreatePortal(const char *fname);
	CPortalSceneEntity* CreateBoxPortal(const CAABox &box);
    CGroupSceneEntity* CreateGroup();
    CGroupSceneEntity* CreateGroup(const char *fname, int classIds=SCENEENTITY_ALL, bool replaceExist=false);
    CLocatorSceneEntity* CreateLocator(const char *name);
    CCameraSceneEntity* CreateCamera(const char *name);
    CGameObjectSceneEntity* CreateGameObject(const char *name);
// modified by bob -- start
    CWaterEntity* CreateWater(const SWaterDesc& _desc);
// modified by bob -- end

    // entity management
    CSceneEntity* CreateEntity(int classId, int entityId=0);
    bool DeleteEntity(CSceneEntity *e);
    bool IsEntityExist(CSceneEntity *e);
    int GetNumEntities();
    CSceneEntity* GetEntityByIndex(int index);
    CSceneEntity* GetEntityById(int id);
    CSceneEntity* GetEntityByName(const char *name, int classIds=SCENEENTITY_ALL);
    int GetEntitiesByName(const char *name, SceneEntityList &entities, int classIds=SCENEENTITY_ALL);

    // environment
    void SetActiveEnvironment(CEnvironmentSceneEntity *e) { m_pActiveEnvironment = e; }
    CEnvironmentSceneEntity* GetActiveEnvironment() { return m_pActiveEnvironment; }
    void SetShadowMethod(int method);

	//
	void AddGrassCollider(const CVector3 &position, float radius);

    // bounding, updated after save or load
    const CAABox& GetBoundingBox() { return m_Box; }

    // lightmap
    CLightMapSettings& GetLightMapSettings() { return m_LightMapSettings; }
    void RenderLightMap(CRenderer *renderer, CRenderLightMapContext *rlmc, const char *path, int renderBufferIndex=0);
    bool RenderLightMap(CRenderer *renderer, CRenderLightMapContext *rlmc, CSceneEntity *e, int lod=0, int renderBufferIndex=0);
    void PackLightMap(const char *srcPath, const char *destPath, bool convertDDS);
    void RemoveLightMap();
    void DeleteLightMapFiles();
    void ResetLightMap();
    CSceneEntityLightMap* GetEntityLightMap(int index);

	// shadow depth map
    CShadowDepthMapInfoSettings& GetShadowDepthMapSettings() { return m_ShadowDepthMapSettings; }
	bool BuildShadowDepthMap(CRenderer *renderer, CRenderDepthMapContext *rdmc);
	void RemoveShadowDepthMap();
	void ResetShadowDepthmap();
	CSceneEntityShadowDepthMap* GetEntityShadowDepthMap(int index);

	// navigation
	void SetVisibleNavMeshIndex(int index) { m_iVisibleNavMeshIndex = index; }
	int GetVisibleNavMeshIndex() { return m_iVisibleNavMeshIndex; }
	CNavigationSettings& GetNavigationSettings(int index);
	bool BuildNavigations();
    void RemoveNavigations();
	CNavMeshAsset* GetNavMesh(int index=0);

	// layer
    CSceneLayerOption& GetLayerOption() { return m_LayerManager; }

	// visible group
	CSceneVisibleGroup& GetVisibleGroup() { return m_VisibleGroup; }

    // cube map
    void RenderCubeMap(const CVector3 &position, int size, bool mirror, CRenderer *renderer, const char *fname, int renderBufferIndex=0);

    // export
    bool Export(const char *fname, bool exportMaterial, bool exportCollision, int terrainLod=0);
    bool Export(const char *fname, SceneEntityList &entities, bool exportInstanceMesh, bool exportCollision, int terrainLod=0);

    // editor flag
    void SetEditorMode(bool mode) { m_bEditorMode = mode; }
    bool GetEditorMode() { return m_bEditorMode; }
    void SetEditorShouldSave(bool save) { m_bEditorShouldSave = save; }
    bool GetEditorShouldSave() { return m_bEditorShouldSave; }

    // editor helper function
    void ComputeVisibleDistance();
	void UpdateEntityVisible();
    void CloneCollisionProperties(CSceneEntity *src);
    void CloneDecalVisibleDistance(CDecalSceneEntity *src);
    void CacheShaders();
    void UpdateDecals();
    void UpdateInstanceMeshes();

    // game object property
    void SetGameObjectPropertyTable(CObjectPropertyTable *table) { m_pObjectPropertyTable = table; }
    CObjectPropertyTable* GetGameObjectPropertyTable() { return m_pObjectPropertyTable; }

private:

    void AddEntity(CSceneEntity *e);
    void ComputeBoundingBox();

    bool Import(const char *fname, SceneEntityList &entities, bool replaceExist);
    bool Load(const char *fname, SceneEntityList &entities, int classIds, bool replaceExist);
    void PostLoad(SceneEntityList &entities, bool activeEnv);
    bool Save(SceneEntityList &entities, CIOStream &stream, bool saveExtra);
    void SaveAssets(SceneEntityList &entities, const char *savePath, const char *oldPath, bool gatherAssets, bool gatherExternalAssets);
    void DeleteEntities();

    void SaveLightMap(CIOStream &stream);
    void SaveEmptyLightMap(CIOStream &stream);
    void LoadLightMap(CIOStream &stream, const CLoadParameter &param, int version);
    void SkipLoadLightMap(CIOStream &stream, int version);

	bool RenderShadowDepthMap(CRenderer *renderer, CRenderDepthMapContext *rlmc, CLightSceneEntity *e, CSceneEntityShadowDepthMap &dm);
    void SaveShadowDepthMap(CIOStream &stream);
    void SaveEmptyShadowDepthMap(CIOStream &stream);
    void LoadShadowDepthMap(CIOStream &stream, const CLoadParameter &param, int version);
    void SkipLoadShadowDepthMap(CIOStream &stream, int version);

    void SaveNavigation(CIOStream &stream);
    void SaveEmptyNavigation(CIOStream &stream);
    void LoadNavigation(CIOStream &stream, const CLoadParameter &param, int version);
    void SkipLoadNavigation(CIOStream &stream, int version);

    void InitializeLayers();
    void SaveLayer(CIOStream &stream);
    void SaveEmptyLayer(CIOStream &stream);
    void LoadLayer(CIOStream &stream, int version);
    void SkipLoadLayer(CIOStream &stream, int version);

    CRenderWorld *m_pRenderWorld;
    CPhysicsWorld *m_pPhysicsWorld;
    CFxPlayer *m_pFxPlayer;

    CAABox m_Box;
    CRandomNumber m_Random;

    SceneEntityList m_Entities;
    SceneEntityList m_DynamicEntities;
    SceneEntityList m_InstanceMeshEntities;
    SceneEntityList m_TerrainEntities;
    int m_iEntityIdCounter;
    CEnvironmentSceneEntity *m_pActiveEnvironment;
    int m_iDrawFlag;
    bool m_bPreloading;
	bool m_bNeedCreateLoadedResource;

    class CLightMapTexture
    {
    public:
        CLightMapTexture() { m_pTexture = 0; m_bGammaCorrection = true; }
        CLightMapTexture(CTextureAsset *t, bool g) { m_pTexture = t; m_bGammaCorrection = g; }
        CTextureAsset* m_pTexture;
        bool m_bGammaCorrection;
    };
    std::vector<CLightMapTexture> m_LightMapTextures;
    std::vector<CSceneEntityLightMap> m_EntityLightMaps;
    CLightMapSettings m_LightMapSettings;

    std::vector<CSceneEntityShadowDepthMap> m_EntityShadowDepthMaps;
	CShadowDepthMapInfoSettings m_ShadowDepthMapSettings;

	CNavMeshAsset *m_pNavMeshes[MAX_NAVIGATION_MESHES];
	CNavigationSettings m_NavigationSettings[MAX_NAVIGATION_MESHES];
	int m_iVisibleNavMeshIndex;

    bool m_bEditorMode;
    bool m_bEditorShouldSave;
    CSceneLayerOption m_LayerManager;
    CSceneVisibleGroup m_VisibleGroup;

    CObjectPropertyTable *m_pObjectPropertyTable;

    CSceneManager *m_pManager;
};


}//GELib

#endif//__GELIB_SCENE_SCENE_H__
