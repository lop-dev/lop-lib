//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxMeshComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXMESHCOMPONENT_H__
#define __GELIB_FX_FXMESHCOMPONENT_H__

#include "FxComponent.h"
#include <GELib/Ext/MeshAsset.h>

namespace GELib
{

class CMaterialAsset;


class CFxMeshRenderObject;

class CFxMeshComponentResource : public CFxComponentResource
{
public:
    CFxMeshComponentResource(CFxAsset *fxRes);
    virtual ~CFxMeshComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();

    virtual int GetClassId() { return FXCLASS_MESH; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    virtual void GetAssets(CAssetHelper &helper);

    bool LoadMesh(const char *fname, const CLoadParameter &param);
    void SetMesh(CMeshAsset *mesh);
    CMeshAsset* GetMesh();
    void DeleteMesh();

    bool LoadMaterial(const char *fname, const CLoadParameter &param);
    void SetMaterial(CMaterialAsset *material);
    CMaterialAsset* GetMaterial();
    void DeleteMaterial();

    EFaceCameraMode m_eFaceCameraMode;

    bool m_bCastDynamicShadow;
    bool m_bMotionBlur;
	bool m_bUseEnvironmentLight;
	EDrawMeshMode m_eDrawMeshMode;
    float m_fShadowResolutionFactor;
    int m_iLightGroups;
	char m_iTextureArrayType;
	char m_iTextureArrayFps;
	char m_iTextureArrayLoopMode;

private:

    CMeshAsset *m_pMesh;
    CMaterialAsset *m_pMaterial;
};


class CFxMeshComponent : public CFxComponent
{
public:
    CFxMeshComponent(CFx *fx, CFxMeshComponentResource *res);
    virtual ~CFxMeshComponent();

    virtual void GetRandomPoint(CVector3 &p, bool onSurface, bool local, float frac);
    virtual void GetClosestPoint(CVector3 &p, const CVector3 &src, bool local, float frac);

    virtual void SetTime(float t);

    CFxMeshComponentResource* MeshResource() { return (CFxMeshComponentResource*)m_pResource; }

private:

    void UpdateRenderObject();
    CFxMeshRenderObject *m_pRenderObject;
};


}//GELib

#endif//__GELIB_FX_FXMESHCOMPONENT_H__
