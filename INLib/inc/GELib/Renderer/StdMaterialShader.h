//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/StdMaterialShader.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_STDMATERIALSHADER_H__
#define __GELIB_RENDERER_STDMATERIALSHADER_H__

#include "MeshShader.h"

namespace GELib
{


class CRendererContext;


class CStdMaterialShader : public CMeshShader
{
public:
    CStdMaterialShader();
    virtual ~CStdMaterialShader();

    virtual void SetParameters(CRendererContext &rc);

    CShaderParameter m_Samplers;
    CShaderParameter m_Vectors;
    CShaderParameter m_GVectors;
    CShaderParameter m_SceneDepthSampler;
    CShaderParameter m_DistortionSampler;
    CShaderParameter m_Time;
    CShaderParameter m_Alpha;
    CShaderParameter m_Noise;
    CShaderParameter m_ObjectColor;
    CShaderParameter m_ViewMatrix3x3;
    CShaderParameter m_ScreenTexCoordOffsetScale;
};

class CStdDepthShader : public CStdMaterialShader
{
public:
    CStdDepthShader();
    virtual ~CStdDepthShader();
    virtual void SetParameters(CRendererContext &rc);
};

class CStdShadowDepthShader : public CStdMaterialShader
{
public:
    CStdShadowDepthShader();
    virtual ~CStdShadowDepthShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_ShadowMatrix;
};

class CStdVolumeDepthShader : public CMeshShader
{
public:
    CStdVolumeDepthShader();
    virtual ~CStdVolumeDepthShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_DepthOffsetScale;
};

class CStdVolumeColorShader : public CStdMaterialShader
{
public:
    CStdVolumeColorShader();
    virtual ~CStdVolumeColorShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_VolumeDepthSampler;
};

class CStdLightShader : public CStdMaterialShader
{
public:
    CStdLightShader();
    virtual ~CStdLightShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_CameraPosition;
    CShaderParameter m_LightColor;
    CShaderParameter m_ShadowColor;
    CShaderParameter m_LightMapOffsetScale;
    CShaderParameter m_LightMapSampler;
    CShaderParameter m_ShadowProjSampler;
    CShaderParameter m_FogColor;
    CShaderParameter m_FogColor2;
    CShaderParameter m_FogDistance;
    CShaderParameter m_FogDirection;
};

class CStdPointLightShader : public CStdLightShader
{
public:
    CStdPointLightShader();
    virtual ~CStdPointLightShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_LightPosition;
};

class CStdSpotLightShader : public CStdPointLightShader
{
public:
    CStdSpotLightShader();
    virtual ~CStdSpotLightShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_LightDirection;
    CShaderParameter m_LightCone;
};

class CStdDirectionalLightShader : public CStdLightShader
{
public:
    CStdDirectionalLightShader();
    virtual ~CStdDirectionalLightShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_LightDirection;
};

class CStdAmbientEmissiveShader : public CStdMaterialShader
{
public:
    CStdAmbientEmissiveShader();
    virtual ~CStdAmbientEmissiveShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_CameraPosition;
    CShaderParameter m_AmbientColor1;
    CShaderParameter m_AmbientColor2;
    CShaderParameter m_AmbientDirection;
    CShaderParameter m_AOShadowColor;
    CShaderParameter m_LightMapOffsetScale;
    CShaderParameter m_LightMapColorScale;
    CShaderParameter m_LightMapSampler;
    CShaderParameter m_LightMapSampler2;
    CShaderParameter m_FogColor;
    CShaderParameter m_FogColor2;
    CShaderParameter m_FogDistance;
    CShaderParameter m_FogDirection;
    CShaderParameter m_ShadowProjSampler;
};

class CStdAmbientDirectionalLightShader : public CStdLightShader
{
public:
    CStdAmbientDirectionalLightShader();
    virtual ~CStdAmbientDirectionalLightShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_AmbientColor1;
    CShaderParameter m_AmbientColor2;
    CShaderParameter m_AmbientDirection;
    CShaderParameter m_AOShadowColor;
    CShaderParameter m_LightDirection;
};

class CStdDistortionShader : public CStdMaterialShader
{
public:
    CStdDistortionShader();
    virtual ~CStdDistortionShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_CameraPosition;
    CShaderParameter m_SceneSampler;
};

class CStdDistortionAccumulateShader : public CStdMaterialShader
{
public:
    CStdDistortionAccumulateShader();
    virtual ~CStdDistortionAccumulateShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_CameraPosition;
};

class CStdGlowShader : public CStdMaterialShader
{
public:
    CStdGlowShader();
    virtual ~CStdGlowShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_CameraPosition;
};

class CStdFlatColorShader : public CMeshShader
{
public:
    CStdFlatColorShader();
    virtual ~CStdFlatColorShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_Color;
};

class CStdFlatColorAlphaTestShader : public CStdMaterialShader
{
public:
    CStdFlatColorAlphaTestShader();
    virtual ~CStdFlatColorAlphaTestShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_Color;
};

class CStdLightMaterialShader : public CStdMaterialShader
{
public:
    CStdLightMaterialShader();
    virtual ~CStdLightMaterialShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_Color;
    CShaderParameter m_ScreenToLightMatrix;
    CShaderParameter m_ScreenToLightProjMatrix;
};

class CStdProjectorShader: public CStdMaterialShader
{
public:
    CStdProjectorShader (void);
    virtual ~CStdProjectorShader (void);

