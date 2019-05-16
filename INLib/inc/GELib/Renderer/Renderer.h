//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/Renderer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_RENDERER_H__
#define __GELIB_RENDERER_RENDERER_H__

#include <vector>
#include <GELib/GeConfig.h>
#include <GELib/Utility/Allocator.h>
#include <GELib/Math/Frustum.h>
#include "RendererTypes.h"
#include "RenderObject.h"
#include "Visibility.h"
#include "RenderMaterial.h"
#include "RenderMesh.h"
#include "RenderStateSet.h"
// modified by bob -- start
#include <GELib/Ext/PerlinNoiseMaker.h>
// modified by bob -- end


namespace GELib
{

class CImagePackLayout;

class CPlotter3D;
class CPlotter2D;
class CBufferedPlotter3D;
class CTextureFont;

class CMeshShader;
class CRenderBatch;
class CImageProcessor;
class CShadowInfo;
class CShaderSettings;
// modified by bob -- start
struct PerlinNoiseDesc;
// modified by bob -- end

class CStdVisObject;
class CStdVisibility;

class CFogShader;
class CCameraVelocityShader;
class CMotionBlurShader;
class CMotionBlurHighQualityShader;
class CDistortionApplyShader;
class CSsaoShader;
class CBuildLightShaftShader;
class CBlurLightShaftShader;
class CFxaaShader;
class CFxaaHighQualityShader;

class CPostProcessShader;
class CPostProcessDofShader;
class CPostProcessColorGradingShader;
class CPostProcessToneShader;
class CPostProcessLumShader;
class CPostProcessDofColorGradingShader;
class CPostProcessDofToneShader;
class CPostProcessDofLumShader;
class CPostProcessColorGradingToneShader;
class CPostProcessColorGradingLumShader;
class CPostProcessDofColorGradingToneShader;
class CPostProcessDofColorGradingLumShader;
class CPostProcessDofColorGradingToneLumShader;
class CPostProcessToneLumShader;
class CPostProcessDofToneLumShader;
class CPostProcessColorGradingToneLumShader;

const int NUM_LOD_FACTORS = 8;

class CColorGrading
{
public:
    void SetDefault();
    void Invert();

	unsigned char rgb[16 * 16 * 16 * 3];
};

class CRendererEnvironmentSettings
{
public:
    GELIB_API CRendererEnvironmentSettings();
    GELIB_API void Reset();

    enum
    {
        MAX_MATERIAL_GLOBAL_VECTORS = 8
    };

    // material parameter
    CVector4 m_MaterialGlobalVectors[MAX_MATERIAL_GLOBAL_VECTORS];
    float m_fAnimationTime;

    // ambient
    CVector3 m_AmbientColor[2];
    CVector3 m_LightMapAmbientColor[2];
    CVector3 m_AOShadowColor;

    // fog
    int m_iFogMode; // 0: off, 1: pixel fog, 2 vertex fog
    CVector4 m_FogColor[2];
    CVector3 m_FogDirection;
    float m_fFogStart;
    float m_fFogEnd;
    float m_fFogDensity[2];
    float m_fFogAlpha[2];

    // dof
    bool m_bEnableDof;
    bool m_bEnableDofAntiLeak;
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

    // color
    float m_fExposure;
    float m_fSaturation;

    // color grading
    bool m_bEnableColorGrading;
    CColorGrading m_ColorGrading;

    // sky
    float m_fSkyDistance;

    // background
    bool m_bEnableClearBackground;
    CVector3 m_BackgroundColor;

    // gamma
    float m_fGamma;

    // shadow
    bool m_bEnableShadow;
	bool m_bEnableSmoothShadow;
    float m_fShadowLimit;

    // motion blur
    int m_iMotionBlurMode;
    int m_iMotionBlurQuality;

    // edge glow
    bool  m_bEnableEdgeGlow;
    float m_fEdgeGlowIntensity;

    // SSAO
    bool m_bEnableSsao;
    int m_iSsaoBlurRadius;
    float m_fSsaoBlurDeviation;
    float m_fSsaoRadius;
    float m_fSsaoThreshold;
    float m_fSsaoAttenuation;

	// mixed resolution
    bool m_bEnableLowResolutionTranslucent;
    bool m_bEnableUpSamplingFilter;
	float m_fUpSamplingFilterThreshold;

    bool m_bEnableGlow;
    bool m_bEnableDistortion;
    bool m_bEnableVolume;
    bool m_bEnableLightMap;
    bool m_bEnableLightMaterial;
    bool m_bEnableLightOcclusion;
	bool m_bEnableAllVertexLight;
	bool m_bEnableEnvironmentLight;
    int  m_iTextureFilterQuality;
    int  m_iMaxAnisotropy;

	// post process anti-aliasing
	int m_iPostProcessAntiAliasingQuality;

    // debug, test
    int m_iShowRenderTarget;
    int m_iShowWireframe;
    int m_iShowOverdraw;
	int m_iShowTextureResolution;
    int m_iShowShadowFrustum;
    int m_iShowStatistic;
    int m_iReplaceMaterialTexture;
	float m_fShowMaxDepth;
	bool m_bEnableDepthPass;
	bool m_bEnableLighting;
};

class CRendererContext
{
public:
    GELIB_API CRendererContext();
    GELIB_API void Reset();

    int GetNumActiveLights();
    CRenderLight* GetActiveLight(int index);

