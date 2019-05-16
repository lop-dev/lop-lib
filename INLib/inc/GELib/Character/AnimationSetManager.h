//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/AnimationSetManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_ANIMATIONSETMANAGER_H__
#define __GELIB_CHARACTER_ANIMATIONSETMANAGER_H__

#include <string>
#include <map>

#include <GELib/Ext/BaseAssetManager.h>
#include <GELib/Utility/StringMap.h>
#include "AnimationSet.h"

namespace GELib
{

class CAnimationSetManager : public CBaseAssetManager
{
public:
    CAnimationSetManager();
    virtual ~CAnimationSetManager();

    CAnimationSet* Load(const char *fname, const CLoadParameter &param);

    CAnimationSet* Create(const char *name, const char *cacheKey=0);
    void Delete(CAnimationSet *as);
    void DeleteLater(CAnimationSet *as);

    typedef StringMap<CAnimationSet*> AnimationSetCache;
	AnimationSetCache& GetAnimationSets() { return m_Cache; }

private:

    AnimationSetCache m_Cache;
};


void SetAnimationSetManager(CAnimationSetManager *manager);
CAnimationSetManager* GetAnimationSetManager();



}//GELib

#endif//__GELIB_CHARACTER_ANIMATIONSETMANAGER_H__
