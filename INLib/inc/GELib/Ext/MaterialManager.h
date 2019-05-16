//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/MaterialManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_MATERIALMANAGER_H__
#define __GELIB_EXT_MATERIALMANAGER_H__

#include <string>
#include <map>

#include "BaseAssetManager.h"
#include "MaterialAsset.h"
#include "MeshUtility.h"
#include <GELib/Utility/StringMap.h>
#include <GELib/Utility/CircularBuffer.h>

namespace GELib
{


class CMaterialManager : public CBaseAssetManager
{
public:
    CMaterialManager();
    virtual ~CMaterialManager();

    bool Save(const char *fname, CMaterialAsset *material);
    CMaterialAsset* Load(const char *fname, const CLoadParameter &param);
	CMaterialAsset* Import(CMeshUtility::Material &meshMaterial, const char *name, const CLoadParameter &param);

    CMaterialAsset* Create(const char *name, const char *cacheKey=0);
    void Delete(CMaterialAsset *material);
    void DeleteLater(CMaterialAsset *material);

	// update materials use this as template
	void UpdateRelatedMaterials(CMaterialAsset *material);

    typedef StringMap<CMaterialAsset*> MaterialCache;
	MaterialCache& GetMaterials() { return m_Cache; }

private:

    MaterialCache m_Cache;
};


void SetMaterialManager(CMaterialManager *manager);
CMaterialManager* GetMaterialManager();



}//GELib

#endif//__GELIB_EXT_MATERIALMANAGER_H__
