//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Scene/SceneEntity.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_SCENE_SCENEENTITY_H__
#define __GELIB_SCENE_SCENEENTITY_H__

#include <string>
#include <GELib/Utility/ColladaFile.h>
#include <GELib/Math/ConvexVolume.h>
#include <GELib/Renderer/RendererTypes.h>


namespace GELib
{

class CIOStream;
class CColladaFile;
class CLightMapInfo;
class IPlotter3D;
class IPlotter2D;
class CPhysicsObject;
class CCollisionShape;
class CFxAsset;
class CFx;
class CAudio;
class CMaterialAsset;
class CBaseRenderLight;
class CMeshAsset;
class CMeshRenderObject;
class CDecalRenderObject;
class CTreeRenderObject;
class CAssetHelper;
class CLoadParameter;
class CCharacter;


class CScene;
class CGroupSceneEntity;
class CEntityLightMap;


// scene entity class id
const int SCENEENTITY_BASE = 1 << 0;
const int SCENEENTITY_MESH = 1 << 1;
const int SCENEENTITY_CHARACTER = 1 << 2;
const int SCENEENTITY_DECAL = 1 << 3;
const int SCENEENTITY_TREE = 1 << 4;
const int SCENEENTITY_INSTANCE_MESH = 1 << 5;
const int SCENEENTITY_TERRAIN = 1 << 6;
// modified by bob -- start
const int SCENEENTITY_WATER = 1 << 7;
// modified by bob -- end
const int SCENEENTITY_LIGHT = 1 << 10;
const int SCENEENTITY_FX = 1 << 13;
const int SCENEENTITY_SOUND = 1 << 14;
const int SCENEENTITY_ENVIRONMENT = 1 << 15;
const int SCENEENTITY_OCCLUDER = 1 << 21;
const int SCENEENTITY_PORTAL = 1 << 22;
const int SCENEENTITY_ZONE = 1 << 23;
const int SCENEENTITY_LOCATOR = 1 << 25;
//const int SCENEENTITY_TRIGGER = 1 << 26;
const int SCENEENTITY_CAMERA = 1 << 27;
const int SCENEENTITY_GAME_OBJECT = 1 << 28;
const int SCENEENTITY_GROUP = 1 << 30;
const int SCENEENTITY_ALL = -1;

class CSceneCollisionProperty
{
public:
    CSceneCollisionProperty();
    int m_iCollisionMask;
    short m_iMaterial;
};

class CSceneEntity
{
public:
    CSceneEntity(CScene *scene);
    virtual ~CSceneEntity();

    virtual int GetClassId() { return SCENEENTITY_BASE; }

    virtual bool Save(CIOStream &stream);
	virtual bool Load(CIOStream &stream, const CLoadParameter &param);
    virtual bool CreateLoadedResource();
    virtual void PostLoad();

    virtual void GetAssets(CAssetHelper &helper);

	virtual int GetNumLightMapLods();
    virtual void SetLightMapIndex(int index, int lod);
    virtual int GetLightMapIndex(int lod) const;

    // render lightmap mode (-1: default, 0: none, 1: mono shadow, 2: color shadow, 3: simple light, 4: full light)
    virtual void SetRenderLightMapMode(int mode);
    virtual int GetRenderLightMapMode() const;

    virtual void SetMatrix(const CMatrix &matrix);
    virtual void GetMatrix(CMatrix &matrix);
    virtual void GetBoundingBox(CAABox &box);

    virtual bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
    virtual bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);
    virtual bool IntersectConvexVolume(const CConvexVolume &c);

    virtual void Update(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    virtual void Show(bool show);
	bool IsShow() const { return m_bShow; }

    virtual void SetLayer(int layer);
    int GetLayer() const { return m_iLayer; }

    virtual void SetVisibleGroup(int g);
	int GetVisibleGroup() const { return m_iVisibleGroup; }

	virtual void SetLodGroup(int g);
	int GetLodGroup() const { return m_iLodGroup; }

	bool IsVisible();
	virtual void UpdateVisible();
	virtual void ResetTime();

    void SetId(int id) { m_iId = id; }
    int  GetId() const { return m_iId; }

    // used by CGroupSceneEntity
    // should use CGroupSceneEntity to change entity group instead of calling SetGroup() directly
    void SetGroup(CGroupSceneEntity *g);
    CGroupSceneEntity* GetGroup();

    CScene* GetScene() const { return m_pScene; }

    void SetName(const std::string &name) { m_Name = name; }
    void SetName(const char *name) { m_Name = name; }
    const std::string& GetName() const { return m_Name; }

protected:

    std::string m_Name;
    int m_iId;

    int m_iGroupEntityId;
	int m_iVisibleGroup;
    short m_iLayer;
	char m_iLodGroup;
    bool m_bShow;
    CScene *m_pScene;
};


class CMeshSceneEntity : public CSceneEntity
{
public:
    CMeshSceneEntity(CScene *scene);
    virtual ~CMeshSceneEntity();