    CMatrix m_CameraMatrix;
    CMatrix m_ViewMatrix;
    CMatrix m_ProjMatrix;
    CMatrix m_ViewProjMatrix;
    CMatrix m_ViewProjTMatrix;    // m_ViewProjTMatrix is clipped if m_bUseClipPlane is true
    CMatrix m_PrevViewMatrix;
    CMatrix m_PrevProjMatrix;
    CMatrix m_PrevViewProjMatrix;
    CMatrix m_PrevViewProjTMatrix;
    CMatrix m_ScreenToWorldMatrix;
    CFrustum m_Frustum;
    CVector3 m_CameraPosition;
    CVector3 m_CameraRotation;

    CVector2 m_ViewportOffset;
    CVector2 m_ViewportSize;
    CVector2 m_ViewportOffsetRatio;
    CVector2 m_ViewportSizeRatio;

    CVector4 m_ScreenTexCoordOffsetScale;
    CVector4 m_VolumeDepthOffsetScale;
    CVector4 m_VelocityOffsetScale;

    int m_iNearPlaneBoxCornerIndex;
    float m_LodFactors[NUM_LOD_FACTORS];

    int m_iBackBufferWidth;
    int m_iBackBufferHeight;
    int m_iShadowDepthSize;

    bool m_bUseClipPlane;
    CPlane m_ClipPlane;
    CFrustum m_ClippedFrustum;

    ShadowParameters m_Shadow;
    CMatrix m_PSSMMatrix;
    float m_fPSSMDepthRange;

    bool m_bShadowDepthPass;
    bool m_bTranslucentDepthPass;
    EFrontFace m_eFrontFace;
    EFrontFace m_eBackFace;
    float m_fTwoSidedSign;
    float m_fElapsedTime;
    CVector4 m_LightMapSelection;
	int m_iLightMapNormalTexture;

    CRendererEnvironmentSettings m_Env;
    CVector4 m_FogDistance;

    CVisibleLight *m_pVLight;
    CVisibleObject *m_pVObject;
    CRenderBatch *m_pBatch;
    CShadowInfo *m_pShadowInfo;

    int m_iSceneColorTarget;
    int m_iSceneDepthTarget;
    int m_iSceneNormalTarget;
    int m_iSceneLightTarget;
    int m_iDistortionSourceTarget;
    int m_iDistortionTarget;
    int m_iShadowDepthTarget;
    int m_iVarianceShadowDepthTarget;
    int m_iShadowProjTarget;
    int m_iVolumeDepthTarget;
    int m_iFinalTarget;
	bool m_bRenderTargetAlpha;

    int m_iWhiteTexture;
    int m_iWhiteCubeTexture;
    int m_iRandomRotationTexture;
    int m_iReplaceMaterialTexture;
	int m_iTextureResolutionMipTexture;

    CChunkObjectAllocator<CVisibleObject> *m_pVisibleObjectAllocator;
    CChunkObjectAllocator<CVisibleLight> *m_pVisibleLightAllocator;


    enum EContextType
    {
        NONE     = 0,
        MATERIAL = 1<<0,
        MESH     = 1<<1,
        OBJECT   = 1<<2,
        LIGHT    = 1<<3,
        LIGHTMAP  = 1<<4,
        SHADER   = 1<<5,
        ALL      = MATERIAL | MESH | OBJECT | LIGHT | LIGHTMAP | SHADER
    };
    int m_iDirtyContext;
    GELIB_API void Dirty(int ct) { m_iDirtyContext |= ct; }
    GELIB_API bool IsDirty(int ct) { return (m_iDirtyContext & ct) != 0; }
    GELIB_API void ClearDirty() { m_iDirtyContext = NONE; }
};

class CRendererStatistic
{
public:
    GELIB_API CRendererStatistic();
    GELIB_API void Reset();

    int m_iNumRenderedLights;
    int m_iNumRenderedProjectors;
    int m_iNumRenderedObjects;
    int m_iNumRenderedShadows;
    int m_iNumRenderedBatches;
    int m_iNumRenderedTriangles;
    int m_iNumVisibleBatches;
    int m_iNumVisibleTriangles;

    float m_fVisTime;
    float m_fBuildTime;
    float m_fSwapTime;
    float m_fDrawTime;
};

class CLightMapInfo
{
public:
    GELIB_API CLightMapInfo();
    int m_Textures[2];
    CVector2 m_Offset;        // uv offset
    CVector2 m_Scale;         // uv scale
    CVector4 m_ColorScale;
    ELightMapMode m_eMode;
};

class CRenderBatch
{
public:
    CMeshShader *m_pShader;
    CRenderMaterial *m_pMaterial;
    CRenderMesh *m_pMesh;
    void *m_pMeshShaderData;
    CVisibleObject *m_pVObject;
    CLightMapInfo *m_pLightMap;
    union
    {
        int m_iColor;
        float m_fLightOcclusion;
    };
    short m_iMeshChunk;
    short m_iMeshChunk2;
    short m_iQueryIndex;
    short m_iMaterialFlags;
    enum
    {
        TWO_SIDED  = 1<<0,
        TWO_SIDED_TWO_PASS  = 1<<1,
        DEPTH_TEST = 1<<2,
        ALPHA_TEST = 1<<3
    };
};

typedef std::vector<CRenderBatch*> CRenderBatchList;

// modified by bob -- start
class CWaterInfo
{
public:
    // generate normal using height map
    int m_iHeightMap;
    int m_iNormalMap;
    CVector4 m_ShaderParams;
    // need to generate height map using perlin noise
    bool m_bSea;
    PerlinNoiseDesc m_PerlinNoiseDesc;
};
typedef std::vector<CWaterInfo*> WaterInfoList;
// modified by bob -- end

class CShadowInfo
{
public:
    CMatrix m_ShadowMatrix;
    CMatrix m_ShadowWorldMatrix;
    CMatrix m_ShadowViewMatrix;
    CMatrix m_ShadowViewProjMatrix;
    CFrustum m_ShadowFrustum;
    CAABox m_CastBox;
    CAABox m_ReceiveBox;
    CRenderBatchList m_ShadowDepthBatches;
    CShadowInfo *m_pBaseShadowInfo;
    float m_fNear, m_fFar;
    float m_fAspect;
    float m_fTx, m_fTy, m_fTw, m_fTh;
    float m_fShadowResolutionFactor;
	float m_fAbsoluteBias;
    float m_fOpacity;
    float m_fBias;
	float m_fShadowCastHeight;
    bool m_bShadowDepthRendered;
    bool m_bNextPass;
};
typedef std::vector<CShadowInfo*> ShadowInfoList;

class CRenderView
{
public:
    GELIB_API CRenderView();
    GELIB_API virtual ~CRenderView();
	void SetLodFactors(float f);

