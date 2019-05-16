//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/NavMeshManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_NAVMESHMANAGER_H__
#define __GELIB_EXT_NAVMESHMANAGER_H__

#include <string>
#include <map>

#include "BaseAssetManager.h"
#include "NavMeshAsset.h"
#include <GELib/Utility/StringMap.h>

namespace GELib
{


class CNavMeshManager : public CBaseAssetManager
{
public:
    CNavMeshManager();
    virtual ~CNavMeshManager();

    bool Save(const char *fname, CNavMeshAsset *nav);
    CNavMeshAsset* Load(const char *fname, const CLoadParameter &param);

    CNavMeshAsset* Create(const char *name, const char *cacheKey=0);
    void Delete(CNavMeshAsset *nav);
    void DeleteLater(CNavMeshAsset *nav);

    typedef StringMap<CNavMeshAsset*> NavMeshCache;
	NavMeshCache& GetNavMeshes() { return m_Cache; }

private:

    NavMeshCache m_Cache;
};


void SetNavMeshManager(CNavMeshManager *manager);
CNavMeshManager* GetNavMeshManager();


}//GELib

#endif//__GELIB_EXT_NAVMESHMANAGER_H__
