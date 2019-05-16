//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/AssetManagers.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_ASSETMANAGERS_H__
#define __GELIB_EXT_ASSETMANAGERS_H__

#include <string>
#include <set>

namespace GELib
{


class CRenderWorld;
class CPhysicsWorld;
class CAudioDevice;
class CFxPlayer;

class CAsyncLoader;
class CCollisionShapeManager;
class CFxManager;
class CSceneManager;
class CSkinMeshManager;
class CAnimationManager;
class CAnimationSetManager;
class CCharacterDescManager;
class CCharacterManager;
class CActManager;
class CTextureManager;
class CMaterialManager;
class CMeshManager;
class CTerrainManager;
class CNavMeshManager;
class CDynamicBufferManager;


class CAssetManagers
{
public:

    CAssetManagers();
    virtual ~CAssetManagers();

    void Initialize();
    void Uninitialize();

    void DeleteWaitObjects();
    void PostUpdate(float t);
    void SwapRenderBuffer();

    CTextureManager *m_pTextureManager;
    CMaterialManager *m_pMaterialManager;
    CMeshManager *m_pMeshManager;
    CTerrainManager *m_pTerrainManager;
    CNavMeshManager *m_pNavMeshManager;
    CDynamicBufferManager *m_pDynamicBufferManager;
    CSkinMeshManager *m_pSkinMeshManager;
    CAnimationManager *m_pAnimationManager;
    CAnimationSetManager *m_pAnimationSetManager;
    CCharacterDescManager *m_pCharacterDescManager;
    CCharacterManager *m_pCharacterManager;
    CCollisionShapeManager *m_pCollisionShapeManager;
    CFxManager *m_pFxManager;
    CSceneManager *m_pSceneManager;
    CActManager *m_pActManager;
	CAsyncLoader *m_pAsyncLoader;

private:

};


}//GELib

#endif//__GELIB_EXT_ASSETMANAGERS_H__