    int m_iX, m_iY, m_iWidth, m_iHeight;    // viewport
    CMatrix m_CameraMatrix;           // camera front is -Z, right is X, up is Y
    CMatrix m_ProjMatrix;
    CMatrix m_PrevCameraMatrix;       // previous camera matrix, used by motion blur
    CMatrix m_PrevProjMatrix;         // previous projection matrix, used by motion blur
    CPlane m_ClipPlane;               // useful for reflection
    bool m_bUseClipPlane;
	bool m_bRenderTargetAlpha;			// render target alpha channel is correct
    int m_iRenderTarget;
    float m_LodFactors[NUM_LOD_FACTORS]; // if <=0, don't cull by distance
};

class CRenderLightMapContext
{
public:
    GELIB_API CRenderLightMapContext();
    GELIB_API virtual ~CRenderLightMapContext();

    GELIB_API void Allocate();
    GELIB_API void Free();

    // input parameters
    int m_iObjectId;        // target object visibility id
    int m_iWidth;
    int m_iHeight;
    ELightMapMode m_eMode;
    float m_fShadowDepthResolutionFactor;
	int m_iNormalTexture;	// terrain normal

    float m_fAmbientOcclusionQuality;
    float m_fAmbientOcclusionShadowDepthBias;
    float m_fAreaLightQuality;

    // output
    int m_iRenderTarget[2];
    CVector4 m_ColorScale;

    // temp
    int m_iDoubleRenderTarget[4];
    int m_iDownSampleRenderTarget[4];
};

class CRenderDepthMapContext
{
public:
    GELIB_API CRenderDepthMapContext();
    GELIB_API virtual ~CRenderDepthMapContext();

    GELIB_API void Allocate();
    GELIB_API void Free();

    // input parameters
    int m_iWidth;
    int m_iHeight;
	float m_fDefaultDepth;
    CMatrix m_CameraMatrix;
    CMatrix m_ProjMatrix;
	bool m_bIgnoreNoShadowObject;

    // output
    int m_iRenderTarget;
};

class CRenderer
{
public:

    GELIB_API CRenderer();
    GELIB_API virtual ~CRenderer();

    GELIB_API void Initialize();
    GELIB_API void Uninitialize();

    // single thread render function
    GELIB_API void Render(const CRenderView *view, CVisibility *visibility, int bufferIndex=0);

    // multi-threads render function (BuildRenderBuffer() in main thread, DrawRenderBuffer() in render thread)
    GELIB_API void BuildRenderBuffer(const CRenderView *view, CVisibility *visibility, int bufferIndex=0);
    GELIB_API void SwapRenderBuffer();     // swap all render buffers
    GELIB_API void DrawRenderBuffer();     // draw all render buffers
    GELIB_API void DrawRenderBuffer(int bufferIndex);
    GELIB_API void InvalidateRenderBuffer();

    // lightmap
    GELIB_API void RenderLightMap(CRenderLightMapContext *rlmc, CVisibility *visibility, int bufferIndex=0);

    // depth map
    GELIB_API void RenderDepthMap(CRenderDepthMapContext *rdmc, CVisibility *visibility, int bufferIndex=0);

    // enable / disable renderer
    GELIB_API void Enable(bool enable);
    GELIB_API bool IsEnabled();

	// enable low end render, no post process, no gamma correction, very simple light
    GELIB_API void EnableLowEndRender(bool enable);	 // warning!, change will reallocate render target

    GELIB_API void EnableVarianceShadow(bool enable);	 // warning!, change will reallocate render target
    GELIB_API void EnableDeferredLighting(bool enable); // warning!, change will reallocate render target

    // format: 0:A8R8G8B8(SRGB), 1:A16B16G16R16F, 2:A2B10G10R10, 3:A32B32G32R32F,  4:A8R8G8B8
    GELIB_API void SetRenderTargetFormat(int fmt);		 // warning!, change will reallocate render target
    GELIB_API int  GetRenderTargetFormat();
    GELIB_API bool IsRenderTargetFormatSupported(int fmt);

    // multi sample: 0: none, 1: 2 samples, 2: 4 samples, 3: 8 samples, 4: 16 samples
    GELIB_API void SetMultiSampleType(int mst);		 // warning!, change will reallocate render target
    GELIB_API int  GetMultiSampleType();
    GELIB_API bool IsMultiSampleTypeSupported(int mst, int fmt);

	// post process anti-aliasing
    GELIB_API void SetPostProcessAntiAliasing(int a);		// 0: off, 1: FXAA low, 2: FXAA high
    GELIB_API int GetPostProcessAntiAliasing();

    // environment settings
    GELIB_API void SetEnvironmentSettings(const CRendererEnvironmentSettings &env);
    GELIB_API const CRendererEnvironmentSettings& GetEnvironmentSettings();

    GELIB_API void SetGamma(float gamma);  // default 2.2

    GELIB_API void EnableColorGrading(bool enable);
    GELIB_API void SetColorGrading(const CColorGrading &ColorGrading);

