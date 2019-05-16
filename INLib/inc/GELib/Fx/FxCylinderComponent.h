//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxCylinderComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXCYLINDERCOMPONENT_H__
#define __GELIB_FX_FXCYLINDERCOMPONENT_H__

#include "FxComponent.h"

namespace GELib
{

class CFxCylinderComponentResource : public CFxComponentResource
{
public:
    CFxCylinderComponentResource(CFxAsset *fxRes);
    virtual ~CFxCylinderComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);

    virtual int GetClassId() { return FXCLASS_CYLINDER; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    float m_fRadius;
    float m_fHeight;

private:
};


class CFxCylinderComponent : public CFxComponent
{
public:
    CFxCylinderComponent(CFx *fx, CFxCylinderComponentResource *res);
    virtual ~CFxCylinderComponent();

    virtual void GetRandomPoint(CVector3 &p, bool onSurface, bool local, float frac);
    virtual void GetClosestPoint(CVector3 &p, const CVector3 &src, bool local, float frac);
    virtual bool IsInside(const CVector3 &p);

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    CFxCylinderComponentResource* CylinderResource() { return (CFxCylinderComponentResource*)m_pResource; }

private:

};



}//GELib

#endif//__GELIB_FX_FXCYLINDERCOMPONENT_H__
