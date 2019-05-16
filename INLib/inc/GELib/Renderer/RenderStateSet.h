//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/RenderStateSet.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_RENDERSTATESET_H__
#define __GELIB_RENDERER_RENDERSTATESET_H__

#include <GELib/GeConfig.h>
#include <GELib/GraphicDevice/GraphicDeviceTypes.h>


namespace GELib
{

class GELIB_API CRenderStateSet
{
public:
	CRenderStateSet();
	virtual ~CRenderStateSet();

	void SetDefault();
	void GetCurrent();
	void Apply();
	void ForceApply();

	bool m_bAlphaTest;
	bool m_bBlend;
    bool m_bSeparateAlphaBlend;
	bool m_bDepthTest;
	bool m_bDepthWrite;
	EBlendFactor m_eSrcBlendFactor;
	EBlendFactor m_eDestBlendFactor;
	EBlendOp m_eBlendOp;
	EBlendFactor m_eSrcBlendFactorAlpha;
	EBlendFactor m_eDestBlendFactorAlpha;
	EBlendOp m_eBlendOpAlpha;
};


}//GELib

#endif//__GELIB_RENDERER_RENDERSTATESET_H__
