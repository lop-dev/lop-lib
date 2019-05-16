//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/TerrainAsset.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_TERRAINASSET_H__
#define __GELIB_EXT_TERRAINASSET_H__

#include <vector>

#include "BaseAsset.h"

namespace GELib
{

class CIOStream;
class CTerrainManager;
class CCollisionShape;


const int TERRAIN_HEIGHT_SAMPLE_MAX = 32760;	// use 32727 will cause physics error
const int ASSETCLASS_TERRAIN = 12;

class CTerrainAsset : public CBaseAsset
{
public:
    CTerrainAsset(CTerrainManager *manager, const char *cacheKey=0);
    virtual ~CTerrainAsset();

	virtual int GetClassId() { return ASSETCLASS_TERRAIN; }
    CTerrainManager* GetManager() { return m_pManager; }
    virtual void ManagerDelete();

    bool Save(CIOStream &stream);
    bool Load(CIOStream &stream, const CLoadParameter &param);
    void Unload();

	void Allocate(int size, bool clear=true);
	void Resize(int size, float maxHeight);
	void BuildEditorMap(float maxHeight);
	void ClearEditorMap();

	int GetSize() { return m_iSize; }

	void SetCollisionShape(int index, CCollisionShape *shape);
	CCollisionShape* GetCollisionShape(int index);

	std::vector<short> m_HeightMap;
	std::vector<float> m_EditorHeightMap;
	std::vector<unsigned char> m_AttributeMap;
	std::vector<unsigned char> m_EditorColorMap;
	std::vector<unsigned char> m_EditorBlendMap;
	std::vector<unsigned char> m_GrassCellMap;

private:

	int m_iSize;
    CTerrainManager *m_pManager;
	std::vector<CCollisionShape*> m_CollisionShapes;
};



}//GELib

#endif//__GELIB_EXT_TERRAINASSET_H__
