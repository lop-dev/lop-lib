//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/WaterTypeDef.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_WATERTYPEDEF_H__
#define __GELIB_EXT_WATERTYPEDEF_H__

#include <GELib/Math/GeMath.h>
#include <string>

namespace GELib
{

struct GeneratorDesc
{
	bool IsChanged(const GeneratorDesc& _desc)
	{
		return _desc.m_vWindVelocity[0] != m_vWindVelocity[0]
			   ||_desc.m_vWindVelocity[1] != m_vWindVelocity[1]
			   ||_desc.m_vDimensions[0] != m_vDimensions[0]
			   ||_desc.m_vDimensions[1] != m_vDimensions[1]
			   ||_desc.m_fAmplitudeScale != m_fAmplitudeScale
			   ||_desc.m_fLittleLen != m_fLittleLen
			   ||_desc.m_iWindPower != m_iWindPower
			   ||_desc.m_iWidth != m_iWidth
			   ||_desc.m_iHeight != m_iHeight
			   ||_desc.m_bComputeDisplace != m_bComputeDisplace;
	}
	float m_vWindVelocity[2];
	float m_vDimensions[2];
	float m_fAmplitudeScale;
	float m_fLittleLen;
	int   m_iWindPower;
	int	  m_iWidth;
	int	  m_iHeight;
	float m_fClipPeriod; //0 for no clip
	bool  m_bHightFrequency;
	bool  m_bComputeDisplace;
};

enum IntersectState
{
	INTERSECT_NULL = 0,
	INTERSECT_NONE,
	INTERSECT_FULL,
	INTERSECT_FARCULL,
	INTERSECT_NEARCULL,
	INTERSECT_OTHER
};

enum WaterType
{
	WT_UNKNOW = 0,
	WT_SEA,
	WT_RIVER
};

struct SWaterDesc
{
	SWaterDesc()
	{
		m_pMeshAsset = NULL;
		m_FftDesc.m_bComputeDisplace = false;
        m_vDeepWaterColor.Set(0.0096f, 0.075f, 0.127f);
	}
	WaterType	m_eWaterType;
	int		m_iVertexWidth;
	int		m_iVertexHeight;
	float	m_fSizeWidth;
	float	m_fSizeHeight;
	float	m_fSeaLevel;
	float	m_fFresnel;
	float	m_fFresnelOffset;
	float	m_fWaveSpeedSacle;
	float	m_fMaxWaveHeight;

	float	m_fRefractVisibleDepth;
	float	m_fRefractDistortScale;
	float	m_fRefractEdgeDepth;
	float	m_fRefractIntensity;
	float	m_fEdgeVisibleDepth;
	float	m_fEdgeDepthOffset;
	float	m_fEdgeTexcoordScale1;
	float	m_fEdgeTexcoordScale2;
	float	m_fEdgeIntensity;
	float	m_fEdgeThickness;
	CVector4 m_vEdgeDistortScale;
	float	m_fReflectDistortScale;
	float	m_fReflectIntensity;
	float	m_fLightIntensity;
	bool	m_bComputeWithGPU;
	bool	m_bVertexWave;
	bool	m_bProjectGrid;
	bool	m_bRefract;
	bool	m_bReflect;
	bool	m_bEdgeWhite;
	GeneratorDesc m_FftDesc;

	//gloss
	bool	m_bGloss;
	CVector4 m_vGlossDistort;
	CVector4 m_vGlossScale;//sun glow(x) and fresnel gloss(yzw)
	std::string m_strGlossTexName;

	//material
	std::string m_strMaterialName;
	bool m_bCreateDefaultMaterial;

	//for local water
	std::string m_strLodMeshName;
	void* m_pMeshAsset;

	//refract deep color
	CVector3 m_vDeepWaterColor;

	//cube reflect map
	std::string m_strCubeMapName;
};

struct SeaVertex
{
	CVector4 position;
	CVector3 normal;
};

struct LakeVertex
{
	CVector4 position;
	short x,y;
};

struct SProjectData
{
	SProjectData()
	{
		m_pProjCorner = NULL;
	}
	CVector3	m_projectPosition;
	void*	m_pProjCorner;
};

struct SWaterMaterialParam
{
	SWaterMaterialParam()
	{
		m_iNormalMapRef = 0;
		m_iFresnelTexRef = 0;
		m_iCubeReflectMapRef = 0;
		m_iGlossTexRef = 0;
	}
	CVector4	m_vRefractFactor;
	CVector4	m_vEdgeWhiteFactor;
	CVector4	m_vReflectFactor;
	CVector4	m_vEdgeOffsetFactor;
	CVector3	m_vDeepWaterColor;
	CVector4	m_vGlossDistort;
	CVector4	m_vGlossScale;
	int m_iNormalMapRef;
	int m_iFresnelTexRef;
	int m_iCubeReflectMapRef;
	int m_iGlossTexRef;
};

}//GELib

#endif//__GELIB_EXT_WATERTYPEDEF_H__
