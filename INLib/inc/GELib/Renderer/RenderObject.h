//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/RenderObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_RENDEROBJECT_H__
#define __GELIB_RENDERER_RENDEROBJECT_H__

#include <GELib/GeConfig.h>
#include "RendererTypes.h"
#include <GELib/Math/GeMath.h>
#include <GELib/Math/PolyMesh.h>
#include "ShadowMap.h"


namespace GELib
{

class CRenderer;
class CRenderMaterial;
class CRenderLight;
// modified by bob -- start
class CWaterInfo;
// modified by bob -- end

class CRenderObject
{
public:
    enum EOutdoorIndoorType
    {
        DOOR_DEFAULT = 0,
        DOOR_DONTCARE = 1,
        DOOR_OUTDOOR_ONLY = 2,
    };

    GELIB_API CRenderObject();
    GELIB_API virtual ~CRenderObject();
    GELIB_API virtual int GetClassId();

    GELIB_API virtual void Render(CRenderer *renderer);
    GELIB_API virtual void UpdateRenderData(CRenderer *renderer);
    GELIB_API virtual float ComputeLightOcclusion(CRenderLight *light);

// modified by bob -- start
    // water
    GELIB_API virtual bool GetWaterInfo(CWaterInfo *pWaterInfo = 0) { return false; }
    GELIB_API virtual bool IsSea() { return false; }
// modified by bob -- end

	bool IsSky() { return m_eDrawMeshMode == DRAW_MESH_SKY; }
	bool DontCareZone() { return m_eOutdoorIndoor == DOOR_DONTCARE; }
	bool IsStaticLighting() { return m_eLightMapMode == LIGHTMAP_FULL_LIGHT || m_eLightMapMode == LIGHTMAP_SIMPLE_LIGHT; }
	bool IsStaticShadow() { return m_eLightMapMode == LIGHTMAP_MONO_SHADOW || m_eLightMapMode == LIGHTMAP_COLOR_SHADOW; }
	bool HasLightMap() { return m_eLightMapMode != LIGHTMAP_NONE; }

    bool m_bShow;
    bool m_bCastDynamicShadow;
    bool m_bCastStaticShadow;       // lightmapper use
    bool m_bReceiveDynamicShadow;
    bool m_bReceiveStaticShadow;    // lightmapper use
    bool m_bInvisibleCastShadow;    // cast dynamic shadow even if object is culled
    bool m_bVertexLight;            // compute light per vertex
    bool m_bMotionBlur;             // per object motion blur
    bool m_bComputeLightOcclusion;  // if true, ComputeLightOcclusion() is called
    bool m_bRenderDataDirty;        // if true, UpdateRenderData() is called
    bool m_bEdgeGlow;				// render edge glow effect
    bool m_bUseEnvironmentLight;	// apply environment lights into ambient
	char m_iLodGroup;				// choose CRenderView::m_LodFactors
    short m_iMaxLights;             // if > 0, number of lights affect object is limited
    short m_iDrawOrder;             // alpha blend sort key, smaller number render first, default is 0
    short m_iSubDrawOrder;
	ELightMapMode m_eLightMapMode;
    EOutdoorIndoorType m_eOutdoorIndoor;
    EDrawMeshMode m_eDrawMeshMode;
    CAABox m_Box;             // bounding box in world space
    float m_fBoundingThreshold;     // default is 0, dynamic object can set to > 0 to reduce visibility update frequency
    CVector3 m_Color;
    CVector3 m_EdgeGlowColor;
    float m_fOpacity;
    float m_fShadowOpacity;
    float m_fShadowResolutionFactor;
    float m_fShadowDepthBias;
	float m_fShadowCastHeight;			// if != 0, compute shadow length by object size and height (faster), if == 0, shadow length is unlimited (slower)
    float m_fMaxVisibleDistance;		// if <= 0, don't cull by distance
    float m_fNestedLodDistance;			// if <= 0, don't use nested lod
    float m_fDynamicShadowDistance;		// if <= 0, don't cull shadow by distance
    float m_fAmbientIntensity;
    int m_iLightGroups;					// light is effective when (CRenderObject::m_iLightGroups & CRenderLight::m_iLightGroups) != 0
    int m_iCastShadowGroups;			// cast shadow when light is effective and (CRenderObject::m_iCastShadowGroups & CRenderLight::m_iLightGroups) != 0
};

class ShadowParameters
{
public:
    ShadowParameters();

    // dynamic shadow properties
    EShadowMapMethod m_eShadowMapMethod;
    float m_fShadowDepthBias;

