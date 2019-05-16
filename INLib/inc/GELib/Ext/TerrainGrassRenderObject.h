//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/TerrainGrassRenderObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_TERRAINGRASSRENDEROBJECT_H__
#define __GELIB_EXT_TERRAINGRASSRENDEROBJECT_H__

#include <vector>

#include "BaseRenderObject.h"
#include "TerrainGrassMesh.h"


namespace GELib
{

class CRenderer;
class CLightMapInfo;
class IPlotter3D;


class CMaterialAsset;
class CTerrain;

class CTerrainGrassMeshCell
{
public:
	short m_iX;
	short m_iY;
	short m_iLife;
	bool m_bDirty;
	CTerrainGrassMesh m_Mesh;
};


class CTerrainGrassRenderObject : public CBaseRenderObject
{
public:
    CTerrainGrassRenderObject();
    virtual ~CTerrainGrassRenderObject();

    virtual void Render(CRenderer *renderer);
    virtual void UpdateRenderData(CRenderer *renderer);

	void Initialize(CTerrain *terrain, int patchX, int patchY);

	void Update(float t);
    void Draw(IPlotter3D &plotter);

	void TerrainHeightMapChanged();
	void TerrainBoundingChanged();

    void SetLightMap(CLightMapInfo *lm);
    CLightMapInfo* GetLightMap() { return m_pLightMap; }

private:

	short m_iPatchX;
	short m_iPatchY;
	float m_fMinHeight;
	float m_fMaxHeight;

	std::vector<CTerrainGrassMeshCell*> m_Meshes;
	CTerrainGrassMeshShaderData m_ShaderData;
    GELib::CLightMapInfo *m_pLightMap;
	CTerrain *m_pTerrain;
};


}//GELib

#endif//__GELIB_EXT_TERRAINGRASSRENDEROBJECT_H__
