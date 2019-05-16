//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxBoxComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXBOXCOMPONENT_H__
#define __GELIB_FX_FXBOXCOMPONENT_H__

#include "FxComponent.h"
#include <GELib/Ext/FaceCamera.h>

namespace GELib
{

class CFxBoxComponentResource : public CFxComponentResource
{
public:
    CFxBoxComponentResource(CFxAsset *fxRes);
    virtual ~CFxBoxComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);

    virtual int GetClassId() { return FXCLASS_BOX; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    CVector3 m_Size;
    EFaceCameraMode m_eFaceCameraMode;

private:
};


class CFxBoxComponent : public CFxComponent
{
public:
    CFxBoxComponent(CFx *fx, CFxBoxComponentResource *res);
    virtual ~CFxBoxComponent();

    virtual void GetRandomPoint(CVector3 &p, bool onSurface, bool local, float frac);
    virtual void GetClosestPoint(CVector3 &p, const CVector3 &src, bool local, float frac);
    virtual bool IsInside(const CVector3 &p);

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

   CFxBoxComponentResource* BoxResource() { return (CFxBoxComponentResource*)m_pResource; }

private:

};


void FxSetRandomSeed(unsigned int seed);
float FxGetRandom(float minValue, float maxValue);


}//GELib

#endif//__GELIB_FX_FXBOXCOMPONENT_H__
