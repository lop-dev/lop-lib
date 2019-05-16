//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxAnimationComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXANIMATIONCOMPONENT_H__
#define __GELIB_FX_FXANIMATIONCOMPONENT_H__

#include "FxComponent.h"
#include <GELib/Ext/MeshAsset.h>

namespace GELib
{

class CAnimationAsset;
class CCharacter;


class CFxAnimationComponentResource : public CFxComponentResource
{
public:
    CFxAnimationComponentResource(CFxAsset *fxRes);
    virtual ~CFxAnimationComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();

    virtual int GetClassId() { return FXCLASS_ANIMATION; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    virtual void GetAssets(CAssetHelper &helper);

    bool LoadAnimation(const char *fname, const CLoadParameter &param);
    void SetAnimation(CAnimationAsset *anim);
    CAnimationAsset* GetAnimation();
    void DeleteAnimation();

private:

    CAnimationAsset *m_pAnimation;
};


class CFxAnimationComponent : public CFxComponent
{
public:
    CFxAnimationComponent(CFx *fx, CFxAnimationComponentResource *res);
    virtual ~CFxAnimationComponent();

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    CFxAnimationComponentResource* AnimationResource() { return (CFxAnimationComponentResource*)m_pResource; }

	virtual IAttachable* GetAttachable();

private:

    CCharacter *m_pCharacter;
};


}//GELib

#endif//__GELIB_FX_FXANIMATIONCOMPONENT_H__