    virtual int GetClassId() { return SCENEENTITY_MESH; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
    virtual bool CreateLoadedResource();
    virtual void PostLoad();

    virtual void GetAssets(CAssetHelper &helper);

	virtual int GetNumLightMapLods();
    virtual void SetLightMapIndex(int index, int lod);
    virtual int GetLightMapIndex(int lod) const;
    virtual void SetRenderLightMapMode(int mode) { m_iRenderLightMapMode = mode; }
    virtual int GetRenderLightMapMode() const { return m_iRenderLightMapMode; }

    virtual void SetMatrix(const CMatrix &matrix);
    virtual void GetMatrix(CMatrix &matrix);
    virtual void GetBoundingBox(CAABox &box);

    virtual bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
    virtual bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);
    virtual bool IntersectConvexVolume(const CConvexVolume &c);

    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

	virtual void UpdateVisible();

    bool LoadMesh(const char *fname, const CLoadParameter &param);
	void SetMesh(CMeshAsset *mesh);
    CMeshAsset* GetMesh();
    bool LoadCollisionShape(const char *fname, const CLoadParameter &param);
    void SetCollisionShape(CCollisionShape *shape);
	CCollisionShape* GetCollisionShape();

    CMeshRenderObject* GetRenderObject() { return m_pRenderObject; }
    CPhysicsObject* GetPhysicsObject() { return m_pPhysicsObject; }

    void SetAcceptDecal(bool a) { m_bAcceptDecal = a; }
    bool GetAcceptDecal() const { return m_bAcceptDecal; }

    int GetNumCollisionProperties();
    CSceneCollisionProperty* GetCollisionProperty(int index);
    void ApplyCollisionProperties();

private:
	void CreateRenderObject();
    void UpdateRenderObject();
    void DeleteRenderObject();
	void CreatePhysicsObject();
    void DeletePhysicsObject();
    CMatrix m_Matrix;
    CMeshRenderObject *m_pRenderObject;
    CPhysicsObject *m_pPhysicsObject;
    int m_LightMapIndex[3];		// LIGHTMAP_LOD_MAX
    int m_iRenderLightMapMode;
    bool m_bAcceptDecal;
    std::vector<CSceneCollisionProperty> m_CollisionProperties;
};


class CCharacterSceneEntity : public CSceneEntity
{
public:
    CCharacterSceneEntity(CScene *scene);
    virtual ~CCharacterSceneEntity();

    virtual int GetClassId() { return SCENEENTITY_CHARACTER; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
    virtual bool CreateLoadedResource();
    virtual void PostLoad();

    virtual void GetAssets(CAssetHelper &helper);

	virtual int GetNumLightMapLods();
    virtual void SetLightMapIndex(int index, int lod);
    virtual int GetLightMapIndex(int lod) const;
    virtual void SetRenderLightMapMode(int mode) { m_iRenderLightMapMode = mode; }
    virtual int GetRenderLightMapMode() const { return m_iRenderLightMapMode; }

    virtual void SetMatrix(const CMatrix &matrix);
    virtual void GetMatrix(CMatrix &matrix);
    virtual void GetBoundingBox(CAABox &box);

    virtual bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
    virtual bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);
    virtual bool IntersectConvexVolume(const CConvexVolume &c);

