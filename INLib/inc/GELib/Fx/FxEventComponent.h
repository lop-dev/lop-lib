//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxEventComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXEVENTCOMPONENT_H__
#define __GELIB_FX_FXEVENTCOMPONENT_H__

#include "FxComponent.h"

namespace GELib
{


class CFxEventComponentResource : public CFxComponentResource
{
public:
    CFxEventComponentResource(CFxAsset *fxRes);
    virtual ~CFxEventComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);

    virtual int GetClassId() { return FXCLASS_EVENT; }
    virtual CFxComponent* CreateInstance(CFx *fx);

private:

};


class CFxEventComponent : public CFxComponent
{
public:
    CFxEventComponent(CFx *fx, CFxEventComponentResource *res);
    virtual ~CFxEventComponent();

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    CFxEventComponentResource* EventResource() { return (CFxEventComponentResource*)m_pResource; }

private:

};


}//GELib

#endif//__GELIB_FX_FXEVENTCOMPONENT_H__
