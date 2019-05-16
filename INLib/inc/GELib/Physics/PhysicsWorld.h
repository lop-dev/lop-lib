//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Physics/PhysicsWorld.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_PHYSICS_PHYSICSWORLD_H__
#define __GELIB_PHYSICS_PHYSICSWORLD_H__

#include <vector>
#include <GELib/Math/GeMath.h>
#include "CollisionResult.h"

namespace GELib
{

class IPlotter3D;

class CPhysicsSdk;
class CPhysicsObject;
class CPhysicsJoint;
class CPhysicsJointDesc;
class CPhysicsMaterial;
class CPhysicsMaterialDesc;
class CPhysicsWorldImpl;


class CPhysicsWorld
{
public:
    CPhysicsWorld();
    virtual ~CPhysicsWorld();

    void Initialize(CPhysicsSdk *sdk);
    void Uninitialize();

	CPhysicsSdk* GetSdk() { return m_pSdk; }

    void Simulate(float t);
    void FetchResults();

    void SetGravity(const CVector3 &g);

    void EnableGroupCollision(int group1, int group2, bool enable);

    void Draw(IPlotter3D &plotter);

    bool RayCast(const CVector3 &orig, const CVector3 &dir, CCollisionResult *hit=0, int mask=0xffffffff);
    bool LineCast(const CVector3 &p1, const CVector3 &p2, CCollisionResult *hit=0, int mask=0xffffffff);
    int RayOverlap(const CVector3 &orig, const CVector3 &dir, CCollisionResultList *hits=0, int mask=0xffffffff, bool hitObjectOnce=true);
    int LineOverlap(const CVector3 &p1, const CVector3 &p2, CCollisionResultList *hits=0, int mask=0xffffffff, bool hitObjectOnce=true);

    bool BoxSweep(const CAABox &box, const CVector3 &motion, CCollisionResult *hit=0, int mask=0xffffffff);
    bool BoxSweep(const COBox &box, const CVector3 &motion, CCollisionResult *hit=0, int mask=0xffffffff);
    bool SphereSweep(const CSphere &s, const CVector3 &motion, CCollisionResult *hit=0, int mask=0xffffffff);
    bool CapsuleSweep(const CVector3 &p1, const CVector3 &p2, float radius, const CVector3 &motion, CCollisionResult *hit=0, int mask=0xffffffff);

    // box matrix can't contains scale
    int BoxOverlap(const CAABox &box, CCollisionResultList *hits=0, int mask=0xffffffff, bool hitObjectOnce=true);
    int BoxOverlap(const COBox &box, CCollisionResultList *hits=0, int mask=0xffffffff, bool hitObjectOnce=true);
    int BoxOverlapBox(const CAABox &box, CCollisionResultList *hits=0, int mask=0xffffffff, bool hitObjectOnce=true);
    int SphereOverlap(const CSphere &s, CCollisionResultList *hits=0, int mask=0xffffffff, bool hitObjectOnce=true);
    int CapsuleOverlap(const CVector3 &p1, const CVector3 &p2, float radius, CCollisionResultList *hits=0, int mask=0xffffffff, bool hitObjectOnce=true);

    // joint
    CPhysicsJoint* CreateJoint(const CPhysicsJointDesc &desc);
    void DeleteJoint(CPhysicsJoint *joint);


    // material
    void SetDefaultMaterial(const CPhysicsMaterialDesc &desc);
    CPhysicsMaterial* CreateMaterial(const CPhysicsMaterialDesc &desc);
	int GetNumMaterials();
	CPhysicsMaterial* GetMaterialByIndex(int index);
	CPhysicsMaterial* GetMaterialById(int id);
    void DeleteMaterial(CPhysicsMaterial *m);
    void DeleteAllMaterials();

    // called by CPhysicsObject when static atctor is released
    void NotifyControllersSceneChanged();

	CPhysicsWorldImpl *m_pImpl;

private:


    CPhysicsSdk *m_pSdk;
    std::vector<CPhysicsMaterial*> m_Materials;
};


}//GELib

#endif//__GELIB_PHYSICS_PHYSICSWORLD_H__