    virtual void Update(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

	virtual void UpdateVisible();
	virtual void ResetTime();

    bool LoadCharacter(const char *fname, const CLoadParameter &param);
    CCharacter* GetCharacter();

    void SetAnimation(const char *name);
    const std::string& GetAnimation() const { return m_AnimationName; }
    void PlayAnimation();
	void EnableHandleAnimationEvent(bool enable) { m_bHandleAnimationEvent = enable; }
	bool IsEnableHandleAnimationEvent() { return m_bHandleAnimationEvent; }

    void SetStartTime(float minTime, float maxTime);
    void GetStartTime(float &minTime, float &maxTime) const;

private:

    void DeleteCharacter();

    float m_fMinStartTime;
    float m_fMaxStartTime;
    CMatrix m_Matrix;

    std::string m_AnimationName;
    CCharacter *m_pCharacter;
    int m_LightMapIndex[3];		// LIGHTMAP_LOD_MAX
    int m_iRenderLightMapMode;
	bool m_bHandleAnimationEvent;
};

class CDecalSceneEntity : public CSceneEntity
{
public:
    CDecalSceneEntity(CScene *scene);
    virtual ~CDecalSceneEntity();

    virtual int GetClassId() { return SCENEENTITY_DECAL; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();
    virtual void PostLoad();

    virtual void GetAssets(CAssetHelper &helper);

	virtual int GetNumLightMapLods();
    virtual void SetLightMapIndex(int index, int lod);
    virtual int GetLightMapIndex(int lod) const;
    virtual void SetRenderLightMapMode(int mode) { m_iRenderLightMapMode = mode; }
    virtual int GetRenderLightMapMode() const { return m_iRenderLightMapMode; }

    virtual void SetMatrix(const CMatrix &matrix);
    virtual void GetMatrix(CMatrix &matrix);
    virtual void GetBoundingBox(CAABox &box);

    virtual bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
    virtual bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);
    virtual bool IntersectConvexVolume(const CConvexVolume &c);

    void CreateRenderObject();
    CDecalRenderObject* GetRenderObject() { return m_pRenderObject; }
    void UpdateRenderObject();

    bool LoadMaterial(const char *fname, const CLoadParameter &param);
    CMaterialAsset* GetMaterial() { return m_pMaterial; }
    void DeleteMaterial();

    void SetTextureCoordRepeat(const CVector2 &r) { m_TextureCoordRepeat = r; }
    const CVector2& GetTextureCoordRepeat() const { return m_TextureCoordRepeat; }

    void SetMaxVisibleDistanceRatio(float d);
    float GetMaxVisibleDistanceRatio() const { return m_fMaxVisibleDistanceRatio; }

    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

	virtual void UpdateVisible();

private:
    void DeleteRenderObject();
    CMatrix m_Matrix;
    CVector2 m_TextureCoordRepeat;
    CDecalRenderObject *m_pRenderObject;
    CMaterialAsset *m_pMaterial;
    float m_fMaxVisibleDistanceRatio;
    int m_iLightMapIndex;
    int m_iRenderLightMapMode;
    bool m_bMeshDirty;
};

class CTreeSceneEntity : public CSceneEntity
{
public:
    CTreeSceneEntity(CScene *scene);
    virtual ~CTreeSceneEntity();

    virtual int GetClassId() { return SCENEENTITY_TREE; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();
    virtual void PostLoad();

    virtual void GetAssets(CAssetHelper &helper);

	virtual int GetNumLightMapLods();
    virtual void SetLightMapIndex(int index, int lod);
    virtual int GetLightMapIndex(int lod) const;
    virtual void SetRenderLightMapMode(int mode) { m_iRenderLightMapMode = mode; }
    virtual int GetRenderLightMapMode() const { return m_iRenderLightMapMode; }

    virtual void SetMatrix(const CMatrix &matrix);
    virtual void GetMatrix(CMatrix &matrix);
    virtual void GetBoundingBox(CAABox &box);

    virtual bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
    virtual bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);
    virtual bool IntersectConvexVolume(const CConvexVolume &c);

    virtual void Update(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

	virtual void UpdateVisible();
	virtual void ResetTime();

    bool LoadMesh(const char *fname, const CLoadParameter &param);
	void SetMesh(CMeshAsset *mesh);
    CMeshAsset* GetMesh();
    bool LoadCollisionShape(const char *fname, const CLoadParameter &param);
    void SetCollisionShape(CCollisionShape *shape);
	CCollisionShape* GetCollisionShape();

    CTreeRenderObject* GetRenderObject() const { return m_pRenderObject; }
    CPhysicsObject* GetPhysicsObject() const { return m_pPhysicsObject; }

    int GetNumCollisionProperties();
    CSceneCollisionProperty* GetCollisionProperty(int index);
    void ApplyCollisionProperties();

private:
	void CreateRenderObject();
    void UpdateRenderObject();
    void DeleteRenderObject();
	void CreatePhysicsObject();
    void DeletePhysicsObject();

    CMatrix m_Matrix;
    CTreeRenderObject *m_pRenderObject;
    CPhysicsObject *m_pPhysicsObject;
    int m_LightMapIndex[3];		// LIGHTMAP_LOD_MAX
    int m_iRenderLightMapMode;
    std::vector<CSceneCollisionProperty> m_CollisionProperties;

    float m_fUpdateWindTimer;
    CVector3 m_WindForce;
};

class CLightSceneEntity : public CSceneEntity
{
public:
    CLightSceneEntity(CScene *scene);
    virtual ~CLightSceneEntity();