    GELIB_API void SetExposure(float middleGray);
    GELIB_API void SetSaturation(float saturation);

    GELIB_API void SetAmbientColor(const CVector3 &color1, const CVector3 &color2);
    GELIB_API void SetAOLightMapColor(const CVector3 &color1, const CVector3 &color2, const CVector3 &shadowColor);

    GELIB_API void EnableFog(bool enable);
    GELIB_API void SetFogColor(const CVector3 &color1, const CVector3 &color2);
    GELIB_API void SetFogDensity(float density1, float density2);
    GELIB_API void SetFogAlpha(float a1, float a2);
    GELIB_API void SetFogDistance(float start, float end);
    GELIB_API void SetFogDirection(const CVector3 &dir);

    GELIB_API void EnableBloom(bool enable);
    GELIB_API void SetBloomThreshold(float threshold);
    GELIB_API void SetBloomIntensity(float intensity);
    GELIB_API void SetBloomBlurRadius(int radius);
    GELIB_API void SetBloomBlurDeviation(float deviation);

    GELIB_API void EnableLightShaft(bool enable);
    GELIB_API void SetLightShaftColor(const CVector3 &color);
    GELIB_API void SetLightShaftDirection(const CVector3 &dir);
    GELIB_API void SetLightShaftIntensity(float intensity);
    GELIB_API void SetLightShaftBackgroundIntensity(float intensity);
    GELIB_API void SetLightShaftThreshold(float threshold);
    GELIB_API void SetLightShaftRange(float s);
    GELIB_API void SetLightShaftBlurLength(float l);

    GELIB_API void EnableToneMapping(bool enable);

    GELIB_API void EnableDof(bool enable);
    GELIB_API void EnableDofAntiLeak(bool enable);
    GELIB_API void SetDofBlurRadius(int radius);
    GELIB_API void SetDofBlurDeviation(float deviation);
    GELIB_API void SetDofDistance(float focalPlane, float nearBlurPlane, float farBlurPlane);

    GELIB_API void EnableGlow(bool enable);
    GELIB_API void EnableEdgeGlow(bool enable);
    GELIB_API void SetEdgeGlowIntensity(float intensity);

    GELIB_API void EnableSsao(bool enable); // mode = 0: SSAO, mode = 1: SSAO_Only_characater
    GELIB_API void SetSsaoBlurRadius(int radius);
    GELIB_API void SetSsaoBlurDeviation(float deviation);
    GELIB_API void SetSsaoRadius(float r);
    GELIB_API void SetSsaoThreshold(float t);
    GELIB_API void SetSsaoAttenuation(float a);

    GELIB_API void EnableDistortion(bool enable);
    GELIB_API void EnableVolume(bool enable);

    GELIB_API void EnableClearBackground(bool clear);
    GELIB_API void SetBackgroundColor(const CVector3 &color);

    GELIB_API void SetSkyDistance(float d);

    GELIB_API void SetMotionBlurMode(int mode);     // 0: no motion blur, 1: camera motion blur, 2: object motion blur, 3: both motion blur
    GELIB_API void SetMotionBlurQuality(int quality); // 0: default, 1: high

	// mixed resolution render
    GELIB_API void EnableLowResolutionTranslucent(bool enable);
    GELIB_API void EnableUpSamplingFilter(bool enable);
    GELIB_API void SetUpSamplingFilterThreshold(float threshold);

    GELIB_API void SetTextureFilterQuality(int quality); // 0: low, 1: default, 2: high
    GELIB_API void SetMaxAnisotropy(int anisotropy); // 0: 4, 1: 8, 2: 16

    GELIB_API void EnableLightMap(bool enable);
    GELIB_API void EnableLightMaterial(bool enable);
    GELIB_API void EnableLightOcclusion(bool enable);	// enable CRenderObject::ComputeLightOcclusion() callback
    GELIB_API void EnableAllVertexLight(bool enable);
    GELIB_API void EnableEnvironmentLight(bool enable);

    // shadow
    GELIB_API void EnableShadow(bool enable);
    GELIB_API void EnableSmoothShadow(bool enable);
    GELIB_API void SetShadowLimit(float limit);	// used by PSSM

    // material animation
    GELIB_API void SetMaterialGlobalVector(int index, const CVector4 &v);
    GELIB_API void SetAnimationTime(float time);

    // statistic, debug
    GELIB_API const CRendererStatistic* GetStatistic();
    GELIB_API void ShowStatistic(int show);
    GELIB_API void ShowRenderTarget(int show);
    GELIB_API void ShowDepth(float maxDepth); // 0: off
	GELIB_API void ShowWireframe(int show); // 1: z on, 2: z off, 4: wireframe only z on, 6: wireframe only z off
    GELIB_API void ShowOverdraw(int show);  // 1: pixel z on, 2: pixel z off, 4: object z on, 6: object z off
    GELIB_API void ShowTextureResolution(int show);  // 1: on
    GELIB_API void ShowShadowFrustum(int show); // 1: z on, 2: z off
    GELIB_API void ReplaceMaterialTexture(int t); // 1: white, 2: black, 3: gray, 4: mip
    GELIB_API void EnableLighting(bool enable);
    GELIB_API void EnableDepthPass(bool enable);

    // test
    GELIB_API void EnableOcclusionQuery(bool enable);
    GELIB_API void EnableOcclusionQueryWait(bool enable);
    GELIB_API void EnableOcclusionQueryFlush(bool enable);
    GELIB_API void EnableLightVolumeStencil(bool enable);
    GELIB_API void EnableReplaceStencil(bool enable);
    GELIB_API void EnableGammaCorrection(bool enable); // warning!, change will reallocate render target

