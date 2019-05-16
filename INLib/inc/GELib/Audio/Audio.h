//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Audio/Audio.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_AUDIO_AUDIO_H__
#define __GELIB_AUDIO_AUDIO_H__

#include <GELib/Math/Vector.h>
#include <string>

namespace GELib
{

class CAudioDevice;

class CAudio
{
public:
    CAudio() {}
    virtual ~CAudio() {}

    virtual CAudioDevice* GetDevice() = 0;

    virtual void SetName(const char *name) = 0;
    virtual void GetName(std::string &name) = 0;

    virtual void SetPosition(const GELib::CVector3 &p) = 0;
    virtual void GetPosition(GELib::CVector3 &p) = 0;

    virtual void SetVelocity(const GELib::CVector3 &v) = 0;
    virtual void GetVelocity(GELib::CVector3 &v) = 0;

    virtual void SetRange(float r) = 0;
    virtual float GetRange() = 0;

    virtual void SetVariable(const char *name, float value) = 0;

    virtual bool Is3D() = 0;

    virtual void SetLoop(bool loop) = 0;
    virtual bool GetLoop() = 0;

    virtual void Play() = 0;
    virtual void Stop() = 0;
    virtual void Pause(bool pause=true) = 0;

    enum EAudioState
    {
        AUDIO_STATE_STOPPED,
        AUDIO_STATE_PLAYING,
        AUDIO_STATE_PAUSED,
    };

    virtual EAudioState GetState() = 0;
};


}//GELib

#endif//__GELIB_AUDIO_AUDIO_H__
