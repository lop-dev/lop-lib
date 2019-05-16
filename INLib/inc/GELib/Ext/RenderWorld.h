//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/RenderWorld.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_RENDERWORLD_H__
#define __GELIB_EXT_RENDERWORLD_H__

#include <vector>

#include "BaseRenderObject.h"

namespace GELib
{

class CStdVisibility;
class CRenderer;

class CTextureAsset;
class CMaterialAsset;

class CRenderWorld
{
public:
    CRenderWorld();
    virtual ~CRenderWorld();

    void Initialize(const CAABox &worldBound, const CVector3 &gridSize);
    void Uninitialize();

    // remove immediately, but delete in SwapRenderBuffer (render thread is not working)
    void DeleteLater(CBaseRenderObject *obj);
    void DeleteLater(CBaseRenderLight *obj);

	void PostUpdate(float t);
	float GetTime() { return m_fTime; }

    // delete waiting objects
    void DeleteWaitObjects();

    CStdVisibility* GetVisibility() { return m_pVisibility; }

private:

	float m_fTime;
    CStdVisibility *m_pVisibility;
    std::vector<CBaseRenderObject*> m_WaitDeleteObjects;
    std::vector<CBaseRenderLight*> m_WaitDeleteLights;
};


}//GELib

#endif//__GELIB_EXT_RENDERWORLD_H__
