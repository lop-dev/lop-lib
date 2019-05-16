//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Scene/InstanceMeshSceneEntity.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_SCENE_INSTANCEMESHSCENEENTITY_H__
#define __GELIB_SCENE_INSTANCEMESHSCENEENTITY_H__

#include <string>
#include <vector>
#include <GELib/Renderer/RendererTypes.h>
#include <GELib/Ext/InstanceMeshRenderObject.h>
#include <GELib/Utility/VectorMap.h>
#include "SceneEntity.h"


namespace GELib
{

class CIOStream;
class CInstanceMeshData;
class CAssetHelper;
class CCollisionShape;

class CScene;


class CInstanceMeshSceneEntity : public CSceneEntity
{
public:
    CInstanceMeshSceneEntity(CScene *scene);
    virtual ~CInstanceMeshSceneEntity();

    virtual int GetClassId() { return SCENEENTITY_INSTANCE_MESH; }

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

    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

	virtual void UpdateVisible();

    bool LoadMesh(const char *fname, const CLoadParameter &param);
	void SetMesh(CMeshAsset *mesh);
    CMeshAsset* GetMesh();
    bool LoadCollisionShape(const char *fname, const CLoadParameter &param);
	void SetCollisionShape(CCollisionShape *shape);
	CCollisionShape* GetCollisionShape();

    int GetNumRenderObjects() const;
    CInstanceMeshRenderObject* GetRenderObject(int index);
    void UpdateRenderObjects();

    int GetNumPhysicsObjects() const;
	CPhysicsObject* GetPhysicsObject(int index);
	CSceneCollisionProperty* GetCollisionProperty();
    void ApplyCollisionProperties();

    void SetCastDynamicShadow(bool b);
    bool GetCastDynamicShadow() const { return m_bCastDynamicShadow; }
    void SetCastStaticShadow(bool b);
    bool GetCastStaticShadow() const { return m_bCastStaticShadow; }
    void SetShadowResolutionFactor(float r);
    float GetShadowResolutionFactor() const { return m_fShadowResolutionFactor; }
    void SetShadowDepthBias(float b);
    float GetShadowDepthBias() const { return m_fShadowDepthBias; }
    void SetMaxVisibleDistance(float d);
    float GetMaxVisibleDistance() const { return m_fMaxVisibleDistance; }
    void SetFadeDistance(float d);
    float GetFadeDistance() const { return m_fFadeDistance; }
    void SetLightGroups(int g);
    int  GetLightGroups() const { return m_iLightGroups; }

	void SetClusterSize(float s);
	float GetClusterSize() const { return m_fClusterSize; }

    void SetWindParameter(const CInstanceMeshWindParameter &w);
    const CInstanceMeshWindParameter& GetWindParameter() const { return m_WindParameter; }

	void SetCollider(int index, const CVector3 &position, float radius);

    void SetRotationVariation(float vmin, float vmax) { m_RotationVariation[0] = vmin; m_RotationVariation[1] = vmax; }
    void GetRotationVariation(float &vmin, float &vmax) const { vmin = m_RotationVariation[0]; vmax = m_RotationVariation[1]; }
    void SetScaleVariation(float vmin, float vmax) { m_ScaleVariation[0] = vmin; m_ScaleVariation[1] = vmax; }
    void GetScaleVariation(float &vmin, float &vmax) const { vmin = m_ScaleVariation[0]; vmax = m_ScaleVariation[1]; }
    void SetNormalVariation(float vmin, float vmax) { m_NormalVariation[0] = vmin; m_NormalVariation[1] = vmax; }
    void GetNormalVariation(float &vmin, float &vmax) const { vmin = m_NormalVariation[0]; vmax = m_NormalVariation[1]; }

    void AddInstances(const CMatrix *m, int num);
    int RemoveInstances(const CSphere &sphere, float probability=1.0f);
	int GetInstances(const CSphere &sphere, std::vector<CMatrix> &instances);
	bool PickInstance(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0, CMatrix *m=0);
    void TransformInstances(const CMatrix &m);

protected:

    void DeleteMesh();
    void DeleteRenderObjects();
    CInstanceMeshRenderObject* CreateRenderObject(unsigned int hash, int lightMapIndex);
    int GetRenderObjectIndex(unsigned int hash);

	void CreatePhysicsObject(CInstanceMeshRenderObject *robj, const CInstanceMeshData &in, bool addToWorld);
	void DeletePhysicsObject(CInstanceMeshRenderObject *robj, const CInstanceMeshData &in);
    void DeletePhysicsObjects();
    void ApplyCollisionProperties(CPhysicsObject *pobj);

    CMeshAsset *m_pMesh;
    int m_iRenderLightMapMode;
    float m_fShadowResolutionFactor;
    float m_fShadowDepthBias;
    float m_fMaxVisibleDistance;
    float m_fFadeDistance;
	float m_fClusterSize;
    int m_iLightGroups;
    bool m_bCastDynamicShadow;
    bool m_bCastStaticShadow;
    CInstanceMeshWindParameter m_WindParameter;
    float m_RotationVariation[2];
    float m_ScaleVariation[2];
    float m_NormalVariation[2];

    class CInstanceGroup
    {
    public:
	    int m_iLightMapIndex;
        CInstanceMeshRenderObject *m_pRenderObject;
    };
	typedef VectorMap<unsigned int, CInstanceGroup> CInstanceGroupList;
    CInstanceGroupList m_InstanceGroups;
	std::vector<CPhysicsObject*> m_PhysicsObjects;
	CCollisionShape *m_pCollisionShape;
    CSceneCollisionProperty m_CollisionProperty;
};



}//GELib

#endif//__GELIB_SCENE_INSTANCEMESHSCENEENTITY_H__
