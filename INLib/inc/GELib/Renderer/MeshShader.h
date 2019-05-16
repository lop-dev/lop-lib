//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/MeshShader.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_MESHSHADER_H__
#define __GELIB_RENDERER_MESHSHADER_H__

#include "Shader.h"
#include "RendererTypes.h"

namespace GELib
{


class CRendererContext;

class CMeshShaderParameters
{
public:
    virtual ~CMeshShaderParameters() {}
};


class CMeshShader : public CShader
{
public:
    CMeshShader();
    virtual ~CMeshShader();

    virtual void SetParameters(CRendererContext &rc);

    void SetMVectors(const CVector4 *vectors, int num);

    CMeshShaderParameters *m_pMeshShaderParameters;
    CShaderParameter m_ViewProjMatrix;
    CShaderParameter m_MVectors;
};

class CPrimitiveShader : public CShader
{
public:
    CPrimitiveShader();
    virtual ~CPrimitiveShader();

    virtual bool Initialize();
    virtual void SetParameters(CRendererContext &rc);

    CShaderParameter m_ViewProjMatrix;
};

class CShadowProjectionShader : public CShader
{
public:
    CShadowProjectionShader();
    virtual ~CShadowProjectionShader();

    bool Initialize();
    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_ViewProjMatrix;
    CShaderParameter m_ScreenToShadowMatrix;
    CShaderParameter m_ScreenTexCoordOffsetScale;
    CShaderParameter m_ShadowColor;
    CShaderParameter m_SceneDepthSampler;
    CShaderParameter m_ShadowDepthSampler;
};

class CPSSMProjectionShader: public CShader
{
public:
    CPSSMProjectionShader();
    virtual ~CPSSMProjectionShader();

    bool Initialize();
    virtual void SetParameters (CRendererContext &rc);

    CShaderParameter m_ViewProjMatrix;
    CShaderParameter m_ScreenTexCoordOffsetScale;
    CShaderParameter m_ShadowColor;
    CShaderParameter m_SceneDepthSampler;
    CShaderParameter m_ShadowDepthSampler;
    CShaderParameter m_SplitMatrix;
    CShaderParameter m_RandomRotationSampler;
    CShaderParameter m_ShadowSizeParameter;
    CShaderParameter m_FadeBegin;
    CShaderParameter m_InvFadeRange;
    CShaderParameter m_DepthRange;
    //CShaderParameter m_One;
    CShaderParameter m_ScreenToLightMatrix;
};

class CPSSMProjectionHighQualityShader: public CPSSMProjectionShader
{
public:
    CPSSMProjectionHighQualityShader();
    bool Initialize();
};

class CPSSMVarianceProjectionShader: public CShader
{
public:
    CPSSMVarianceProjectionShader();
    virtual ~CPSSMVarianceProjectionShader();

    bool Initialize();
    virtual void SetParameters (CRendererContext &rc);

    CShaderParameter m_ViewProjMatrix;
    CShaderParameter m_ScreenTexCoordOffsetScale;
    CShaderParameter m_ShadowColor;
    CShaderParameter m_SceneDepthSampler;
    CShaderParameter m_ShadowDepthSampler;
    CShaderParameter m_SplitMatrix;
    CShaderParameter m_FadeBegin;
    CShaderParameter m_InvFadeRange;
    CShaderParameter m_DepthRange;
};

class CVarianceShadowProjectionShader : public CShader
{
public:
    CVarianceShadowProjectionShader();
    virtual ~CVarianceShadowProjectionShader();

    bool Initialize();
    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_ViewProjMatrix;
    CShaderParameter m_ScreenToShadowMatrix;
    CShaderParameter m_ScreenTexCoordOffsetScale;
    CShaderParameter m_ShadowColor;
    CShaderParameter m_SceneDepthSampler;
    CShaderParameter m_VarianceShadowDepthSampler;
};

class CBlurShadowProjectionShader : public CShader
{
public:
    CBlurShadowProjectionShader();
    virtual ~CBlurShadowProjectionShader();

    bool Initialize();
    void SetSampleOffsets(const CVector4 offsets[2]);
    CShaderParameter m_ShadowSampler;
    CShaderParameter m_SceneDepthSampler;
    CShaderParameter m_SampleOffsets;
};

class CBlurVarianceShadowDepthShader : public CShader
{
public:
    CBlurVarianceShadowDepthShader();
    virtual ~CBlurVarianceShadowDepthShader();

    bool Initialize();
    void SetSampleWeights(const CVector4 weights[13]);
    CShaderParameter m_ShadowDepthSampler;
    CShaderParameter m_SampleWeights;
};

class CCopyVarianceShadowDepthShader : public CShader
{
public:
    CCopyVarianceShadowDepthShader();
    virtual ~CCopyVarianceShadowDepthShader();

    bool Initialize();
    CShaderParameter m_ShadowDepthSampler;
    CShaderParameter m_SampleWeight;
};

class CVolumeIntersectionShader : public CMeshShader
{
public:
    CVolumeIntersectionShader();
    virtual ~CVolumeIntersectionShader();

    bool Initialize();
    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_SceneDepthSampler;
    CShaderParameter m_DepthOffsetScale;
    CShaderParameter m_ScreenTexCoordOffsetScale;
};

// modified by bob -- start
class CSeaWaveShader : public CShader
{
public:
    CSeaWaveShader();
    virtual ~CSeaWaveShader();

    virtual bool Initialize();
    CShaderParameter m_Sampler;
    CShaderParameter m_HeightRatioAndStep;
};


class CPerlinOctaveMakerShader : public CShader
{
public:
    CPerlinOctaveMakerShader();
    virtual ~CPerlinOctaveMakerShader();
    virtual bool Initialize();
    CShaderParameter m_Sampler;
    CShaderParameter m_SamplerScaler;
    CShaderParameter m_OctavesBlendFactor;
    CShaderParameter m_OctaveImageOffset0;
    CShaderParameter m_OctaveImageOffset1;
};

class CPerlinNoiseMakerShader : public CShader
{
public:
    CPerlinNoiseMakerShader();
    virtual ~CPerlinNoiseMakerShader();
    virtual bool Initialize();
    CShaderParameter m_OctaveSamplers;
    CShaderParameter m_ScaleAndFrequence;
};
// modified by bob -- end
}//GELib

#endif//__GELIB_RENDERER_MESHSHADER_H__
