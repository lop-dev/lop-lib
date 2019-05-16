//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/WaterMaterialShader.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_WATERMATERIALSHADER_H__
#define __GELIB_EXT_WATERMATERIALSHADER_H__

#include <GELib/Renderer/StdMaterialShader.h>
#include <GELib/Renderer/Shader.h>

namespace GELib
{
class CRendererContext;
class CLightWaterShader : public CStdDirectionalLightShader
{
public:
	CLightWaterShader();
	virtual ~CLightWaterShader();
	virtual void SetParameters(GELib::CRendererContext &rc);

private:
	GELib::CShaderParameter	m_ReflectSampler;
	GELib::CShaderParameter	m_RefratSampler;
	GELib::CShaderParameter	m_WorldToScreen;
	GELib::CShaderParameter	m_FresnelSampler;
	GELib::CShaderParameter	m_NormalMap;
	GELib::CShaderParameter	m_RefractFactor;
	GELib::CShaderParameter	m_EdgeWhiteFactor;
	GELib::CShaderParameter	m_ReflectFactor;
	GELib::CShaderParameter	m_EdgeOffsetFactor;
	GELib::CShaderParameter	m_DeepWaterColor;

    CShaderParameter m_AmbientColor1;
    CShaderParameter m_AmbientColor2;
};
}//GELib

#endif//__GELIB_EXT_WATERMATERIALSHADER_H__
