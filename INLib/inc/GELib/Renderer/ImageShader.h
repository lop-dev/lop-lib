//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/ImageShader.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_IMAGESHADER_H__
#define __GELIB_RENDERER_IMAGESHADER_H__

#include "Shader.h"

namespace GELib
{


class CFillColorShader : public CShader
{
public:
    CFillColorShader();
    virtual ~CFillColorShader();
    virtual bool Initialize();

    CShaderParameter m_Color;
};

class CFillTextureShader : public CShader
{
public:
    CFillTextureShader();
    virtual ~CFillTextureShader();
    virtual bool Initialize();

    CShaderParameter m_Sampler;
};

class CFillTextureWithAlphaShader : public CShader
{
public:
    CFillTextureWithAlphaShader();
    virtual ~CFillTextureWithAlphaShader();
    virtual bool Initialize();

    CShaderParameter m_Sampler;
    CShaderParameter m_Alpha;
};

class CFillTextureWithMultiplierAndAdderShader : public CShader
{
public:
    CFillTextureWithMultiplierAndAdderShader();
    virtual ~CFillTextureWithMultiplierAndAdderShader();
    virtual bool Initialize();

    CShaderParameter m_Sampler;
    CShaderParameter m_ColorMultiplier;
    CShaderParameter m_ColorAdder;
};

class CGammaTextureShader : public CShader
{
public:
    CGammaTextureShader();
    virtual ~CGammaTextureShader();
    virtual bool Initialize();

    CShaderParameter m_Sampler;
    CShaderParameter m_Gamma;
};

class CDownSample4x4Shader : public CShader
{
public:
    CDownSample4x4Shader();
    virtual ~CDownSample4x4Shader();

    virtual bool Initialize();

    CShaderParameter m_Sampler;
};

class CDownSample3x3Shader : public CShader
{
public:
    CDownSample3x3Shader();
    virtual ~CDownSample3x3Shader();
    virtual bool Initialize();

    CShaderParameter m_Sampler;
};

class CDownSample2x2Shader : public CShader
{
public:
    CDownSample2x2Shader();
    virtual ~CDownSample2x2Shader();
    virtual bool Initialize();

    CShaderParameter m_Sampler;
};

class CDownSampleMax4x4Shader : public CShader
{
public:
    CDownSampleMax4x4Shader();
    virtual ~CDownSampleMax4x4Shader();

    virtual bool Initialize();

    CShaderParameter m_Sampler;
};

class CDownSampleMax2x2Shader : public CShader
{
public:
    CDownSampleMax2x2Shader();
    virtual ~CDownSampleMax2x2Shader();

    virtual bool Initialize();

    CShaderParameter m_Sampler;
};

class CDownSampleMin4x4Shader : public CShader
{
public:
    CDownSampleMin4x4Shader();
    virtual ~CDownSampleMin4x4Shader();

    virtual bool Initialize();

    CShaderParameter m_Sampler;
};

class CDownSampleMin2x2Shader : public CShader
{
public:
    CDownSampleMin2x2Shader();
    virtual ~CDownSampleMin2x2Shader();

    virtual bool Initialize();

    CShaderParameter m_Sampler;
};

class CNearestDepthUpSampleShader : public CShader
{
public:
    CNearestDepthUpSampleShader();
    virtual ~CNearestDepthUpSampleShader();

    virtual bool Initialize();

    CShaderParameter m_Sampler;
    CShaderParameter m_Sampler1;
    CShaderParameter m_DepthSampler;
    CShaderParameter m_LowDepthSampler;
    CShaderParameter m_DepthThreshold;
};

class CSampleShader : public CShader
{
public:
    CSampleShader();
    virtual ~CSampleShader();

    void SetSampleWeights(const CVector4 *weights, int num);

    CShaderParameter m_Sampler;
    CShaderParameter m_SampleWeights;
};

class CGaussianFilter3x3Shader : public CSampleShader
{
public:
    CGaussianFilter3x3Shader();
    virtual ~CGaussianFilter3x3Shader();
    virtual bool Initialize();
};

class CGaussianFilter5x5Shader : public CSampleShader
{
public:
    CGaussianFilter5x5Shader();
    virtual ~CGaussianFilter5x5Shader();
    virtual bool Initialize();
};

class CGaussianFilter15x15Shader : public CSampleShader
{
public:
    CGaussianFilter15x15Shader();
    virtual ~CGaussianFilter15x15Shader();
    virtual bool Initialize();
};

class CDownSample4x4BilinearShader: public CShader
{
public:
    CDownSample4x4BilinearShader();
    ~CDownSample4x4BilinearShader();
    virtual bool Initialize();

