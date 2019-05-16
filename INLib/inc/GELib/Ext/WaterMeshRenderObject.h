//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/WaterMeshRenderObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_WATERMESHRENDEROBJECT_H__
#define __GELIB_EXT_WATERMESHRENDEROBJECT_H__

#include <GELib/Ext/BaseRenderObject.h>
#include "WaterTypeDef.h"

namespace GELib
{
class CWaterMesh;
class CWaterSurface;
class CTextureAsset;
class CMeshAsset;

class GELIB_API CWaterMeshRenderObject : public CBaseRenderObject
{
	friend class CWaterManager;
public:
    CWaterMeshRenderObject(const SWaterDesc& _waterDesc,CWaterMesh *_pMesh,CWaterSurface *_pSurface);
    virtual ~CWaterMeshRenderObject();

    void SetMaterial(CMaterialAsset *pMaterial);
	CMaterialAsset * GetMaterial( int _iIndex );
	int	GetMaterialNum();

    virtual bool GetWaterInfo(CWaterInfo *pWaterInfo = 0);
    virtual bool IsSea();

    virtual void Render(CRenderer *pRenderer);
	virtual void RenderShadow(CRenderer *pRenderer){};
    virtual void UpdateRenderData(CRenderer *pRenderer);
	virtual void OnBufferBuild(CRendererContext& rc);
	virtual void OnBufferRender(CRenderer *renderer);

	virtual bool UpdateWaterMaterialParam();
	const SWaterMaterialParam& GetWaterMaterialParam()
	{
		return m_waterMaterialParam;
	}

public:
	//virtual void SetSurfaceType();
	WaterType GetSurfaceType();
	void SetSurfaceSize(float _fSizeX, float _fSizeY);
	CVector2 GetSurfaceSize();
	void SetMeshSize(float _fSizeX, float _fSizeY);
	CVector2 GetMeshSize();
	void SetWaterLevel(float _level);
	float GetWaterLevel();
	void SetMaxWaveHeight(float _waveHeight);
	float GetMaxWaveHeight();

	void EnableVertexWave(bool _bEnable);
	bool IsVertexWave();
	void SetWindVelocity(float _fVelocityX, float _fVelocityY);
	CVector2 GetWindVelocity();
	void SetWindPower(int _iWindPower);
	int GetWindPower();
	void SetWaveDimensions(float _fSizeX, float _fSizeY);
	CVector2 GetWaveDimensions();
	void SetAmplitudeScale(float _fScale);
	float GetAmplitudeScale();
	void SetLittleLen(float _fLen);
	float GetLittleLen();
	void SetWaveSpeedScale(float _fScale);
	float GetWaveSpeedScale();

	void SetWaterFresnel(float _fFresnel,float _fOffset);
	CVector2 GetWaterFresnel();
	void SetLightIntensity(float _fIntensity);
	float GetLightIntensity();

	bool IsWaterRefract();
	void EnableWaterRefract(bool _bIsRefract);
	float GetRefractVisibleDepth();
	void SetRefractVisibleDepth(float _fDepth);
	void SetRefractDistortScale(float _fScale);
	float GetRefractDistortScale();
	void SetRefractEdgeDepth(float _fDepth);
	float GetRefractEdgeDepth();
	void SetRefractIntensity(float _fIntensity);
	float GetRefractIntensity();
	void SetDeepWaterColor(const CVector3& _color);
	CVector3 GetDeepWaterColor();

	void EnableEdgeWhite(bool _bEdge);
	bool IsEdgeWhite();
	void SetEdgeVisibleDepth(float _fDepth);
	float GetEdgeVisibleDepth();
	void SetEdgeTexcoordScale(float _fScale1,float _fScale2);
	CVector2 GetEdgeTexcoordScale();
	void SetEdgeIntensity(float _fIntensity);
	float GetEdgeIntensity();
	void SetEdgeThickness(float _fThickness);
	float GetEdgeThickness();
	CVector2 GetEdgeDistort1();
	CVector2 GetEdgeDistort2();
	void SetEdgeDistort1(float _x,float _y);
	void SetEdgeDistort2(float _x,float _y);

	void EnableWaterReflect(bool _bReflect);
	bool IsWaterReflect();
	void SetReflectDistortScale(float _fScale);
	float GetReflectDistortScale();
	void SetReflectIntensity(float _fIntensity);
	float GetReflectIntensity();

	bool IsComputerWithGPU();

	//gloss
	bool IsWaterGloss();
	void EnableGloss(bool _bEnable);
	void SetGlossDistort(const CVector4& _vDistort);
	void SetSunGlowScale(float _fScale);
	void SetGlossScaleMax(float _fMax);
	void SetGlossScaleMin(float _fMin);
	void SetGlossFadeDistance(float _fDist);
	const CVector4& GetGlossDistort();
	float GetSunGlowScale(void);
	float GetGlossScaleMax(void);
	float GetGlossScaleMin(void);
	float GetGlossFadeDistance(void);
	bool SetGlossTex(CTextureAsset* _pTex);
	void UpdateGlossTex();
	CTextureAsset* GetGlossTex();

	const SWaterDesc& GetWaterDesc()
	{
		return m_waterDesc;
	}

    void SetPosition(const CVector3 &pos);

    void SetMatrix(const CMatrix &matrix);
	const CMatrix& GetMatrix()
	{
		return m_Matrix;
	}

	void GetBoundingBox(CAABox &box)
	{
		box.Set(m_Box.m_Min, m_Box.m_Max);
	}

	bool SetReflectCubeTex(CTextureAsset* _pTex);
	CTextureAsset* GetReflectCubeTex();
	bool ResetReflectCubeTex(CTextureAsset* _pTex);
	void UpdateReflectCubeTex();

	bool IntersectRay(const CVector3 &orig, const CVector3 &dir
		, float *t=0, CVector3 *normal=0);
	bool IntersectLine(const CVector3 &p1, const CVector3 &p2
		, float *t=0, CVector3 *normal=0);

    bool SetMesh(CMeshAsset *mesh);
    CMeshAsset* GetMesh();

private:
	void _UpdateDesc();
	bool UpdateBoundingBox();

private:

	CTextureAsset *m_pReflectCubeTex;
	CTextureAsset *m_pGlossTex;
    CWaterMesh *m_pMesh;
	CWaterSurface *m_pSurface;
    void* m_pMeshShaderData;
	SWaterDesc m_waterDesc;
	CVector3 m_vLastCameraPos;
	CMatrix	m_Matrix;
	CPlane	m_Plane;
	SWaterMaterialParam	m_waterMaterialParam;
};

}//GELib

#endif//__GELIB_EXT_WATERMESHRENDEROBJECT_H__