    // used by CRenderObject
    GELIB_API void DrawMesh(CRenderMesh *mesh, int meshChunk, void *meshShaderData, CRenderMaterial *material, CLightMapInfo *lightMap);
    GELIB_API CRendererContext* GetContext();
    GELIB_API CRendererContext* GetContext(int renderBufferIndex);
	GELIB_API bool IsBuildingShadowBatch() { return m_eBuildBatchType == BUILD_BATCHES_SHADOW; }

    // used by CImageProcessor
    GELIB_API enum
    {
        CHANGERT_RESTORE = 1<<0,
        CHANGERT_RESTORE_RECT = 1<<1,
        CHANGERT_CLEAR_COLOR = 1<<2,
        CHANGERT_CLEAR_Z = 1<<3,
        CHANGERT_CLEAR_STENCIL = 1<<4,
        CHANGERT_CLEAR_RECT = 1<<5,
        CHANGERT_CLEAR_COLOR_WHITE = 1<<6,
        CHANGERT_CLEAR_COLOR_BACKGROUND = 1<<7,
		CHANGERT_CLEAR_FULL = 1 << 9,
        CHANGERT_TILING_RESTORE_SCENE = 1 << 8,
    };
    GELIB_API bool ChangeRenderTarget(int rt, int flag=0);
    GELIB_API void DrawQuad(int srcRT=0);
    GELIB_API void DrawQuad(const CVector2 offsets[], int numOffsets, int srcRT=0);
    GELIB_API void DrawQuad(float x1, float y1, float x2, float y2, float tx1, float ty1, float tx2, float ty2, float z=1.0f, int srcRT=0);
    GELIB_API void DrawQuad(float x1, float y1, float x2, float y2, float tx1, float ty1, float tx2, float ty2, const CVector2 offsets[], int numOffsets, float z=1.0f, int srcRT=0);

    // generate renderer shaders (tool)
    GELIB_API void GenerateShaders();

private:

    bool CheckBackBufferSize();
    void AllocateRenderTargets();
    void FreeRenderTargets();
	void CheckAllocateRenderTargets();
    void VerifyEnvironmentRequirements(CRendererEnvironmentSettings &env);

    // flag is same as ChangeRenderTarget()
    void ClearCurrentRenderTarget(int flag);
    void ResolveCurrentRenderTarget(int idx, int id, int flag = 0);
    void RestoreCurrentRenderTarget(int flag);

// modified by bob -- start
    class CWaterLayer
    {
    public:
        CRenderBatchList m_WaterBatches;
        WaterInfoList m_WaterInfos;
    };
    typedef std::vector<CWaterLayer> CWaterLayerList;
// modified by bob -- end

    class CLightingLayer
    {
    public:
        CVisibleLight *m_pVLight;
        CRenderBatchList m_LightingBatches;
        CRenderBatchList m_AmbientEmissiveBatches;
        CRenderBatchList m_LightMapShadowProjectionBatches;
        ShadowInfoList m_ShadowInfos;
    };
	typedef std::vector<CLightingLayer> LightingLayerList;

    class CProjectorLayer
    {
    public:
        CVisibleLight *m_pVLight;
        CRenderBatchList m_ProjectorBatches[2];
    };
	typedef std::vector<CProjectorLayer> ProjectorLayerList;

    class CTranslucentLayer
    {
    public:
        CVisibleObject *m_pVObject;
        CRenderBatchList m_DepthBatches;
        CRenderBatchList m_VertexLightBatches;

        class CTranslucentLightingLayer
        {
        public:
            CVisibleLight *m_pVLight;
            CRenderBatch *m_pLightingBatch;
        };
        typedef std::vector<CTranslucentLightingLayer> TranslucentLightingLayerList;

        class CRenderMaterialBatch
        {
        public:
			CRenderMaterial *m_pMaterial;
            CRenderBatch *m_pAmbientEmissiveBatch;
			CVisibleLight *m_pAmbientDirectionalLight;
            TranslucentLightingLayerList m_LightingLayers;
        };
        typedef std::vector<CRenderMaterialBatch> CRenderMaterialBatchList;

        CRenderMaterialBatchList m_MaterialBatches;
    };
	typedef std::vector<CTranslucentLayer> CTranslucentLayerList;

    class CVolumeLayer
    {
    public:
        CRenderMaterial *m_pMaterial;
        CMeshShader *m_pShader;
        CRenderBatchList m_DepthBatches;
    };
	typedef std::vector<CVolumeLayer> CVolumeLayerList;

    class CPlaneDecalLayer
    {
    public:
        EBlendMode m_eBlend;
        CVisibleObject *m_pVObject;
        CRenderBatchList m_AmbientEmissiveBatches;
        LightingLayerList m_LightingLayers;
    };
	typedef std::vector<CPlaneDecalLayer> CPlaneDecalLayerList;

    class CVolumeDecalLayer
    {
    public:
        CVisibleObject *m_pVObject;
        CRenderBatchList m_DepthBatches;
        CRenderBatchList m_AmbientEmissiveBatches[2];
        LightingLayerList m_LightingLayers[2];
    };
	typedef std::vector<CVolumeDecalLayer> CVolumeDecalLayerList;

    CLightingLayer* GetLightingLayer(CVisibleLight *vlt);
    CLightingLayer* GetLightingLayer(LightingLayerList &layers, CVisibleLight *vlt);
    CProjectorLayer* GetProjectorLayer(CVisibleLight *vlt);
    CTranslucentLayer* GetTranslucentLayer(int layerType, CVisibleObject *vobj);
    CVolumeLayer* GetVolumeLayer(CRenderMaterial *material);
    CPlaneDecalLayer* GetPlaneDecalLayer(CVisibleObject *vobj);
    CVolumeDecalLayer* GetVolumeDecalLayer(CVisibleObject *vobj);
// modified by bob -- start
    CWaterLayer* GetWaterLayer();
// modified by bob -- end

