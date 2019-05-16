//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/AnimationManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_ANIMATIONMANAGER_H__
#define __GELIB_CHARACTER_ANIMATIONMANAGER_H__

#include <string>
#include <map>

#include "AnimationAsset.h"
#include <GELib/Utility/StringMap.h>
#include <GELib/Ext/BaseAssetManager.h>

namespace GELib
{


class CAnimationManager : public CBaseAssetManager
{
public:
    CAnimationManager();
    virtual ~CAnimationManager();

    bool Save(const char *fname, CAnimationAsset *anim);
    CAnimationAsset* Load(const char *fname, const CLoadParameter &param);
    CAnimationAsset* Import(const char *fname, int compressLevel=0);	// no compress
    CAnimationAsset* ImportAdditive(const char *fname, const char *baseFname, const char *name, int compressLevel=0);

    CAnimationAsset* Create(const char *name, const char *cacheKey=0);
    void Delete(CAnimationAsset *anim);
    void DeleteLater(CAnimationAsset *anim);

    typedef StringMap<CAnimationAsset*> AnimationCache;
	AnimationCache& GetAnimations() { return m_Cache; }

private:

    AnimationCache m_Cache;
};

void SetAnimationManager(CAnimationManager *manager);
CAnimationManager* GetAnimationManager();



}//GELib

#endif//__GELIB_CHARACTER_ANIMATIONMANAGER_H__