    CShaderParameter m_Sampler;
};

class CBrightPassShader : public CShader
{
public:
    CBrightPassShader();
    virtual ~CBrightPassShader();
    virtual bool Initialize();

    CShaderParameter m_Sampler;
    CShaderParameter m_BrightPassThreshold;
    CShaderParameter m_BrightPassScale;
};

class CGaussianFilterBilinearShader : public CSampleShader
{
public:
    CGaussianFilterBilinearShader();
    ~CGaussianFilterBilinearShader();

    enum
    {
        MAX_RADIUS = 15,
    };

protected:
    bool Init(int radius);
};

class CGaussianFilterBilinear1Shader : public CGaussianFilterBilinearShader
{
public:
    CGaussianFilterBilinear1Shader() { m_eFunction = SHADER_FUNCTION_GAUSSIAN_FILTER_BILINEAR1; }
    virtual bool Initialize() { return Init(1); }
};

class CGaussianFilterBilinear2Shader : public CGaussianFilterBilinearShader
{
public:
    CGaussianFilterBilinear2Shader() { m_eFunction = SHADER_FUNCTION_GAUSSIAN_FILTER_BILINEAR2; }
    virtual bool Initialize() { return Init(2); }
};

class CGaussianFilterBilinear3Shader : public CGaussianFilterBilinearShader
{
public:
    CGaussianFilterBilinear3Shader() { m_eFunction = SHADER_FUNCTION_GAUSSIAN_FILTER_BILINEAR3; }
    virtual bool Initialize() { return Init(3); }
};

class CGaussianFilterBilinear4Shader : public CGaussianFilterBilinearShader
{
public:
    CGaussianFilterBilinear4Shader() { m_eFunction = SHADER_FUNCTION_GAUSSIAN_FILTER_BILINEAR4; }
    virtual bool Initialize() { return Init(4); }
};

class CGaussianFilterBilinear5Shader : public CGaussianFilterBilinearShader
{
public:
    CGaussianFilterBilinear5Shader() { m_eFunction = SHADER_FUNCTION_GAUSSIAN_FILTER_BILINEAR5; }
    virtual bool Initialize() { return Init(5); }
};

class CGaussianFilterBilinear6Shader : public CGaussianFilterBilinearShader
{
public:
    CGaussianFilterBilinear6Shader() { m_eFunction = SHADER_FUNCTION_GAUSSIAN_FILTER_BILINEAR6; }
    virtual bool Initialize() { return Init(6); }
};

class CGaussianFilterBilinear7Shader : public CGaussianFilterBilinearShader
{
public:
    CGaussianFilterBilinear7Shader() { m_eFunction = SHADER_FUNCTION_GAUSSIAN_FILTER_BILINEAR7; }
    virtual bool Initialize() { return Init(7); }
};

class CGaussianFilterBilinear8Shader: public CGaussianFilterBilinearShader
{
public:
    CGaussianFilterBilinear8Shader() { m_eFunction = SHADER_FUNCTION_GAUSSIAN_FILTER_BILINEAR8; }
    virtual bool Initialize() { return Init(8); }
};

class CGaussianFilterBilinear9Shader : public CGaussianFilterBilinearShader
{
public:
    CGaussianFilterBilinear9Shader() { m_eFunction = SHADER_FUNCTION_GAUSSIAN_FILTER_BILINEAR9; }
    virtual bool Initialize() { return Init(9); }
};

class CGaussianFilterBilinear10Shader : public CGaussianFilterBilinearShader
{
public:
    CGaussianFilterBilinear10Shader() { m_eFunction = SHADER_FUNCTION_GAUSSIAN_FILTER_BILINEAR10; }
    virtual bool Initialize() { return Init(10); }
};

class CGaussianFilterBilinear11Shader: public CGaussianFilterBilinearShader
{
public:
    CGaussianFilterBilinear11Shader() { m_eFunction = SHADER_FUNCTION_GAUSSIAN_FILTER_BILINEAR11; }
    virtual bool Initialize() { return Init(11); }
};

class CGaussianFilterBilinear12Shader : public CGaussianFilterBilinearShader
{
public:
    CGaussianFilterBilinear12Shader() { m_eFunction = SHADER_FUNCTION_GAUSSIAN_FILTER_BILINEAR12; }
    virtual bool Initialize() { return Init(12); }
};

class CGaussianFilterBilinear13Shader : public CGaussianFilterBilinearShader
{
public:
    CGaussianFilterBilinear13Shader() { m_eFunction = SHADER_FUNCTION_GAUSSIAN_FILTER_BILINEAR13; }
    virtual bool Initialize() { return Init(13); }
};

class CGaussianFilterBilinear14Shader : public CGaussianFilterBilinearShader
{
public:
    CGaussianFilterBilinear14Shader() { m_eFunction = SHADER_FUNCTION_GAUSSIAN_FILTER_BILINEAR14; }
    virtual bool Initialize() { return Init(14); }
};

class CGaussianFilterBilinear15Shader: public CGaussianFilterBilinearShader
{
public:
    CGaussianFilterBilinear15Shader() { m_eFunction = SHADER_FUNCTION_GAUSSIAN_FILTER_BILINEAR15; }
    virtual bool Initialize() { return Init(15); }
};

class CDofFilterBilinearShader : public CSampleShader
{
public:
    CDofFilterBilinearShader ();
    ~CDofFilterBilinearShader();

