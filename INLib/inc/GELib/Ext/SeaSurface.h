//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/SeaSurface.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_SEASURFACE_H__
#define __GELIB_EXT_SEASURFACE_H__

#include <vector>
#include <GELib/Math/Matrix.h>
#include "WaterTypeDef.h"
#include "WaterSurface.h"

namespace GELib
{
class CIOStream;
class CStdMaterial;
class CRendererContext;
class CMaterialAsset;
class CBaseRenderObject;
class CSeaWaveGenerator;

class CSeaSurface : public CWaterSurface
{
public:
	CSeaSurface(void);
	GELIB_API virtual bool CreateSurface(const SWaterDesc& _waterDesc);
	GELIB_API virtual bool UpdateDesc(const SWaterDesc& _waterDesc);
	GELIB_API virtual void* GetVertexData(unsigned int& _OutSize);
	GELIB_API virtual bool BuildSurface(GELib::CRendererContext &rc);
	GELIB_API virtual bool OnUpdateData();
	GELIB_API virtual bool OnRender(CRenderer * renderer);
    GELIB_API virtual bool GetWaterInfo(CWaterInfo *waterInfo);
	GELIB_API virtual bool OnRecreateSource();
	GELIB_API virtual int GetHeightMap()
	{
		return m_iHeightMapTex;
	}
	GELIB_API virtual int GetNormalMap()
	{
		return m_iSurfaceNormalTarget;
	}
	GELIB_API virtual bool GetVisible()
	{
		return m_bVisible;
	}
	GELIB_API virtual ~CSeaSurface(void);

	GELIB_API static void GetDefaultSurfaceDesc(SWaterDesc& _waterDesc);

protected:
	//for project grid
	void _BuildProjector(GELib::CRendererContext &rc, float _MaxHeight,float _SeaLevel);
	IntersectState _GetIntersectPoint(const std::vector<GELib::CVector3>& _corner
		, std::vector<GELib::CVector3>& _points,float _fPlane);
	bool _GetIntersectPoint(const GELib::CVector3& _point1,const GELib::CVector3& _point2
		,GELib::CVector3& _pointOut,float _fPlane);
	float _GetHeight(float _fPosX, float _fPosY);
	GELib::CMatrix	m_cameraMatrix;
	GELib::CMatrix	m_projectMatrix;
	GELib::CVector3	m_projectPosition;
	GELib::CVector4	m_ProjCorner[4];
	GELib::CVector2	m_maxScreenValue;
	GELib::CVector2	m_minScreenValue;

protected:
	bool _CreateResource();
	void _FreeResource();
protected:
	enum
	{
		WAVE_FFT_WIDTH = 64,
		WAVE_FFT_HEIGHT = 64,
		WAVE_SAVE_NUM	= 24
	};
	std::vector<float> m_pWaveNoise;
	std::vector<SeaVertex> m_VertexBuff;
	std::vector<GELib::CVector4> m_pPreComputeHolder[WAVE_SAVE_NUM];
	SProjectData	m_projectData;
	int		m_iVertexSurfaceWidth;
	int		m_iVertexSurfaceHeight;
	bool	m_bVisible;

	//sea mesh describe
	bool	m_bComputeWithGPU;
	bool	m_bVertexWave;
	float	m_fSizeWidth;
	float	m_fSizeHeight;
	float	m_fSeaLevel;
	float	m_fMaxWaveHeight;
	float	m_fTotalTime;
	int		m_iSurfaceNormalTarget;
	int		m_iHeightMapTex;
	float	m_fWaveSpeedScale;

	CSeaWaveGenerator *m_pFftGenerator;

	//add for displace wave
	std::vector<GELib::CVector4> m_pWaveNoiseWithDisplace;
	bool m_bComputeDisplace;
private:
	void _CopyToSaveWave(int _iIndex);
};

}//GELib


#endif//__GELIB_EXT_SEASURFACE_H__