    bool m_bEnableRenderer;

    enum EBuildBatchType
    {
        BUILD_BATCHES_NONE = 0,
        BUILD_BATCHES_NORMAL,
        BUILD_BATCHES_VERTEX_LIGHT,
        BUILD_BATCHES_SHADOW,
        BUILD_BATCHES_OVERDRAW,
        BUILD_BATCHES_TEXTURE_RESOLUTION,
        BUILD_BATCHES_LIGHTMAP,
        BUILD_BATCHES_DEPTHMAP
    };
    EBuildBatchType m_eBuildBatchType;

    enum
    {
        MERGE_NONE = 0,
        MERGE_NO_ALPHA_TEST = 1,
        MERGE_ANY_ALPHA_TEST = 2
    };

    void SortObjects();
    void BuildBatches(EBuildBatchType batchType=BUILD_BATCHES_NORMAL);
    void AppendWireframeBatch(CRenderMesh *mesh, int meshChunk, void *meshShaderData, CRenderMaterial *material, const CVector4 &color);
    void AppendOverdrawBatch(CRenderMesh *mesh, int meshChunk, void *meshShaderData, CRenderMaterial *material, EBlendMode blend, ELightMapMode lightMapMode);
    void AppendTextureResolutionBatch(CRenderMesh *mesh, int meshChunk, void *meshShaderData, CRenderMaterial *material, EBlendMode blend, ELightMapMode lightMapMode);
    CRenderBatch* AppendBatch(CRenderBatchList &batches, EShaderFunction shaderFunc, CVisibleObject *vobj, CRenderMesh *mesh, int meshChunk, void *meshShaderData, CRenderMaterial *material, CLightMapInfo *lightMap, int color, int queryIndex, int poolIndex, int merge=MERGE_NONE);
    CRenderBatch* AppendBatch(EShaderFunction shaderFunc, CVisibleObject *vobj, CRenderMesh *mesh, int meshChunk, void *meshShaderData, CRenderMaterial *material, CLightMapInfo *lightMap, int color, int queryIndex, int poolIndex);
    void SortBatches(bool depthBatches=true, bool otherBatches=true);

    enum
    {
        TWO_SIDED_TWO_PASS = 0,
        TWO_SIDED_ONE_PASS = 1,
        FORCE_TWO_SIDED_ONE_PASS = 2,
    };

    void DrawBatches(CRenderBatchList &batches, int twoSidedPass=TWO_SIDED_TWO_PASS);
    void DrawBatch(CRenderBatch *batch, int twoSidedPass=TWO_SIDED_TWO_PASS);

    void UpdateRenderData();

    void FillColorGradingTexture();
    void ClearBackground();
    void RenderDepth();
    void CopyLowResolutionDepth();
    void PrepareAntiAliasing();
    void RenderSsao();
    void RenderFirstShadow();
    void RenderSky();
    void RenderAmbientEmissive();
    void RenderLighting();
    void RenderVertexLight();
    void RenderPlaneDecal();
    void RenderVolumeDecal();
    void RenderProjector(int projType);
    void RenderFog();
    void RenderDistortion(int layerType);
// modified by bob -- start
    void RenderWater();
    void RenderPerlinMap(int iHeightMap, const PerlinNoiseDesc* pDesc);
    void RenderWaterNormal(int iHeightMap,int iNormalMap, CVector4& params);
// modified by bob -- end
    void RenderTranslucent(int layerType);
    void ApplyTranslucent(int layerType);
    void RenderTranslucentDepth();
    void RenderVolume(EVolumeMode volumeMode);
    void RenderEdgeGlow();
    void RenderGlow();
    void RenderVelocity();
    void RenderPostProcess();
    void RenderWireframe();
    void RenderOverdraw();
    void RenderTextureResolution();

    void RenderGBuffer();
    void AccumulateDeferredLighting();
    void RenderDeferredLighting();

    bool RenderShadow(CLightingLayer *layer);
    void RenderPerObjectShadowMap(CLightingLayer *layer);
    void BuildPerObjectShadowMapInfos(CLightingLayer *layer);
    void RenderParallelSplitShadowMap(CLightingLayer *layer);
    void RenderCascadeShadowMap(CLightingLayer *layer);
    void BuildVarianceShadowMap(CLightingLayer *layer, EShadowMapMethod method, int index, bool blur);
    void BlurShadowProjection();

    void RenderLightMap(CRenderLightMapContext *rlmc);
    void RenderLightMapShadow(CRenderLightMapContext *rlmc, CLightingLayer *layer);
    void RenderDepthMap(CRenderDepthMapContext *rdmc);

    CPostProcessShader* GetPostProcessShader(bool dof, bool colorGrading, bool tone, bool fxaa);

    void UseScissor(int x, int y, int w, int h);
    void UseScissor(CRenderBatchList &batches, int outOffsetX=0, int outOffsetY=0);
    void UnuseScissor();
    void DrawScissor();
    void UseVolumeStencil(const CVector3 corners[8]);
    void ReplaceVolumeStencil(bool always=false);
    void UseLightVolumeStencil(CVisibleLight *vlt);
    void UnuseVolumeStencil();

    void ClearTextureBinding();
    void SetDefaultRenderState();
    void ApplyMaxAnisotropy();

    void DrawShadowFrustum();
    void DrawStatistic();
	void DrawInternalRenderTarget();

    void ComputeFog(const CVector3 &p, CVector4 &fogColor);
    void ComputeObjectFog();
    void DisableObjectFog();

