//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxFxComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXFXCOMPONENT_H__
#define __GELIB_FX_FXFXCOMPONENT_H__

#include "FxComponent.h"

namespace GELib
{

class CFxAsset;


class CFxFxComponentResource : public CFxComponentResource
{
public:
    CFxFxComponentResource(CFxAsset *fxRes);
    virtual ~CFxFxComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();

    virtual int GetClassId() { return FXCLASS_FX; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    virtual void GetAssets(CAssetHelper &helper);

    bool LoadFx(const char *fname, const CLoadParameter &param);
    void DeleteFx();

	CFxAsset* GetFxFxAsset() { return m_pFxFxAsset; }

    std::string m_AttachableName;
	bool m_bLoop;
	float m_fSpeed;

private:

    CFxAsset *m_pFxFxAsset;
};


class CFxFxComponent : public CFxComponent
{
public:
    CFxFxComponent(CFx *fx, CFxFxComponentResource *res);
    virtual ~CFxFxComponent();

    virtual void SetRelatedComponents();

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    CFxFxComponentResource* FxResource() { return (CFxFxComponentResource*)m_pResource; }

private:
    void UpdateFx();

    CFx *m_pSpawnFx;
};


}//GELib

#endif//__GELIB_FX_FXFXCOMPONENT_H__
