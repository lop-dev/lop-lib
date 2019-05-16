//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/WaterManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_WATERMANAGER_H__
#define __GELIB_EXT_WATERMANAGER_H__

#include "WaterTypeDef.h"
#include <GELib/GraphicDevice/GraphicDevice.h>
#include <vector>
#include <map>

extern const int sc_iSimplePostProcessId;

namespace GELib
{

#define FLOAT_MAX_VALUE (3.4E+38f)

class CMaterialAsset;
class CTextureAsset;
class CRenderWorld;
class CRenderer;
class CWaterSurface;
class CWaterMesh;
class CWaterMeshRenderObject;
class CWaterEntity;

class GELIB_API CWaterManager
{
public:
    CWaterManager();
    ~CWaterManager();

	bool Initialize(bool _bReflect,CRenderWorld * _pRenderWorld,CRenderer *_pRender);
	void UnInitialize();
	bool OnResetWindow(int _iWidth, int _iHeight);
	bool OnRenderView();
	bool Update(const CVector3& vCameraPos);

	bool SwapRenderBuffer();
	int GetViewTarget();
	CVector4 GetReflectPlane()
	{
		return m_reflectPlane;
	}

	bool IsEnableScissor()
	{
		return m_bUseScissor;
	}

	void EnableScissor(bool _bUseScissor)
	{
		m_bUseScissor = _bUseScissor;
	}

	void SetReflectScissor(CVector4& _scissor)
    {
        m_updateReflectScissor = _scissor;
	}

	void AddReflectScissor(CVector4& _scissor)
	{
		if(m_bUseScissor)
		{
			if(_scissor.x<m_updateReflectScissor.x)
				m_updateReflectScissor.x = _scissor.x;
			if(_scissor.y<m_updateReflectScissor.y)
				m_updateReflectScissor.y = _scissor.y;

			if(_scissor.z>m_updateReflectScissor.z)
				m_updateReflectScissor.z = _scissor.z;
			if(_scissor.w>m_updateReflectScissor.w)
				m_updateReflectScissor.w = _scissor.w;
		}
	}

	CVector4& GetReflectScissor( )
	{
		return m_renderReflectScissor;
	}

	void SetVisibleReflect(bool _bVisible,const CVector3& _pos,float _fWaterPlane,float _fWaveHeight);

	bool				IsSeaExist();
	CWaterMesh*			CreateWaterMesh(const SWaterDesc& _desc);
	CWaterSurface*		CreateWaterSurface(const SWaterDesc& _desc);
	void				DeleteWaterMesh(CWaterMesh* _waterMesh);
	void				DeleteWaterSurface(CWaterSurface* _waterSurface);
	CTextureAsset* GetFresnelTexture(float _fFresnel,float _fOffset);
	void				ReleaseFresnelTexture(int _iIndex);

	CWaterMeshRenderObject*	CreateWaterRenderObject(const SWaterDesc& _desc);
	void				DeleteWaterRenderObject(CWaterMeshRenderObject* _obj);

	void				UpdateReflectHeight(const SWaterDesc& _desc);

	int GetReflectTargetScale()
	{
		return m_iReflectTargetScale;
	}
	void SetReflectTargetScale(int _iScale);

	void EnableAllVertexWave(bool _bEnable);
	bool IsAllVertexWaveEnable()
	{
		return m_bAllVertexWaveEnable;
	}
	bool IsDeviceSupportVertexWave();
	bool IsDeviceSupportEdgeAndReflect();
	void UpdateAllRenderObjectDesc();

    bool IsDepthTargetSupport()
    {
        return m_bDepthTargetSupport;
    }

	//interaction interface
	bool IsInteractionWithVertex()
	{
		return m_bInteractionVertex&&IsDeviceSupportVertexWave();
	}

	bool IsInteractionWithNormal()
	{
		return m_bInteractionNormal;
	}

	void EnableInteractionWithVertex(bool _bEnable)
	{
		m_bInteractionVertex = _bEnable;
	}

	void EnableInteractionWithNormal(bool _bEnable)
	{
		m_bInteractionNormal = _bEnable;
	}

	bool IsInteractionFollowCamera()
	{
		return m_bFollowCamera;
	}

	void EnableInteractionFollowCamera(bool _bEnable)
	{
		m_bFollowCamera = _bEnable;
	}

	const CVector4& GetInteractionAera()
	{
		return m_vInteractionArea;
	}

