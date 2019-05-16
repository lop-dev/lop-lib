//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/RenderMaterial.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_RENDERMATERIAL_H__
#define __GELIB_RENDERER_RENDERMATERIAL_H__

#include <string>

#include <GELib/GeConfig.h>
#include "RendererTypes.h"
#include "ShaderFunction.h"


namespace GELib
{

class CGraphicDevice;

class CMeshShader;
class CRenderMesh;
class CShaderSettings;


enum EMaterialShaderProperty
{
	MSP_LIGHTING = 1 << 0,
	MSP_SPECULAR = 1 << 1,
	MSP_DISTORTION = 1 << 2,
	MSP_GLOW = 1 << 3,
	MSP_VERTEX_COLOR = 1 << 4,
	MSP_TANGENT_TO_PS = 1 << 5,
	MSP_VIEW_TO_PS = 1 << 6,
	MSP_ERROR = 1 << 31,
};

class CRenderMaterial
{
public:
    GELIB_API CRenderMaterial();
    GELIB_API virtual ~CRenderMaterial();
    GELIB_API virtual int GetClassId();

	// call CheckShaderProperties() before GetShader()
    GELIB_API virtual void CheckShaderProperties();
    GELIB_API virtual CMeshShader* GetShader(EShaderFunction func, CRenderMesh *mesh, ELightMapMode lightMapMode);
    GELIB_API virtual void ClearShaders();

	void SetShaderProperty(int p) { m_iShaderProperties |= p; }
	void ClearShaderProperty(int p) { m_iShaderProperties &= ~p; }
	bool HasShaderProperty(int p) { return (m_iShaderProperties & p) != 0; }

    bool m_bShow;
    bool m_bCastShadow;
    bool m_bVertexLight;
    bool m_bVertexFog;
    bool m_bTwoSided;
    bool m_bTwoSidedLighting;
    bool m_bTwoSidedTwoPass;
    bool m_bDepthWrite;
    bool m_bDepthTest;
    bool m_bAlphaTest;
    bool m_bAccumulateDistortion;
	bool m_bSinglePass;
    float m_fAlphaRef;

    EBlendMode m_eBlendMode;
    EVolumeMode m_eVolumeMode;
    EResolutionMode m_eResolutionMode;

protected:

    int m_iShaderProperties;
};



}//GELib

#endif//__GELIB_RENDERER_RENDERMATERIAL_H__
