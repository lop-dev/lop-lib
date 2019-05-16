//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxSound2dComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXSOUND2DCOMPONENT_H__
#define __GELIB_FX_FXSOUND2DCOMPONENT_H__

#include "FxComponent.h"

namespace GELib
{

class CAudio;


class CFxSound2dComponentResource : public CFxComponentResource
{
public:
    CFxSound2dComponentResource(CFxAsset *fxRes);
    virtual ~CFxSound2dComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);

    virtual int GetClassId() { return FXCLASS_SOUND2D; }
    virtual CFxComponent* CreateInstance(CFx *fx);

private:

};


class CFxSound2dComponent : public CFxComponent
{
public:
    CFxSound2dComponent(CFx *fx, CFxSound2dComponentResource *res);
    virtual ~CFxSound2dComponent();

    virtual void SetTime(float t);

	virtual void DrawSelected(IPlotter3D &plotter, IPlotter2D &canvas);

    CFxSound2dComponentResource* Sound2dResource() { return (CFxSound2dComponentResource*)m_pResource; }

private:

    CAudio *m_pAudio;
};



}//GELib

#endif//__GELIB_FX_FXSOUND2DCOMPONENT_H__
