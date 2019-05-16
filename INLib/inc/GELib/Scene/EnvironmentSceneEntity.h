//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Scene/EnvironmentSceneEntity.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_SCENE_ENVIRONMENTSCENEENTITY_H__
#define __GELIB_SCENE_ENVIRONMENTSCENEENTITY_H__

#include <GELib/Math/GeMath.h>
#include <GELib/Renderer/Renderer.h>
#include "SceneEntity.h"

namespace GELib
{

class CIOStream;


class CEnvironmentSceneEntity : public CSceneEntity
{
public:
    CEnvironmentSceneEntity(CScene *scene);
    virtual ~CEnvironmentSceneEntity();

    virtual int GetClassId() { return SCENEENTITY_ENVIRONMENT; }
    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);

    void SetDefault();
    void Apply(CRenderer *renderer);

    // ambient
    CVector3 m_AmbientColor[2];
    CVector3 m_LightMapAmbientColor[2];
    CVector3 m_AOShadowColor;

    // fog
    bool m_bEnableFog;
    CVector3 m_FogColor[2];
    CVector3 m_FogDirection;
    float m_fFogStart;
    float m_fFogEnd;
    float m_fFogDensity[2];
    float m_fFogAlpha[2];

    // dof
    bool m_bEnableDof;
    float m_fFocalPlaneDistance;
    float m_fNearBlurPlaneDistance;
    float m_fFarBlurPlaneDistance;
    int m_iDofBlurRadius;
    float m_fDofBlurDeviation;

    // bloom
    bool m_bEnableBloom;
    float m_fBloomIntensity;
    float m_fBloomThreshold;
    int m_iBloomBlurRadius;
    float m_fBloomBlurDeviation;

    // light shaft
    bool m_bEnableLightShaft;
    CVector3 m_LightShaftColor;
    CVector3 m_LightShaftDirection;
    float m_fLightShaftThreshold;
    float m_fLightShaftIntensity;
    float m_fLightShaftBackgroundIntensity;
    float m_fLightShaftRange;
    float m_fLightShaftBlurLength;

    // tone mapping
    bool m_bEnableToneMapping;

    // sky
    float m_fSkyDistance;

    // background
    CVector3 m_BackgroundColor;

    // color
    float m_fExposure;
    float m_fSaturation;

    // gamma
    bool m_bEnableColorGrading;
    std::string m_ColorGradingName;
    CColorGrading m_ColorGrading;
};


}//GELib

#endif//__GELIB_SCENE_ENVIRONMENTSCENEENTITY_H__
