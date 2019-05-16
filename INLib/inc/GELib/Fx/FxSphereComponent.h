//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxSphereComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXSPHERECOMPONENT_H__
#define __GELIB_FX_FXSPHERECOMPONENT_H__

#include "FxComponent.h"

namespace GELib
{


class CFxSphereComponentResource : public CFxComponentResource
{
public:
    CFxSphereComponentResource(CFxAsset *fxRes);
    virtual ~CFxSphereComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);

    virtual int GetClassId() { return FXCLASS_SPHERE; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    friend class CFxSphereComponent;
    float m_fRadius;

private:
};


class CFxSphereComponent : public CFxComponent
{
public:
    CFxSphereComponent(CFx *fx, CFxSphereComponentResource *res);
    virtual ~CFxSphereComponent();

    virtual void GetRandomPoint(CVector3 &p, bool onSurface, bool local, float t);
    virtual void GetClosestPoint(CVector3 &p, const CVector3 &src, bool local, float frac);
    virtual bool IsInside(const CVector3 &p);

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    CFxSphereComponentResource* SphereResource() { return (CFxSphereComponentResource*)m_pResource; }

private:

};




}//GELib

#endif//__GELIB_FX_FXSPHERECOMPONENT_H__
