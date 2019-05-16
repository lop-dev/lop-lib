//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxSoundComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXSOUNDCOMPONENT_H__
#define __GELIB_FX_FXSOUNDCOMPONENT_H__

#include "FxComponent.h"

namespace GELib
{

class CAudio;


class CFxSoundComponentResource : public CFxComponentResource
{
public:
    CFxSoundComponentResource(CFxAsset *fxRes);
    virtual ~CFxSoundComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);

    virtual int GetClassId() { return FXCLASS_SOUND; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    std::string m_AudioName;

private:

};


class CFxSoundComponent : public CFxComponent
{
public:
    CFxSoundComponent(CFx *fx, CFxSoundComponentResource *res);
    virtual ~CFxSoundComponent();

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    CFxSoundComponentResource* SoundResource() { return (CFxSoundComponentResource*)m_pResource; }

private:

    CAudio *m_pAudio;
    bool m_bPlayed;
};



}//GELib

#endif//__GELIB_FX_FXSOUNDCOMPONENT_H__
