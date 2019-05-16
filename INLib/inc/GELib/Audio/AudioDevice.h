//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Audio/AudioDevice.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_AUDIO_AUDIODEVICE_H__
#define __GELIB_AUDIO_AUDIODEVICE_H__

#include <GELib/Math/Vector.h>
#include "Audio.h"
#include <string>
#include <vector>

namespace GELib
{

class CAudioDefaultProperty
{
public:
    std::string m_Name;
    float m_fRange;
};

class CAudioDeviceSettings
{
public:
    std::string m_GlobalSettingsFileName;
    std::vector<CAudioDefaultProperty> m_DefaultProperties;
};


class CAudioDevice
{
public:
    CAudioDevice() {}
    virtual ~CAudioDevice() {}

    virtual bool Initialize(const CAudioDeviceSettings &settings) = 0;
    virtual void Uninitialize() = 0;

    // streamPacketSize = 0: no streaming
    virtual bool LoadWave(const char *name, int streamingPacketSize=0, int index=0) = 0;
    virtual bool LoadSound(const char *name, int index=0) = 0;

    virtual CAudio* CreateAudio() = 0;
    virtual void DeleteAudio(CAudio *a) = 0;

    virtual void SetListenerPosition(const GELib::CVector3 &p) = 0;
    virtual void SetListenerDirection(const GELib::CVector3 &front, const GELib::CVector3 &up) = 0;
    virtual void SetListenerVelocity(const GELib::CVector3 &v) = 0;

    virtual void SetVolume(const char *category, float volume) = 0;

    virtual void SetVariable(const char *name, float value) = 0;

    virtual void Update(float t) = 0;

    virtual void GetAudios(std::vector<CAudio*> &audios) = 0;
    virtual void GetAudioNames(std::vector<std::string> &audioNames) = 0;

    enum EInternalDevice
    {
        INTERNAL_DEVICE_XACT_ENGINE,
        INTERNAL_DEVICE_XAUDIO
    };

    virtual void* GetInternalDevice(EInternalDevice device) = 0;

};

enum EAudioDeviceType
{
    AUDIO_DEVICE_NULL,
    AUDIO_DEVICE_XACT,
    AUDIO_DEVICE_FMOD
};

CAudioDevice* CreateAudioDevice(EAudioDeviceType deviceType);
void DeleteAudioDevice(CAudioDevice *device);
void SetAudioDevice(CAudioDevice *device);
CAudioDevice* GetAudioDevice();


}//GELib

#endif//__GELIB_AUDIO_AUDIODEVICE_H__
