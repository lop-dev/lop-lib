//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/MeshManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_MESHMANAGER_H__
#define __GELIB_EXT_MESHMANAGER_H__

#include <string>
#include <map>

#include "BaseAssetManager.h"
#include "MeshAsset.h"
#include <GELib/Utility/StringMap.h>

namespace GELib
{

class CColladaFile;


class CMeshManager : public CBaseAssetManager
{
public:
    CMeshManager();
    virtual ~CMeshManager();

    bool Save(const char *fname, CMeshAsset *mesh);
    CMeshAsset* Load(const char *name, CMeshAsset::ERenderType rt, const CLoadParameter &param);
    CMeshAsset* Import(const char *fname, CMeshAsset::ERenderType rt);
    CMeshAsset* Import(const char *fname, CColladaFile *collada, const char *nodeName, const char *name,
						CMeshAsset::ERenderType rt);

    CMeshAsset* Create(const char *name, CMeshAsset::ERenderType rt, const char *cacheKey=0);
    void Delete(CMeshAsset *mesh);
    void DeleteLater(CMeshAsset *mesh);

    typedef StringMap<CMeshAsset*> MeshCache;
	MeshCache& GetMeshes(CMeshAsset::ERenderType rt) { return m_Cache[rt]; }

private:
    bool ColladaToMesh(CColladaFile *collada, const char *nodeName,
						CMeshAsset *mesh, CMeshAsset::ERenderType rt, const CLoadParameter &param);

    MeshCache m_Cache[CMeshAsset::RT_MAX];
};


void SetMeshManager(CMeshManager *manager);
CMeshManager* GetMeshManager();



}//GELib

#endif//__GELIB_EXT_MESHMANAGER_H__