    virtual int GetClassId() { return SCENEENTITY_LIGHT; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();
    virtual void PostLoad();

    virtual void GetAssets(CAssetHelper &helper);

    virtual void SetMatrix(const CMatrix &matrix);
    virtual void GetMatrix(CMatrix &matrix);
    virtual void GetBoundingBox(CAABox &box);

    virtual bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
    virtual bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);
    virtual bool IntersectConvexVolume(const CConvexVolume &c);

    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

	virtual void UpdateVisible();

    void Import(CColladaFile::Light *colladaLight);
    void GetColladaProperty(CColladaFile::Light *colladaLight);

    void CreateRenderObject();
    CBaseRenderLight* GetRenderObject() const { return m_pRenderObject; }

    bool LoadMaterial(const char *fname, const CLoadParameter &param);
    void SetMaterial(CMaterialAsset *material);
    CMaterialAsset* GetMaterial() const { return m_pMaterial; }
    void DeleteMaterial();

	void EnableShadowDepthMap(bool enable);
	bool IsEnableShadowDepthMap();

    void SetShadowDepthMapIndex(int index);
    int GetShadowDepthMapIndex() const;

private:
    void UpdateRenderObject();
    void DeleteRenderObject();

    CMatrix m_Matrix;
    CBaseRenderLight *m_pRenderObject;
    CMaterialAsset *m_pMaterial;
	bool m_bEnableShadowDepthMap;
    int m_iShadowDepthMapIndex;
};


class CFxSceneEntity : public CSceneEntity
{
public:
    CFxSceneEntity(CScene *scene);
    virtual ~CFxSceneEntity();

    virtual int GetClassId() { return SCENEENTITY_FX; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();
    virtual void PostLoad();

    virtual void GetAssets(CAssetHelper &helper);

    virtual void SetMatrix(const CMatrix &matrix);
    virtual void GetMatrix(CMatrix &matrix);
    virtual void GetBoundingBox(CAABox &box);

    virtual bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
    virtual bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);
    virtual bool IntersectConvexVolume(const CConvexVolume &c);

    virtual void Update(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

	virtual void UpdateVisible();

    bool LoadFx(const char *fname, const CLoadParameter &param);
	CFxAsset* GetFxAsset() { return m_pFxAsset; }
	CFx* GetFx() { return m_pFx; }

    void SetLoopDelay(float minDelay, float maxDelay);
    void GetLoopDelay(float &minDelay, float &maxDelay) const;
private:

    void DeleteFx();

    float m_fMinLoopDelay;
    float m_fMaxLoopDelay;
    float m_fLoopDelay;
    CMatrix m_Matrix;
    CFxAsset *m_pFxAsset;
    CFx *m_pFx;
};


class CSoundSceneEntity : public CSceneEntity
{
public:
    CSoundSceneEntity(CScene *scene);
    virtual ~CSoundSceneEntity();

    virtual int GetClassId() { return SCENEENTITY_SOUND; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
    virtual void PostLoad();

    virtual void SetMatrix(const CMatrix &matrix);
    virtual void GetMatrix(CMatrix &matrix);
    virtual void GetBoundingBox(CAABox &box);

    virtual bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
    virtual bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);
    virtual bool IntersectConvexVolume(const CConvexVolume &c);

    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    void SetAudioName(const char *name);
    const char * GetAudioName() const;
    void Set3D(bool d);
    bool Is3D() const { return m_b3D; }
    void PlayAudio();
    void StopAudio();

private:

	void DeleteAudio();

    bool m_b3D;
    CMatrix m_Matrix;
    std::string m_AudioName;
    CAudio *m_pAudio;
};




}//GELib

#endif//__GELIB_SCENE_SCENEENTITY_H__
