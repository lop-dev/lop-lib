//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/CharacterDescManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_CHARACTERDESCMANAGER_H__
#define __GELIB_CHARACTER_CHARACTERDESCMANAGER_H__

#include <GELib/Utility/StringMap.h>
#include <GELib/Ext/BaseAssetManager.h>
#include "CharacterDesc.h"

namespace GELib
{


class CCharacterDescManager : public CBaseAssetManager
{
public:
    CCharacterDescManager();
    virtual ~CCharacterDescManager();

    CCharacterDesc* Load(const char *fname, const CLoadParameter &param);
    CCharacterDesc* Create(const char *name, const char *cacheKey=0);
    void Delete(CCharacterDesc *c);
    void DeleteLater(CCharacterDesc *c);

    typedef StringMap<CCharacterDesc*> CharacterDescCache;
	CharacterDescCache& GetCharacterDescs() { return m_Cache; }

private:

    CharacterDescCache m_Cache;
};

void SetCharacterDescManager(CCharacterDescManager *manager);
CCharacterDescManager* GetCharacterDescManager();



}//GELib

#endif//__GELIB_CHARACTER_CHARACTERDESCMANAGER_H__
