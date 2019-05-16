//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Physics/CollisionShapeManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_PHYSICS_COLLISIONSHAPEMANAGER_H__
#define __GELIB_PHYSICS_COLLISIONSHAPEMANAGER_H__

#include <map>
#include "CollisionShape.h"
#include <GELib/Utility/StringMap.h>
#include <GELib/Ext/BaseAssetManager.h>


namespace GELib
{

class CColladaFile;

class CCollisionShapeManager : public CBaseAssetManager
{
public:
    CCollisionShapeManager();
    virtual ~CCollisionShapeManager();

    bool Save(const char *fname, CCollisionShape *shape);
    CCollisionShape* Load(const char *fname, const CLoadParameter &param);
    CCollisionShape* Import(const char *fname);
    CCollisionShape* Import(const char *fname, CColladaFile *collada, const char *nodeName, const char *name);

    CCollisionShape* Create(const char *name, const char *cacheKey=0);
    void Delete(CCollisionShape *shape);
    void DeleteLater(CCollisionShape *shape);

    typedef StringMap<CCollisionShape*> CollisionShapeCache;
	CollisionShapeCache& GetCollisionShapes() { return m_Cache; }

private:
    bool ColladaToCollisionShape(CColladaFile *collada, const char *nodeName, CCollisionShape *colShape);

    CollisionShapeCache m_Cache;
};


void SetCollisionShapeManager(CCollisionShapeManager *manager);
CCollisionShapeManager* GetCollisionShapeManager();


}//GELib

#endif//__GELIB_PHYSICS_COLLISIONSHAPEMANAGER_H__
