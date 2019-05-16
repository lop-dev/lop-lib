//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxBoardComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXBOARDCOMPONENT_H__
#define __GELIB_FX_FXBOARDCOMPONENT_H__

#include "FxComponent.h"
#include <GELib/Ext/MeshAsset.h>

namespace GELib
{

class CMaterialAsset;
class CFxBoardRenderObject;
class CFxBoardMesh;

class CFxBoardComponentResource : public CFxComponentResource
{
public:
    CFxBoardComponentResource(CFxAsset *fxRes);
    virtual ~CFxBoardComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();

    virtual int GetClassId() { return FXCLASS_BOARD; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    virtual void GetAssets(CAssetHelper &helper);

    bool LoadMaterial(const char *fname, const CLoadParameter &param);
    void SetMaterial(CMaterialAsset *material);
    CMaterialAsset* GetMaterial();
    void DeleteMaterial();

    CVector2 m_Size;  // half extent
    EFaceCameraMode m_eFaceCameraMode;
	bool m_bUseEnvironmentLight;
	char m_iTextureArrayType;
	char m_iTextureArrayFps;
	char m_iTextureArrayLoopMode;
    int m_iLightGroups;

private:
    CMaterialAsset *m_pMaterial;
};


class CFxBoardComponent : public CFxComponent
{
public:
    CFxBoardComponent(CFx *fx, CFxBoardComponentResource *res);
    virtual ~CFxBoardComponent();

    virtual void GetRandomPoint(CVector3 &p, bool onSurface, bool local, float frac);
    virtual void GetClosestPoint(CVector3 &p, const CVector3 &src, bool local, float frac);

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    void UpdateMesh(CFxBoardMesh *mesh);

    CFxBoardComponentResource* BoardResource() { return (CFxBoardComponentResource*)m_pResource; }

private:

    void UpdateRenderObject();
    CFxBoardRenderObject *m_pRenderObject;
	float m_fSpawnRandom;
};


}//GELib

#endif//__GELIB_FX_FXBOARDCOMPONENT_H__
