//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/LakeSurface.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_LAKESURFACE_H__
#define __GELIB_EXT_LAKESURFACE_H__

#include "WaterSurface.h"
#include "PerlinNoiseMaker.h"
#include <GELib/Math/GeMath.h>
#include "WaterTypeDef.h"
#include <vector>

namespace GELib
{
class CIOStream;
class CStdMaterial;
class CRendererContext;
class CMaterialAsset;
class CMeshUtility;
class CBaseRenderObject;
class CWaterInfo;

class CLakeSurface : public CWaterSurface
{
public:
	GELIB_API CLakeSurface(void);
	GELIB_API virtual bool CreateSurface(const SWaterDesc& _waterDesc);
	GELIB_API virtual bool UpdateDesc(const SWaterDesc& _waterDesc);
	GELIB_API virtual void* GetVertexData(unsigned int& _OutSize);
	GELIB_API virtual bool BuildSurface(CRendererContext &rc);
	GELIB_API virtual bool OnUpdateData();
	GELIB_API virtual bool OnRender(CRenderer *_pPass);
    GELIB_API virtual bool GetWaterInfo(CWaterInfo *waterInfo);
	GELIB_API virtual bool OnRecreateSource();
	GELIB_API virtual int GetHeightMap()
	{
		return m_iHeightMapTarget;
	}
	GELIB_API virtual int GetNormalMap()
	{
		return m_iSurfaceNormalTarget;
	}
	GELIB_API virtual bool GetVisible()
	{
		return m_bVisible;
	}
	GELIB_API virtual ~CLakeSurface(void);

	GELIB_API static void GetDefaultSurfaceDesc(SWaterDesc& _waterDesc);

protected:
	bool _CreateResource();
	void _FreeResource();

protected:

	int		m_iSurfaceWidth;
	int		m_iSurfaceHeight;
	bool	m_bVisible;

	//surface describe
	bool	m_bComputeWithGPU;
	bool	m_bVertexWave;
	float	m_fSizeWidth;
	float	m_fSizeHeight;
	float	m_fSeaLevel;
	float	m_fMaxWaveHeight;
	float	m_fTotalTime;
	float	m_fWaveSpeedScale;
	int		m_iSurfaceNormalTarget;
	int		m_iHeightMapTarget;

	//vertex wave describe
	float m_vWindVelocity[2];
	float m_vDimensions[2];
	float m_fAmplitudeScale;
	int   m_iWindPower;

	PerlinNoiseDesc m_PerlinNoiseDesc;

private:
};

}//GELib


#endif//__GELIB_EXT_LAKESURFACE_H__