    int  AllocateOcclusionQuery();
    void FreeOcclusionQuery();

    void InitializeSharedData();
    void UninitializeSharedData();

    static int CreateOnePixelTexture(int r, int g, int b, int a, bool gammaCorrection);
    static int CreateOnePixelDepthTexture(float depth);
    static int CreateOnePixelCubeTexture(int r, int g, int b, int a);
    static int CreateMipColorTexture(int size);
    static int CreateTextureResolutionMipTexture();

	class PTVertex
	{
	public:
		float x, y, z, tx, ty;
	};

	class PT8Vertex
	{
	public:
		CVector3 p;
		CVector4 t[8];
	};

    class CSharedData
    {
    public:
        CSharedData();

        void InitializeShaders();
        void ClearShaders();

        int m_iRefCounter;

        int  m_iMainRenderTargetFormat;
        int  m_iMultiSampleType;
        bool m_bGammaRenderTarget;
		bool m_bEnableGammaCorrection;
        bool m_bReallocateRenderTargets;
		bool m_bEnableLowEndRender;
		bool m_bEnableVarianceShadow;
		bool m_bEnableDeferredLighting;
        unsigned int m_iDeviceResetCount;

        int m_iTilingTarget;
        int m_iSceneLightTarget;
        int m_iSceneColorTarget;
        int m_iSceneColorTarget2;
        int m_iSceneColorTarget3;
        int m_iHalfSceneColorTarget;
        int m_iHalfSceneColorTarget2;
        int m_iQuarterSceneColorTarget;
        int m_iQuarterSceneColorTarget2;
        int m_iQuarterSceneColorTarget3;
        int m_iQuarterSceneColorTarget4;
        int m_iEighthSceneColorTarget;
        int m_iEighthSceneColorTarget2;

        int m_iSceneDepthTarget;
        int m_iHalfSceneDepthTarget;
        int m_iQuarterSceneDepthTarget;
        int m_iShadowDepthTarget;
        int m_iVarianceShadowDepthTarget;
        int m_iVolumeDepthTarget;
        int m_iVelocityTarget;

        int m_iWhiteTexture;
        int m_iBlackTexture;
        int m_iZeroTexture;
        int m_iWhiteCubeTexture;
        int m_iBlackCubeTexture;
        int m_iGrayTexture;
        int m_iMipColorTexture;
        int m_iTextureResolutionMipTexture;
        int m_iFarDepthTexture;
        int m_iColorGradingTexture;
        int m_iRandomRotationTexture;
        int m_iDitherTexture;

        int m_iBackBufferWidth;
        int m_iBackBufferHeight;
        int m_iShadowDepthSize;

        int m_iPVertexDecl;
        int m_iPTVertexDecl;
        int m_iPT8VertexDecl;

// modified by bob -- start
        // Water
        CPerlinNoiseMaker *m_pPerlinMaker;
// modified by bob -- end

        CImagePackLayout *m_pTextureLayout;
        CImageProcessor *m_pImageProcessor;
        CTextureFont *m_pFont;
        CRenderMaterial *m_pDeferredLightDefaultMaterial;

        CPrimitiveShader *m_pPrimitiveShader;
        CShadowProjectionShader *m_pShadowProjectionShader;
        CPSSMProjectionShader *m_pPSSMProjectionShader;
        CPSSMProjectionHighQualityShader *m_pPSSMProjectionHighQualityShader;
        CPSSMVarianceProjectionShader *m_pPSSMVarianceProjectionShader;
        CVarianceShadowProjectionShader *m_pVarianceShadowProjectionShader;
        CBlurShadowProjectionShader *m_pBlurShadowProjectionShader;
        CBlurVarianceShadowDepthShader *m_pBlurVarianceShadowDepthShader;
        CCopyVarianceShadowDepthShader *m_pCopyVarianceShadowDepthShader;
        CVolumeIntersectionShader *m_pVolumeIntersectionShader;
// modified by bob -- start
        CSeaWaveShader *m_pSeaWaveShader;
// modified by bob -- end

        CFogShader *m_pFogShader;
        CCameraVelocityShader *m_pCameraVelocityShader;
        CMotionBlurShader *m_pMotionBlurShader;
        CMotionBlurHighQualityShader *m_pMotionBlurHighQualityShader;
        CDistortionApplyShader *m_pDistortionApplyShader;
        CSsaoShader *m_pSsaoShader;
        CBuildLightShaftShader *m_pBuildLightShaftShader;
        CBlurLightShaftShader *m_pBlurLightShaftShader;
        CFxaaShader *m_pFxaaShader;
        CFxaaHighQualityShader *m_pFxaaHighQualityShader;

        CPostProcessShader *m_pPostProcessShader;
        CPostProcessDofShader *m_pPostProcessDofShader;
        CPostProcessColorGradingShader *m_pPostProcessColorGradingShader;
        CPostProcessToneShader *m_pPostProcessToneShader;
        CPostProcessLumShader *m_pPostProcessLumShader;
        CPostProcessDofColorGradingShader *m_pPostProcessDofColorGradingShader;
        CPostProcessDofToneShader *m_pPostProcessDofToneShader;
        CPostProcessDofLumShader *m_pPostProcessDofLumShader;
        CPostProcessColorGradingToneShader *m_pPostProcessColorGradingToneShader;
        CPostProcessColorGradingLumShader *m_pPostProcessColorGradingLumShader;
        CPostProcessDofColorGradingToneShader *m_pPostProcessDofColorGradingToneShader;
        CPostProcessDofColorGradingLumShader *m_pPostProcessDofColorGradingLumShader;
        CPostProcessDofColorGradingToneLumShader *m_pPostProcessDofColorGradingToneLumShader;
        CPostProcessToneLumShader *m_pPostProcessToneLumShader;
        CPostProcessDofToneLumShader *m_pPostProcessDofToneLumShader;
        CPostProcessColorGradingToneLumShader *m_pPostProcessColorGradingToneLumShader;
        std::vector<CShader*> m_NotCachedShaders;
    };

