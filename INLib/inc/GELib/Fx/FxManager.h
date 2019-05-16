//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXMANAGER_H__
#define __GELIB_FX_FXMANAGER_H__

#include <vector>
#include "FxAsset.h"
#include <GELib/Utility/StringMap.h>
#include <GELib/Ext/BaseAssetManager.h>


namespace GELib
{


class CFxManager : public CBaseAssetManager
{
public:
    CFxManager();
    virtual ~CFxManager();

    bool Save(const char *fname, CFxAsset *fx);
    CFxAsset* Load(const char *fname, const CLoadParameter &param);

    CFxAsset* Create(const char *name, const char *cacheKey=0);
    void Delete(CFxAsset *fx);
    void DeleteLater(CFxAsset *fx);

    typedef StringMap<CFxAsset*> FxCache;
	FxCache& GetFxs() { return m_Cache; }

private:

    FxCache m_Cache;
};


void SetFxManager(CFxManager *manager);
CFxManager* GetFxManager();



}//GELib

#endif//__GELIB_FX_FXMANAGER_H__
