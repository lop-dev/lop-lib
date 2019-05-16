//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxLightComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXLIGHTCOMPONENT_H__
#define __GELIB_FX_FXLIGHTCOMPONENT_H__

#include "FxComponent.h"
#include "FxRenderLight.h"

namespace GELib
{

class CMaterialAsset;
class CBaseRenderLight;


class CFxLightComponentResource : public CFxComponentResource
{
public:
    CFxLightComponentResource(CFxAsset *fxRes);
    virtual ~CFxLightComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();

    virtual int GetClassId() { return FXCLASS_LIGHT; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    virtual void GetAssets(CAssetHelper &helper);

    bool LoadMaterial(const char *fname, const CLoadParameter &param);
    void SetMaterial(CMaterialAsset *material);
    CMaterialAsset* GetMaterial();
    void DeleteMaterial();

    bool m_bCastDynamicShadow;
    float m_fRange;
    float m_fInnerCone;
    float m_fOuterCone;
    float m_fShadowDepthBias;
    short m_iProjectorLayer;
    CRenderLight::ELightType m_eType;
    CRenderLight::EProjectorBlendMode m_eProjectorBlendMode;
    CVector2 m_Size;
    int m_iLightGroups;

private:

    CMaterialAsset *m_pMaterial;
};


class CFxLightComponent : public CFxComponent
{
public:
    CFxLightComponent(CFx *fx, CFxLightComponentResource *res);
    virtual ~CFxLightComponent();

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    CFxLightComponentResource* LightResource() { return (CFxLightComponentResource*)m_pResource; }

private:

    void UpdateRenderObject();
    CFxRenderLight *m_pRenderObject;
};



}//GELib

#endif//__GELIB_FX_FXLIGHTCOMPONENT_H__
