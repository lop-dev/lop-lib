//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/CharacterManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_CHARACTERMANAGER_H__
#define __GELIB_CHARACTER_CHARACTERMANAGER_H__

#include <string>
#include <map>
#include <GELib/Ext/BaseAssetManager.h>
#include "Character.h"


namespace GELib
{

class CRenderWorld;
class CPhysicsWorld;
class CFxPlayer;

class CCharacterManager : public CBaseAssetManager
{
public:
    CCharacterManager();
    virtual ~CCharacterManager();

    void SetWorld(CRenderWorld *renderWorld, CPhysicsWorld *physicsWorld, CFxPlayer *fxPlayer);
	CRenderWorld* GetRenderWorld() { return  m_pRenderWorld; }
	CPhysicsWorld* GetPhysicsWorld() { return  m_pPhysicsWorld; }
	CFxPlayer* GetFxPlayer() { return  m_pFxPlayer; }

    CCharacter* Load(const char *fname, const CLoadParameter &param);
    CCharacter* Load(const char *fname, const CLoadParameter &param, CRenderWorld *renderWorld, CPhysicsWorld *physicsWorld, CFxPlayer *fxPlayer);
	void Delete(CCharacter *c);
    void DeleteLater(CCharacter *c);

    virtual void DeleteWaitObjects();

private:

	void ForceDeleteWaitCharacters();
	CircularBuffer<CCharacter*> m_WaitDeleteCharacters;

	CRenderWorld *m_pRenderWorld;
	CPhysicsWorld *m_pPhysicsWorld;
	CFxPlayer *m_pFxPlayer;
};

void SetCharacterManager(CCharacterManager *manager);
CCharacterManager* GetCharacterManager();



}//GELib

#endif//__GELIB_CHARACTER_CHARACTERMANAGER_H__