    CShaderParameter m_DepthSampler;
    CShaderParameter m_DofDistance;

    enum
    {
        MAX_RADIUS = 5,
    };

protected:
    bool Init(int radius);
};


class CDofFilterBilinear1Shader : public CDofFilterBilinearShader
{
public:
    CDofFilterBilinear1Shader() { m_eFunction = SHADER_FUNCTION_DOF_FILTER_BILINEAR1; }
    virtual bool Initialize() { return Init(1); }
};

class CDofFilterBilinear2Shader : public CDofFilterBilinearShader
{
public:
    CDofFilterBilinear2Shader() { m_eFunction = SHADER_FUNCTION_DOF_FILTER_BILINEAR2; }
    virtual bool Initialize() { return Init(2); }
};

class CDofFilterBilinear3Shader : public CDofFilterBilinearShader
{
public:
    CDofFilterBilinear3Shader() { m_eFunction = SHADER_FUNCTION_DOF_FILTER_BILINEAR3; }
    virtual bool Initialize() { return Init(3); }
};

class CDofFilterBilinear4Shader : public CDofFilterBilinearShader
{
public:
    CDofFilterBilinear4Shader() { m_eFunction = SHADER_FUNCTION_DOF_FILTER_BILINEAR4; }
    virtual bool Initialize() { return Init(4); }
};

class CDofFilterBilinear5Shader : public CDofFilterBilinearShader
{
public:
    CDofFilterBilinear5Shader() { m_eFunction = SHADER_FUNCTION_DOF_FILTER_BILINEAR5; }
    virtual bool Initialize() { return Init(5); }
};

class CBlurLightMapShader : public CShader
{
public:
    CBlurLightMapShader();
    ~CBlurLightMapShader();
    virtual bool Initialize();

    CShaderParameter m_Sampler;
};

class CEdgePaddingShader : public CShader
{
public:
    CEdgePaddingShader();
    ~CEdgePaddingShader();
    virtual bool Initialize();

    CShaderParameter m_Sampler;
};

class CExtractMonoShadowShader : public CShader
{
public:
    CExtractMonoShadowShader();
    ~CExtractMonoShadowShader();
    virtual bool Initialize();

    CShaderParameter m_Sampler;
    CShaderParameter m_Sampler1;
};

class CExtractColorShadowShader : public CShader
{
public:
    CExtractColorShadowShader();
    ~CExtractColorShadowShader();
    virtual bool Initialize();

    CShaderParameter m_Sampler;
    CShaderParameter m_Sampler1;
};


class CEncodeRGBMShader : public CShader
{
public:
    CEncodeRGBMShader();
    ~CEncodeRGBMShader();
    virtual bool Initialize();

    CShaderParameter m_ColorMultiplier;
    CShaderParameter m_Sampler;
};

class CComputeBasisIntensityShader : public CShader
{
public:
    CComputeBasisIntensityShader();
    ~CComputeBasisIntensityShader();
    virtual bool Initialize();

    CShaderParameter m_Sampler;
};


}//GELib

#endif//__GELIB_RENDERER_IMAGESHADER_H__
