//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxDecalComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXDECALCOMPONENT_H__
#define __GELIB_FX_FXDECALCOMPONENT_H__

#include "FxComponent.h"
#include <GELib/Ext/MaterialAsset.h>

namespace GELib
{

class CMaterialAsset;
class CFxDecalRenderObject;


class CFxDecalRenderObject;

class CFxDecalComponentResource : public CFxComponentResource
{
public:
    CFxDecalComponentResource(CFxAsset *fxRes);
    virtual ~CFxDecalComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();

    virtual int GetClassId() { return FXCLASS_DECAL; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    virtual void GetAssets(CAssetHelper &helper);

    bool LoadMaterial(const char *fname, const CLoadParameter &param);
    void SetMaterial(CMaterialAsset *material);
    CMaterialAsset* GetMaterial();
    void DeleteMaterial();

    CVector3 m_Size;
	bool m_bUseEnvironmentLight;
    int m_iLightGroups;
	char m_iTextureArrayType;
	char m_iTextureArrayFps;
	char m_iTextureArrayLoopMode;

private:

    CMaterialAsset *m_pMaterial;
};


class CFxDecalComponent : public CFxComponent
{
public:
    CFxDecalComponent(CFx *fx, CFxDecalComponentResource *res);
    virtual ~CFxDecalComponent();

    virtual void GetRandomPoint(CVector3 &p, bool onSurface, bool local, float frac);
    virtual void GetClosestPoint(CVector3 &p, const CVector3 &src, bool local, float frac);

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    CFxDecalComponentResource* DecalResource() { return (CFxDecalComponentResource*)m_pResource; }

private:

    void UpdateRenderObject();
	CFxDecalRenderObject *m_pRenderObject;
};



}//GELib

#endif//__GELIB_FX_FXDECALCOMPONENT_H__