	const CVector4& GetInteractionParam()
	{
		return m_vInteractionParam;
	}

	void SetInteractionParam(const CVector4& _vParam)
	{
		m_vInteractionParam = _vParam;
	}

	const CVector4& GetAeraOffsetParam(bool& bAreaChanged)
	{
		bAreaChanged = m_bNeedCopyInteractionData;
		m_bNeedCopyInteractionData = false;
		return m_vAreaOffsetAndScale;
	}

	void SetInteractionAera(const CVector4& _vInteractionAera)
	{
		if(m_vInteractionArea!=_vInteractionAera)
		{
			m_vLastInteractionArea = m_vInteractionArea;
			m_vInteractionArea = _vInteractionAera;
			m_bInteractionAreaChanged = true;
		}
	}

	bool IsVertexTextureSupport()
	{
		return m_bVertexTextureEnable;
	}

	float GetWaterPlaneHeight(float x,float y,float z = 0.0f);//return 0,if no water

	std::string GetDefaultGlossTexture();
	std::string GetInteractionNoiseTexture();

	//for server
	 void IgnoreLoad(bool ignore) { m_bIgnoreLoad = ignore; }
	 bool IsIgnoreLoad() {return m_bIgnoreLoad;}

     // Quality
     enum
     {
         QUALITY_EDGE = 0,
         QUALITY_REFRACT,
         QUALITY_REFLECT,
         QUALITY_REFLECTCUBE,
         QUALITY_EDGEDISTORTION,
         QUALITY_GLOSS,
         QUALITY_DISTORT,
         QUALITY_VERTEXWAVE,
         QUALITY_MAX = 7  // Can not Exceed Max
     };
private:

	CMaterialAsset* _CreateMaterial(const SWaterDesc& _desc);
	bool _CreateReflectRenderView();
	bool _UpdateReflectRenderView();
	void _DestroyReflecRendertView();
	bool _RenderReflectBuffer();
	void _ReleaseWater();
	bool _RecreateResource();
	void _UpdateInteractionArea(float _fPosX, float _fPosY);

private:
	typedef  std::vector<CWaterSurface *> WaterSurfaceList;
	typedef  std::vector<CWaterMesh *> WaterMeshList;
	typedef  std::vector<CWaterMeshRenderObject *> WaterRenderObjectList;
	typedef	 std::pair<int,CTextureAsset*> FresnelPair;
	typedef	 std::map<int,CTextureAsset*> FresnelTextureMap;

	WaterSurfaceList		m_WaterSurfaces;
	WaterMeshList			m_WaterMeshs;
	WaterRenderObjectList	m_WaterRenderObjects;

	FresnelTextureMap		m_fresnelMap;
	CRenderWorld *m_pRenderWorldRef;
	CRenderer* m_pRenderRef;

	int		m_iReflectTargetScale; // 1/m_iReflectTargetScale
	int		m_iReflectRenderView;

	float	m_fMaxWaveHeight;  //use for update
	float	m_fWaterPlane;
	bool	m_bInteractionAreaChanged;
	CVector4 m_updateReflectScissor;
	CVector4 m_vInteractionParam; //x: height scale y:water resistance z:force transform speed w:force save speed

	float	m_fNearestWaveHeight;//use for render
	float	m_fNearestWaterPlane;
	bool	m_bNeedCopyInteractionData;
	CVector4 m_vAreaOffsetAndScale;

	bool m_bUseScissor;
	CVector4 m_reflectPlane;
	CVector4 m_renderReflectScissor;
	CVector3 m_cameraPos;
	bool	m_bReflect;
	bool	m_bRecreateResource;
	bool	m_bVisibleReflect;
	bool	m_bVisibleReflectTemp;
	float	m_fVisibleMinDistance;
	bool	m_bAllVertexWaveEnable;
	bool	m_bVertexTextureEnable;
	EPixelFormat m_eFresnelFormat;
	int		m_iSettingChange;

	//interaction
	bool m_bInteractionVertex;
	bool m_bInteractionNormal;
	bool m_bFollowCamera;
	CVector4 m_vInteractionArea;
	CVector4 m_vLastInteractionArea;

	//water pass flag
	bool m_bWaterPassEnable;

	//for server
	bool m_bIgnoreLoad;

    bool m_bDepthTargetSupport;
};

void SetWaterManager(CWaterManager *m);
CWaterManager* GetWaterManager();

}//GELib

#endif//__GELIB_EXT_WATERMANAGER_H__
