//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/SkinMeshManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_SKINMESHMANAGER_H__
#define __GELIB_CHARACTER_SKINMESHMANAGER_H__

#include <string>
#include <map>

#include "SkinMeshAsset.h"
#include <GELib/Utility/StringMap.h>
#include <GELib/Ext/BaseAssetManager.h>

namespace GELib
{

class CColladaFile;


class CSkinMeshManager : public CBaseAssetManager
{
public:
    CSkinMeshManager();
    virtual ~CSkinMeshManager();

    bool Save(const char *fname, CSkinMeshAsset *skin);
    CSkinMeshAsset* Load(const char *fname, const CLoadParameter &param);
    CSkinMeshAsset* Import(const char *fname);

    CSkinMeshAsset* Create(const char *name, const char *cacheKey=0);
    void Delete(CSkinMeshAsset *skin);
    void DeleteLater(CSkinMeshAsset *skin);

    bool IsExist(CSkinMeshAsset *skin);

    typedef StringMap<CSkinMeshAsset*> SkinMeshCache;
	SkinMeshCache& GetSkinMeshes() { return m_Cache; }

private:

    SkinMeshCache m_Cache;
};


void SetSkinMeshManager(CSkinMeshManager *manager);
CSkinMeshManager* GetSkinMeshManager();


}//GELib

#endif//__GELIB_CHARACTER_SKINMESHMANAGER_H__
