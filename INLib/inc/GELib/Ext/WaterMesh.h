//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/WaterMesh.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_WATERMESH_H__
#define __GELIB_EXT_WATERMESH_H__

#include <GELib/Renderer/RenderMesh.h>
#include "WaterTypeDef.h"
#include "WaterSurface.h"

namespace GELib
{

class CMaterialAsset;
class CStdMaterial;
class CMeshAsset;

const int MESHCLASS_SEAMESH = 12;

class CWaterMesh : public CRenderMesh
{
public:

    GELIB_API CWaterMesh();
    GELIB_API virtual ~CWaterMesh();
    GELIB_API virtual int GetClassId() = 0;
	GELIB_API virtual bool CreateMesh(const SWaterDesc& _waterDesc) = 0;
	GELIB_API virtual bool UpdateDesc(const SWaterDesc& _waterDesc) = 0;
	//update mesh data on swap buffer
	GELIB_API virtual bool UpdateData(CWaterSurface *_pSurface) {return true;}
	GELIB_API virtual CMaterialAsset* GetMaterial(int chunk) = 0;
	GELIB_API virtual void SetMaterial(CMaterialAsset* pSeaMaterial) = 0;
	GELIB_API virtual CStdMaterial* GetRenderMaterial(int chunk) = 0;
	GELIB_API virtual int GetNumChunks() = 0;
	GELIB_API virtual void* NewShareData() = 0;
	GELIB_API virtual void DeleteShaderData(void *sd) = 0;

	GELIB_API virtual void SetVisible(bool _bVisibale) = 0;
	GELIB_API virtual bool GetVisible( ) = 0;
	GELIB_API virtual void SetHightMap(int _iHightMap) {}
    GELIB_API virtual void GetBoundingBox(CAABox& _bBox) {}
    GELIB_API virtual void SetShaderQuality(int iQuality) = 0;
    GELIB_API virtual bool SetMesh(CMeshAsset* _pMesh) { return false; }
    GELIB_API virtual CMeshAsset* GetMesh() { return NULL; }
	GELIB_API void	IncreaseRef(){ ++m_iRefrenceCounter;}
	GELIB_API void	DecreaseRef(){ --m_iRefrenceCounter;}
	GELIB_API int   GetRefCounter(){ return m_iRefrenceCounter;}
protected:
	int m_iRefrenceCounter;
};

}//GELib

#endif//__GELIB_EXT_WATERMESH_H__
