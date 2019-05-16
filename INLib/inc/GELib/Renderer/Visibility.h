//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/Visibility.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_VISIBILITY_H__
#define __GELIB_RENDERER_VISIBILITY_H__

#include <vector>
#include <GELib/Renderer/RenderObject.h>
#include <GELib/Math/GeMath.h>
#include <GELib/Math/Frustum.h>

namespace GELib
{


class IPlotter3D;
class IPlotter2D;

class CVisibility;
class CVisibleLight;
class CRenderMaterial;
class CMeshShader;
class CRendererContext;

class CVisibleLight;

class CVisibleObject
{
public:
    CVisibleObject();
    virtual ~CVisibleObject();

    void ProjectToScreen(const CMatrix &viewProjMatrix, int sw, int sh);
	void ComputeSortKey();
	void SortEffectiveLights(int maxLight);
	void ApplyEffectiveLightsToAmbient(CVector3 ambientColor[2], CVector3 &ambientDirection);
	bool IsSky() { return m_eDrawMeshMode == DRAW_MESH_SKY; }

    CRenderObject *m_pRenderObject;

    // copied from m_pRenderObject
	EDrawMeshMode m_eDrawMeshMode;
    CAABox m_Box;
    CVector3 m_Color;
    float m_fOpacity;
    bool m_bComputeLightOcclusion;
    bool m_bReceiveDynamicShadow;

    // computed by visibility
    class CEffectiveLight
    {
    public:
        CEffectiveLight(CVisibleLight *light=0, float occlusion=1.0f) { m_pVisibleLight=light; m_fOcclusion=occlusion; m_fEffectiveness = 1.0f; }
        void ComputeEffectiveness(CVisibleObject *vobj);

        CVisibleLight *m_pVisibleLight;
        float m_fOcclusion;
        float m_fEffectiveness;
		float m_fLuminance;
		CVector3 m_LightVector;
    };
    typedef std::vector<CEffectiveLight> CEffectiveLightList;

    CEffectiveLightList m_EffectiveLights;
    CEffectiveLightList m_EffectiveProjectors;
    CEffectiveLightList m_EnvironmentLights;
    CVector3 m_AmbientColor[2];      // pre multiplied with object color and intensity
	CVector3 m_AmbientDirection;
    CVector3 m_AOShadowColor;
    float m_fViewZDistance;
    float m_fDeferredLightOcclusion;
    bool m_bOnlyShadowVisible;				// obj is invisible, but shadow is visible
	bool m_bUseEnvironmentLight;

    // temp variable used by visibility
    void *m_pVisObject;

    // temp variables used by renderer
    short m_iScreenX;
    short m_iScreenY;
    short m_iScreenW;
    short m_iScreenH;
    short m_iLayers[7];     // TRANSLUCENT_LAYER_NUMBER
    short m_iAmbientLayer;
    short m_iNumVisibleBatches;
    bool m_bTranslucent;
    bool m_bGlow;
    bool m_bObjectFog;
    int m_iSortKey;
    CVector4 m_FogColor;
};

class CVisibleLight
{
public:
    CVisibleLight();
    virtual ~CVisibleLight();

    void ProjectToScreen(const CMatrix &cameraMatrix, const CMatrix &viewProjMatrix, int sw, int sh);
	void ComputeSortKey();
    void Draw(IPlotter3D &plotter);
    bool IsInfiniteDirectional() { return m_eType == CRenderLight::DIRECTIONAL_LIGHT && m_fRange <= 0; }
    bool IsRangedDirectional() { return m_eType == CRenderLight::DIRECTIONAL_LIGHT && m_fRange > 0; }
	bool IsProjectorWithLightGroup() { return m_iProjectorLayer < 0; }
	bool IsProjectorWithoutLightGroup() { return m_iProjectorLayer > 0; }

    CRenderLight *m_pRenderLight;

    // copied from m_pRenderLight
    CRenderLight::ELightType m_eType;
    bool m_bCastDynamicShadow;
    float m_fRange;
    float m_fInnerCone;
    float m_fOuterCone;
    short m_iLightMapType;
    short m_iProjectorLayer;
    float m_fProjectorOpacity;
    CRenderLight::EProjectorBlendMode m_eProjectorBlendMode;
    CVector2 m_Size;
    CVector3 m_Color;     // pre multiplied by intensity
    CVector3 m_ShadowColor;
	float m_fEnvironmentLightIntensity;
    CMatrix m_Matrix;
    ShadowParameters m_ShadowParameters;
    CRenderMaterial *m_pMaterial;
	CVector4 m_MaterialParameters[2];

    // computed by visibility
    float m_fCosInnerCone;
    float m_fCosOuterCone;
    CMatrix m_ViewMatrix;
    CMatrix m_ProjMatrix;
    CMatrix m_ViewProjMatrix;
    CFrustum m_Frustum;
    short m_iScreenX;
    short m_iScreenY;
    short m_iScreenW;
    short m_iScreenH;
    short m_iNumLightingObjects;
    std::vector<CVisibleObject*> m_CastShadowObjects;

    // temp variable used by visibility
    void *m_pVisLight;

    // temp variables used by renderer
    CMeshShader *m_pMaterialShader;
    CMeshShader *m_pDeferredLightShader;
    bool m_bShadowRendered;
    short m_iLayer;
    int m_iSortKey;
};


typedef std::vector<CVisibleObject*> CVisibleObjectList;
typedef std::vector<CVisibleLight*> CVisibleLightList;

class CVisibleSet
{
public:
    CVisibleObjectList m_VisibleObjects;
    CVisibleLightList m_VisibleLights;
    CVisibleLightList m_VisibleProjectors;
};

class CVisStatistic
{
public:
	CVisStatistic();
	void Reset();
	void ResetAll();
	int m_iNumObjects;
	int m_iNumLights;
	int m_iNumUpdateObjects;
	int m_iNumUpdateLights;
	int m_iNumDirtyObjects;
	int m_iNumDirtyLights;
	int m_iNumCheckRelevants;
};

class CVisibility
{
public:
    CVisibility();
    virtual ~CVisibility();

    virtual void GetVisibleSet(const CRendererContext &rc, CVisibleSet &visSet);
    virtual void GetLightMapVisibleSet(const CRendererContext &rc, CVisibleSet &visSet, int objId, bool useAOLightMapColor);
    virtual void GenerateAmbientOcclusionLights(int num);
    virtual void RemoveAmbientOcclusionLights();
    virtual const CVisStatistic* GetStatistic();
    virtual void ResetStatistic();
    virtual void Draw(IPlotter3D &plotter, const CVisibleSet &visSet);
    virtual void Draw(IPlotter2D &canvas, const CVisibleSet &visSet);
};


}//GELib
#endif//__GELIB_RENDERER_VISIBILITY_H__