    virtual void SetParameters (CRendererContext &rc);
    CShaderParameter m_Color;
    CShaderParameter m_WorldToLightMatrix;
    CShaderParameter m_WorldToLightProjMatrix;
    CShaderParameter m_LightDirection;
};

class CStdVertexLightShader : public CStdMaterialShader
{
public:
    CStdVertexLightShader();
    virtual ~CStdVertexLightShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_CameraPosition;
    CShaderParameter m_AmbientColor1;
    CShaderParameter m_AmbientColor2;
    CShaderParameter m_AmbientDirection;
    CShaderParameter m_LightColor[2];
    CShaderParameter m_LightPosition[2];
    CShaderParameter m_ShadowColor;
    CShaderParameter m_LightMapOffsetScale;
    CShaderParameter m_LightMapColorScale;
    CShaderParameter m_LightMapSampler;
    CShaderParameter m_AOShadowColor;
    CShaderParameter m_FogColor;
    CShaderParameter m_FogColor2;
    CShaderParameter m_FogDistance;
    CShaderParameter m_FogDirection;
};

class CStdVertexLightLowShader : public CStdMaterialShader
{
public:
    CStdVertexLightLowShader();
    virtual ~CStdVertexLightLowShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_CameraPosition;
    CShaderParameter m_AmbientColor1;
    CShaderParameter m_AmbientColor2;
    CShaderParameter m_AmbientDirection;
    CShaderParameter m_LightColor;
    CShaderParameter m_LightPosition;
    CShaderParameter m_LightMapOffsetScale;
    CShaderParameter m_LightMapColorScale;
    CShaderParameter m_LightMapSampler;
    CShaderParameter m_FogColor;
    CShaderParameter m_FogColor2;
    CShaderParameter m_FogDistance;
    CShaderParameter m_FogDirection;
};

class CStdVertexSkyShader : public CStdMaterialShader
{
public:
    CStdVertexSkyShader();
	virtual ~CStdVertexSkyShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_CameraPosition;
    CShaderParameter m_FogColor;
    CShaderParameter m_FogColor2;
    CShaderParameter m_FogDistance;
    CShaderParameter m_FogDirection;

};

class CStdVertexSkyLowShader : public CStdVertexSkyShader
{
public:
    CStdVertexSkyLowShader();
};

class CStdVelocityShader : public CStdMaterialShader
{
public:
    CStdVelocityShader();
    virtual ~CStdVelocityShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_PrevViewProjMatrix;
    CShaderParameter m_VelocityOffsetScale;
};

class CStdGBufferShader : public CStdMaterialShader
{
public:
    CStdGBufferShader();
    virtual ~CStdGBufferShader();
    virtual void SetParameters(CRendererContext &rc);