    static CSharedData s_SharedData;

    enum
    {
        TRANSLUCENT_LAYER_PRE_DISTORTION = 0,
        TRANSLUCENT_LAYER_POST_DISTORTION,
        TRANSLUCENT_LAYER_FOREGROUND,
        TRANSLUCENT_LAYER_HALF_RESOLUTION,
        TRANSLUCENT_LAYER_QUARTER_RESOLUTION,
        TRANSLUCENT_LAYER_NUMBER        // see CVisibleObject::m_iLayers
    };

    class CRenderBuffer
    {
    public:
        CRenderBuffer();
        ~CRenderBuffer();

        void ResetAllocators();
        void ResetBatches();

        CRendererContext m_Context;
		CVisibility *m_pVisibility;
        CVisibleSet m_VisibleSet;

        CRenderBatchList m_DepthBatches;
        CRenderBatchList m_TranslucentDepthBatches;
        CRenderBatchList m_GBufferBatches;
        CRenderBatchList m_VelocityBatches;
        CRenderBatchList m_AmbientEmissiveBatches;
        CRenderBatchList m_DeferredLightBatches;
        CRenderBatchList m_SkyBatches[2];
        CRenderBatchList m_VertexLightDepthBatches;
        CRenderBatchList m_VertexLightOpaqueBatches;
        CRenderBatchList m_VertexLightTranslucentBatches[3];
        LightingLayerList m_LightingLayers;
        ProjectorLayerList m_ProjectorLayers;
        CTranslucentLayerList m_TranslucentLayers[TRANSLUCENT_LAYER_NUMBER];
		float m_LowResolutionMaxZ[2];

        CVolumeLayerList m_VolumeLayers;
        CPlaneDecalLayerList m_PlaneDecalLayers;
        CVolumeDecalLayerList m_VolumeDecalLayers;
        CRenderBatchList m_DistortionBatches[2];
        CRenderBatchList m_DistortionAccumulateBatches[2];
// modified by bob -- start
        CWaterLayerList  m_WaterLayers;
// modified by bob -- end

        CRenderBatchList m_GlowBatches;
        CRenderBatchList m_EdgeGlowBatches;
        CRenderBatchList m_SkyGlowBatches;
        CRenderBatchList m_WireframeBatches;
        CVisibleObjectList m_ObjectFogs;

        CChunkObjectAllocator<CRenderBatch> m_BatchAllocator[3];
        CChunkObjectAllocator<CVisibleObject> m_VisibleObjectAllocator;
        CChunkObjectAllocator<CVisibleLight> m_VisibleLightAllocator;
        CChunkObjectAllocator<CShadowInfo> m_ShadowInfoAllocator;
// modified by bob -- start
        CChunkObjectAllocator<CWaterInfo> m_WaterInfoAllocator;
// modified by bob -- end

        std::vector<CRenderObject*> m_RenderDataDirtyObjects;
        CBufferedPlotter3D *m_pPlotter;

	    // statistic
        CRendererStatistic m_Statistic;
		CVisStatistic m_VisStatistic;
		double m_fVisTotalTime;
		double m_fBuildTotalTime;
		double m_fSwapTotalTime;
		double m_fDrawTotalTime;
		int m_iVisCounter;
		int m_iBuildCounter;
		int m_iSwapCounter;
		int m_iDrawCounter;

        bool m_bValidForDrawing;
    };

    void SetNumRenderBuffers(int n);
    void DrawRenderBuffer(CRenderBuffer *drawRB);

    typedef std::vector<CRenderBuffer*> CRenderBufferList;
    CRenderBufferList m_BuildBufferList;
    CRenderBufferList m_DrawBufferList;
    CRenderBuffer *m_pBuildRB;
    CRenderBuffer *m_pDrawRB;

    int m_iCurTarget;
    int m_iCurTargetWidth;
    int m_iCurTargetHeight;
    int m_iChangeTargetCount;

    bool m_bSeparateSceneColor;
    bool m_bSeparateSceneDepth;

    bool m_bSsaoRendered;
    bool m_bVelocityRendered;
    CVisibleLight *m_pLastShadowedLight;

    int m_iScissorX;
    int m_iScissorY;
    int m_iScissorW;
    int m_iScissorH;
    bool m_bUsingScissor;
    bool m_bUsingVolumeStencil;
	bool m_bIgnoreNoShadowObject;

    bool m_bEnableLightVolumeStencil;
    bool m_bEnableReplaceStencil;

    CRendererEnvironmentSettings m_EnvSet;
    CRenderStateSet m_RenderStates;
    CPlotter2D *m_pCanvas;
    CPlotter3D *m_pPlotter;

    class COcclusionQuery
    {
    public:
        enum
        {
            NOT_QUERIED = -1,
            WAIT_QUERY_RESULT = -2,
            MAX_OCCLUSION_QUERIES = 512
        };

        COcclusionQuery() { m_iId = 0; m_iVisiblePixels = NOT_QUERIED; }
        int m_iId;
        int m_iVisiblePixels;
    };

    std::vector<COcclusionQuery> m_OcclusionQueries;
    int m_iFreeOcclusionQueryIndex;
    bool m_bEnableOcclusionQuery;
    bool m_bEnableOcclusionQueryWait;
    bool m_bEnableOcclusionQueryFlush;
};

extern const unsigned short g_BoxIndices[];


}//GELib

#endif//__GELIB_RENDERER_RENDERER_H__
