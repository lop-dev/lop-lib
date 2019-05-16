//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/PostProcessShader.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_POSTPROCESSSHADER_H__
#define __GELIB_RENDERER_POSTPROCESSSHADER_H__

#include "Shader.h"

namespace GELib
{


class CFogShader : public CShader
{
public:
    CFogShader();
    virtual ~CFogShader();
    virtual bool Initialize();

    CShaderParameter m_SceneDepthSampler;
    CShaderParameter m_FogColor;
    CShaderParameter m_FogColor2;
    CShaderParameter m_FogDistance;
    CShaderParameter m_FogDirection;
    CShaderParameter m_ScreenToViewMatrix;
};

class CCameraVelocityShader : public CShader
{
public:
    CCameraVelocityShader();
    virtual ~CCameraVelocityShader();
    virtual bool Initialize();

    CShaderParameter m_SceneDepthSampler;
    CShaderParameter m_ScreenToPrevProjMatrix;
    CShaderParameter m_VelocityOffsetScale;
};

class CMotionBlurShader : public CShader
{
public:
    CMotionBlurShader();
    virtual ~CMotionBlurShader();
    virtual bool Initialize();

    CShaderParameter m_SceneSampler;
    CShaderParameter m_VelocitySampler;
};

class CMotionBlurHighQualityShader : public CMotionBlurShader
{
public:
    CMotionBlurHighQualityShader();
    virtual bool Initialize();
};

class CDistortionApplyShader : public CShader
{
public:
    CDistortionApplyShader();
    virtual ~CDistortionApplyShader();
    virtual bool Initialize();

    CShaderParameter m_SceneSampler;
    CShaderParameter m_DistortionSampler;
};

class CSsaoShader : public CShader
{
public:
    CSsaoShader();
    virtual ~CSsaoShader();

    bool Initialize();

    CShaderParameter m_SceneDepthSampler;
    CShaderParameter m_DitherTextureSampler;
    CShaderParameter m_DitherTextureRatio;
    CShaderParameter m_Radius;
    CShaderParameter m_Threshold;
    CShaderParameter m_Attenuation;
};

class CBuildLightShaftShader : public CShader
{
public:
    CBuildLightShaftShader();
    virtual ~CBuildLightShaftShader();
    virtual bool Initialize();

    CShaderParameter m_LightTexCoord;
    CShaderParameter m_Color;
    CShaderParameter m_Occlusion;
    CShaderParameter m_SceneSampler;
    CShaderParameter m_SceneDepthSampler;
};

class CBlurLightShaftShader : public CShader
{
public:
    CBlurLightShaftShader();
    virtual ~CBlurLightShaftShader();
    virtual bool Initialize();

    CShaderParameter m_LightTexCoord;
    CShaderParameter m_BlurLength;
    CShaderParameter m_BlurWeights;
    CShaderParameter m_Step;
    CShaderParameter m_LightShaftSampler;
};

class CFxaaShader : public CShader
{
public:
	CFxaaShader();
	virtual ~CFxaaShader();
	virtual bool Initialize();

    CShaderParameter m_SceneSampler;
	CShaderParameter m_QualityRcpFrame;
	CShaderParameter m_ConsoleRcpFrameOpt;
	CShaderParameter m_ConsoleRcpFrameOpt2;
	CShaderParameter m_Console360RcpFrameOpt2;
	CShaderParameter m_Quality;
	CShaderParameter m_ConsoleEdge;
	CShaderParameter m_Console360ConstDir;
};

class CFxaaHighQualityShader : public CFxaaShader
{
public:
	CFxaaHighQualityShader();
	virtual ~CFxaaHighQualityShader();
	virtual bool Initialize();
};

class CPostProcessShader : public CShader
{
public:
    CPostProcessShader();
    virtual ~CPostProcessShader();
    virtual bool Initialize();

    CShaderParameter m_SceneSampler;
    CShaderParameter m_SceneDepthSampler;
    CShaderParameter m_DofSampler;
    CShaderParameter m_BloomSampler;
    CShaderParameter m_ColorGradingSampler;
    CShaderParameter m_DofDistance;
    CShaderParameter m_Exposure;
    CShaderParameter m_Gamma;
    CShaderParameter m_Saturation;
    CShaderParameter m_ColorMultiplier;
    CShaderParameter m_ColorAdder;
};

class CPostProcessDofShader : public CPostProcessShader
{
public:
    CPostProcessDofShader();
    virtual bool Initialize();
};

class CPostProcessColorGradingShader : public CPostProcessShader
{
public:
    CPostProcessColorGradingShader();
    virtual bool Initialize();
};

class CPostProcessToneShader : public CPostProcessShader
{
public:
    CPostProcessToneShader();
    virtual bool Initialize();
};

class CPostProcessLumShader : public CPostProcessShader
{
public:
    CPostProcessLumShader();
    virtual bool Initialize();
};

class CPostProcessDofColorGradingShader : public CPostProcessShader
{
public:
    CPostProcessDofColorGradingShader();
    virtual bool Initialize();
};

class CPostProcessDofToneShader : public CPostProcessShader
{
public:
    CPostProcessDofToneShader();
    virtual bool Initialize();
};

class CPostProcessDofLumShader : public CPostProcessShader
{
public:
    CPostProcessDofLumShader();
    virtual bool Initialize();
};

class CPostProcessColorGradingToneShader : public CPostProcessShader
{
public:
    CPostProcessColorGradingToneShader();
    virtual bool Initialize();
};

class CPostProcessColorGradingLumShader : public CPostProcessShader
{
public:
    CPostProcessColorGradingLumShader();
    virtual bool Initialize();
};

class CPostProcessDofColorGradingToneShader : public CPostProcessShader
{
public:
    CPostProcessDofColorGradingToneShader();
    virtual bool Initialize();
};

class CPostProcessDofColorGradingLumShader : public CPostProcessShader
{
public:
    CPostProcessDofColorGradingLumShader();
    virtual bool Initialize();
};

class CPostProcessDofColorGradingToneLumShader : public CPostProcessShader
{
public:
    CPostProcessDofColorGradingToneLumShader();
    virtual bool Initialize();
};

class CPostProcessToneLumShader : public CPostProcessShader
{
public:
    CPostProcessToneLumShader();
    virtual bool Initialize();
};

class CPostProcessDofToneLumShader : public CPostProcessShader
{
public:
    CPostProcessDofToneLumShader();
    virtual bool Initialize();
};

class CPostProcessColorGradingToneLumShader : public CPostProcessShader
{
public:
    CPostProcessColorGradingToneLumShader();
    virtual bool Initialize();
};



}//GELib

#endif//__GELIB_RENDERER_POSTPROCESSSHADER_H__