    CShaderParameter m_LightMapOffsetScale;
    CShaderParameter m_LightMapSampler;
    CShaderParameter m_LightOcclusion;
};

class CStdDeferredLightShader : public CStdMaterialShader
{
public:
    CStdDeferredLightShader();
    virtual ~CStdDeferredLightShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_LightColor;
    CShaderParameter m_ShadowColor;
    CShaderParameter m_ScreenToWorldMatrix;
    CShaderParameter m_ScreenToLightMatrix;
    CShaderParameter m_ScreenToLightProjMatrix;
    CShaderParameter m_SceneNormalSampler;
    CShaderParameter m_ShadowProjSampler;
};

class CStdDeferredPointLightShader : public CStdDeferredLightShader
{
public:
    CStdDeferredPointLightShader();
    virtual ~CStdDeferredPointLightShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_LightPosition;
};

class CStdDeferredSpotLightShader : public CStdDeferredPointLightShader
{
public:
    CStdDeferredSpotLightShader();
    virtual ~CStdDeferredSpotLightShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_LightDirection;
    CShaderParameter m_LightCone;
};

class CStdDeferredDirectionalLightShader : public CStdDeferredLightShader
{
public:
    CStdDeferredDirectionalLightShader();
    virtual ~CStdDeferredDirectionalLightShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_LightDirection;
};

class CStdAmbientDeferredLightShader : public CStdMaterialShader
{
public:
    CStdAmbientDeferredLightShader();
    virtual ~CStdAmbientDeferredLightShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_CameraPosition;
    CShaderParameter m_AmbientColor1;
    CShaderParameter m_AmbientColor2;
    CShaderParameter m_AmbientDirection;
    CShaderParameter m_AOShadowColor;
    CShaderParameter m_LightMapOffsetScale;
    CShaderParameter m_LightMapSampler;
    CShaderParameter m_FogColor;
    CShaderParameter m_FogColor2;
    CShaderParameter m_FogDistance;
    CShaderParameter m_FogDirection;
    CShaderParameter m_SceneLightSampler;
    CShaderParameter m_ShadowProjSampler;
};

class CLightMapShader : public CStdMaterialShader
{
public:
    CLightMapShader();
    virtual ~CLightMapShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_LightColor;
    CShaderParameter m_LightPosition;
    CShaderParameter m_LightDirection;
    CShaderParameter m_LightCone;
    CShaderParameter m_AmbientColor1;
    CShaderParameter m_AmbientColor2;
    CShaderParameter m_AmbientDirection;
    CShaderParameter m_ShadowColor;
    CShaderParameter m_ScreenTexCoordOffsetScale;
    CShaderParameter m_ShadowProjSampler;
    CShaderParameter m_NormalSampler;
    CShaderParameter m_Selection;
};

class CLightMapAmbientShader : public CStdMaterialShader
{
public:
    CLightMapAmbientShader();
    virtual ~CLightMapAmbientShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_AmbientColor1;
    CShaderParameter m_AmbientColor2;
    CShaderParameter m_AmbientDirection;
    CShaderParameter m_Selection;
    CShaderParameter m_NormalSampler;
};

class CLightMapShadowProjectionShader : public CMeshShader
{
public:
    CLightMapShadowProjectionShader();
    virtual ~CLightMapShadowProjectionShader();

    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_WorldToShadowMatrix;
    CShaderParameter m_ShadowDepthSampler;
};

class CDepthMapShader : public CStdDepthShader
{
public:
    CDepthMapShader();
    virtual ~CDepthMapShader();
    virtual void SetParameters(CRendererContext &rc);
    CShaderParameter m_ViewMatrix;
};

class CTextureResolutionShader : public CStdMaterialShader
{
public:
    CTextureResolutionShader();
    virtual ~CTextureResolutionShader();
    virtual void SetParameters(CRendererContext &rc);
	CShaderParameter m_TextureSize;
    CShaderParameter m_MipColorSampler;
};


}//GELib

#endif//__GELIB_RENDERER_STDMATERIALSHADER_H__
