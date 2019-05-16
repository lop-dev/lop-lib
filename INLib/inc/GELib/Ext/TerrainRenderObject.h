//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/TerrainRenderObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_TERRAINRENDEROBJECT_H__
#define __GELIB_EXT_TERRAINRENDEROBJECT_H__

#include <vector>

#include "BaseRenderObject.h"
#include "TerrainMesh.h"


namespace GELib
{

class CRenderer;
class CLightMapInfo;


class CMaterialAsset;
class CTerrain;

class CTerrainRenderObject : public CBaseRenderObject
{
public:
    CTerrainRenderObject();
    virtual ~CTerrainRenderObject();

    virtual void Render(CRenderer *renderer);
    virtual void UpdateRenderData(CRenderer *renderer);

	void Initialize(CTerrain *terrain, int lod, int patchX, int patchY);

	int GetLod() { return m_iLod; }
	int GetPatchX() { return m_iPatchX; }
	int GetPatchY() { return m_iPatchY; }

	void TerrainHeightMapChanged();
	void TerrainBoundingChanged();
	void GetHeightRange(float &hmin, float &hmax) { hmin = m_fMinHeight; hmax = m_fMaxHeight; }

    void SetLightMap(CLightMapInfo *lm);
    CLightMapInfo* GetLightMap() { return m_pLightMap; }

	void SetMeshMode(ETerrainMeshMode mode) { m_eMeshMode = mode; }

	CTerrainMesh* GetMesh() { return &m_Mesh; }

private:

	short m_iLod;
	short m_iPatchX;
	short m_iPatchY;
	float m_fMinHeight;
	float m_fMaxHeight;
	float m_fSkirtHeight;
	bool m_bHeightMapDirty;
	bool m_bHasHole;

	ETerrainMeshMode m_eMeshMode;
	CTerrainMesh m_Mesh;
	CTerrainMeshShaderData m_ShaderData;
    GELib::CLightMapInfo *m_pLightMap;
	CTerrain *m_pTerrain;
};



}//GELib

#endif//__GELIB_EXT_TERRAINRENDEROBJECT_H__
