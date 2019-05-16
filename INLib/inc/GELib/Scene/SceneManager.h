//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Scene/SceneManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_SCENE_SCENEMANAGER_H__
#define __GELIB_SCENE_SCENEMANAGER_H__

#include <GELib/Utility/StringMap.h>
#include <GELib/Ext/BaseAssetManager.h>


namespace GELib
{

class CPhysicsWorld;
class CFxPlayer;
class CAudioDevice;
class CRenderWorld;
class CLoadParameter;

class CScene;

class CSceneManager : public CBaseAssetManager
{
public:
    CSceneManager();
    virtual ~CSceneManager();

    void SetWorld(CRenderWorld *renderWorld, CPhysicsWorld *physicsWorld, CFxPlayer *fxPlayer);
	CRenderWorld* GetRenderWorld() { return  m_pRenderWorld; }
	CPhysicsWorld* GetPhysicsWorld() { return  m_pPhysicsWorld; }
	CFxPlayer* GetFxPlayer() { return  m_pFxPlayer; }

    CScene* Load(const char *fname, const CLoadParameter &param);
    CScene* Load(const char *fname, const CLoadParameter &param, CRenderWorld *renderWorld, CPhysicsWorld *physicsWorld, CFxPlayer *fxPlayer);
    CScene* Preload(const char *fname, const CLoadParameter &param);
    CScene* Preload(const char *fname, const CLoadParameter &param, CRenderWorld *renderWorld, CPhysicsWorld *physicsWorld, CFxPlayer *fxPlayer);	// load but do not add to render world, physics world

    CScene* Create(const char *name, const char *cacheKey=0);
    void Delete(CScene *scene);
    void DeleteLater(CScene *material);

    typedef StringMap<CScene*> SceneCache;
	SceneCache& GetScenes() { return m_Cache; }

private:

    CScene* _Load(const char *fname, const CLoadParameter &param, bool preload, CRenderWorld *renderWorld, CPhysicsWorld *physicsWorld, CFxPlayer *fxPlayer);

    CRenderWorld *m_pRenderWorld;
    CPhysicsWorld *m_pPhysicsWorld;
    CFxPlayer *m_pFxPlayer;

    SceneCache m_Cache;
};

void SetSceneManager(CSceneManager *manager);
CSceneManager* GetSceneManager();



}//GELib

#endif//__GELIB_SCENE_SCENEMANAGER_H__