    // for per-object
    bool m_bEnableVarianceShadow;
    bool m_bEnableSmoothShadow;
    float m_fShadowResolutionFactor;
    short m_iMinShadowResolution;
    short m_iMaxShadowResolution;

    // for pssm
    int m_iPSSMNumber;
    float m_fPSSMLambda;
    bool m_bPSSMAutoSplit;
    bool m_bShadowPCFHighQuality;
    float m_fPSSMManualSplitDepth[PSSM_MAX_NUMBER - 1];
    float m_fShadowPCFRadius;
    float m_fPSSMFadeBegin;
    float m_fPSSMFadeRange;
};

class CRenderLight
{
public:
    GELIB_API CRenderLight();
    GELIB_API virtual ~CRenderLight();
    GELIB_API virtual int GetClassId();

    GELIB_API virtual CRenderMaterial* GetMaterial();
    GELIB_API virtual void GetMaterialParameter(int index, CVector4 &m);

    GELIB_API void ComputeBoundingBox(CAABox &box);
    GELIB_API void ComputeProjMatrix(CMatrix &matrix);

    enum ELightType
    {
        POINT_LIGHT,
        SPOT_LIGHT,
        DIRECTIONAL_LIGHT
    };

    enum EProjectorBlendMode
    {
        PROJECTOR_BLEND_NONE,
        PROJECTOR_BLEND_ALPHA,
        PROJECTOR_BLEND_ADDITIVE,
        PROJECTOR_BLEND_MULTIPLY,
        PROJECTOR_BLEND_MULTIPLY_ADDITIVE
    };

    bool m_bShow;
    bool m_bCastDynamicShadow;
    bool m_bCastStaticShadow;           // lightmapper use
    bool m_bVertexLight;                // not implemented yet
    bool m_bPassPortal;
    bool m_bAmbientOcclusionLight;      // lightmapper use
	bool m_bEnvironmentLight;			// apply light to ambient of objects with CRenderObject::m_bUseEnvironmentLight == true
	char m_iLodGroup;					// choose CRenderView::m_LodFactors
    float m_fRange;
    float m_fInnerCone;
    float m_fOuterCone;
    float m_fIntensity;
	float m_fEnvironmentLightIntensity;
    float m_fProjectorOpacity;
    float m_fAreaLightSize;             // lightmapper use
    short m_iLightMapType;              // 0: no lightmap, 1: default
    short m_iProjectorLayer;            // if > 0, only project material on to mesh (no lighting, don't consider light group), smaller number draw first
                                        // if < 0, only project material on to mesh (no lighting, consider light group), larger number draw first
    ELightType m_eType;
    EProjectorBlendMode m_eProjectorBlendMode;
    CVector2 m_Size;					// directional light size
    CVector3 m_Color;
    CVector3 m_ShadowColor;				// dynamic and static shadow color
    CMatrix m_Matrix;					// position and rotation, no scaling
    int m_iLightGroups;                 // light is effective when (CRenderObject::m_iLightGroups & CRenderLight::m_iLightGroups) != 0
    float m_fBoundingThreshold;			// default is 0, dynamic object can set to > 0 to reduce visibility update frequency
    float m_fMaxVisibleDistance;		// if <= 0, don't cull by distance
    ShadowParameters m_ShadowParameters;
};

class CRenderOccluder
{
public:
    GELIB_API CRenderOccluder();
    GELIB_API virtual ~CRenderOccluder();

    GELIB_API void ComputeBoundingBox(CAABox &box);

    bool m_bOn;

    CMatrix m_Matrix;
    CPolyMesh m_PolyMesh;
};


class CRenderPortal
{
public:
    GELIB_API CRenderPortal();
    GELIB_API virtual ~CRenderPortal();

    GELIB_API void ComputeBoundingBox(CAABox &box);

    bool m_bOn;
    bool m_bPassLight;

    CMatrix m_Matrix;
    CPolyMesh m_PolyMesh;
};

class CRenderZone
{
public:
    GELIB_API CRenderZone();
    GELIB_API virtual ~CRenderZone();

    GELIB_API void ComputeBoundingBox(CAABox &box);

    CVector3 m_AmbientColor[2];
    CVector3 m_LightMapAmbientColor[2];
    CVector3 m_AOShadowColor;

    CMatrix m_Matrix;
    CPolyMesh m_PolyMesh;
};



}//GELib

#endif//__GELIB_RENDERER_RENDEROBJECT_H__
