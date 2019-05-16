//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/TerrainManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_TERRAINMANAGER_H__
#define __GELIB_EXT_TERRAINMANAGER_H__

#include <string>
#include <map>

#include "BaseAssetManager.h"
#include "TerrainAsset.h"
#include <GELib/Utility/StringMap.h>

namespace GELib
{


class CTerrainManager : public CBaseAssetManager
{
public:
    CTerrainManager();
    virtual ~CTerrainManager();

    bool Save(const char *fname, CTerrainAsset *Terrain);
    CTerrainAsset* Load(const char *fname, const CLoadParameter &param);

    CTerrainAsset* Create(const char *name, const char *cacheKey=0);
    void Delete(CTerrainAsset *Terrain);
    void DeleteLater(CTerrainAsset *Terrain);

    typedef StringMap<CTerrainAsset*> TerrainCache;
	TerrainCache& GetTerrains() { return m_Cache; }

private:

    TerrainCache m_Cache;
};


void SetTerrainManager(CTerrainManager *manager);
CTerrainManager* GetTerrainManager();




}//GELib

#endif//__GELIB_EXT_TERRAINMANAGER_H__
