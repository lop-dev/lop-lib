//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/ActManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_ACTMANAGER_H__
#define __GELIB_EXT_ACTMANAGER_H__

#include <vector>
#include "ActAsset.h"
#include <GELib/Utility/StringMap.h>
#include <GELib/Ext/BaseAssetManager.h>


namespace GELib
{


class CActManager : public CBaseAssetManager
{
public:
    CActManager();
    virtual ~CActManager();

    bool Save(const char *fname, CActAsset *a);
    CActAsset* Load(const char *fname, const CLoadParameter &param);

    CActAsset* Create(const char *name, const char *cacheKey=0);
    void Delete(CActAsset *a);
    void DeleteLater(CActAsset *a);

    typedef StringMap<CActAsset*> ActCache;
	ActCache& GetActs() { return m_Cache; }

private:

    ActCache m_Cache;
};


void SetActManager(CActManager *manager);
CActManager* GetActManager();



}//GELib

#endif//__GELIB_EXT_ACTMANAGER_H__
