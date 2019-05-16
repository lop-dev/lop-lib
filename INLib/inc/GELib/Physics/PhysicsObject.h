//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Physics/PhysicsObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_PHYSICS_PHYSICSOBJECT_H__
#define __GELIB_PHYSICS_PHYSICSOBJECT_H__

#include <GELib/Math/GeMath.h>
#include "CollisionResult.h"
#include <vector>

namespace GELib
{

class IPlotter3D;
class CLoadParameter;


class CPhysicsSdk;
class CPhysicsWorld;
class CCollisionShape;
class CCollisionShapeElement;
class CCollisionResult;
class CPhysicsMaterial;
class CPhysicsObject;
class CPhysicsObjectImpl;

class CControllerDesc
{
public:
    CControllerDesc();
    void SetBox(const CVector3 &extents);
    void SetCapsule(float height, float radius);
    void SetExtraBox(const CVector3 &extents, const CVector3 &offset);

    int m_UpAxis;       // 0:X, 1:Y, 2:Z
    float m_fSlopeLimit;
    float m_fSkinWidth;
    float m_fStepHeight;
    CVector3 m_BoxExtents;
    float m_fCapsuleHeight;
    float m_fCapsuleRadius;
    CVector3 m_ExtraBoxExtents;
    CVector3 m_ExtraBoxOffset;
    bool m_bUseExtraBox;
};

const int CONTROLLER_COLLIDE_SIDES = 1 << 0;
const int CONTROLLER_COLLIDE_UP =    1 << 1;
const int CONTROLLER_COLLIDE_DOWN =  1 << 2;

enum EPhysicsForceMode
{
    FORCEMODE_FORCE,
    FORCEMODE_IMPULSE,
    FORCEMODE_VELOCITY_CHANGE,
    FORCEMODE_ACCELERATION
};

class CPhysicsObject
{
public:

    CPhysicsObject();
    virtual ~CPhysicsObject();

    // collision shape must be setup before AddToPhysicsWorld()
    // all other properties can only be used after AddToPhysicsWorld()
    bool LoadCollisionShape(const char *fname, const CLoadParameter &param);
    bool IsLoading();
    CCollisionShape* CreateCollisionShape(const char *name="");
    void SetCollisionShape(CCollisionShape *shape);
    CCollisionShape* GetCollisionShape() { return m_pCollisionShape; }

	void CreateCollisionShapeResource(CPhysicsSdk *sdk);

    void AddToPhysicsWorld(CPhysicsWorld *world, bool dynamic);
    CPhysicsWorld* GetPhysiscWorld() { return m_pWorld; }
    bool IsDynamic();

    // shape
    int GetNumShapes();
    bool GetShapeTriangle(int shapeIndex, unsigned int triangleId, CTriangle &tri);
    CCollisionShapeElement* GetCollisionShapeElement(int shapeIndex);

    void SetShapeGroup(int shapeIndex, int group);
    int  GetShapeGroup(int shapeIndex);
    void SetShapeMask(int shapeIndex, int mask);
    int  GetShapeMask(int shapeIndex);
    void SetShapeMaterial(int shapeIndex, CPhysicsMaterial *material);
    void GetShapeBoundingBox(int shapeIndex, CAABox &box);
    void GetBoundingBox(CAABox &box);

    void EnableCollision(bool enable);
    bool IsEnableCollision();

    // matrix contains no scale
    void SetMatrix(const CMatrix &matrix);
    void GetMatrix(CMatrix &matrix);
    const CMatrix& GetMatrix();
    void SetPosition(const CVector3 &p);
    void GetPosition(CVector3 &p);
    const CVector3& GetPosition();
    void SetScale(const CVector3 &scale);
    void GetScale(CVector3 &scale);
    const CVector3& GetScale();

    // kinematic move
    void SetKinematic(bool on);
    bool IsKinematic();
	void MoveTo(const CMatrix &matrix);
	void MoveTo(const CVector3 &p);

    // gravity
    void EnableGravity(bool enable);
    bool IsEnableGravity();

    // mass
    void SetMass(float mass);
    float GetMass();
    void ComputeMass(float density);    // can called after AddToPhysicsWorld()

    // damping
    void SetLinearDamping(float d);
    float GetLinearDamping();
    void SetAngularDamping(float d);
    float GetAngularDamping();

    // velocity
    void SetLinearVelocity(const CVector3 &vel);
    void GetLinearVelocity(CVector3 &vel);
    void SetAngularVelocity(const CVector3 &vel);
    void GetAngularVelocity(CVector3 &vel);

    // force, torque
    void AddForce(const CVector3 &force, EPhysicsForceMode mode=FORCEMODE_FORCE);
    void AddForceAtPosition(const CVector3 &force, const CVector3 &pos, EPhysicsForceMode mode=FORCEMODE_FORCE);
    void AddForceAtLocalPosition(const CVector3 &force, const CVector3 &pos, EPhysicsForceMode mode=FORCEMODE_FORCE);
    void AddLocalForce(const CVector3 &force, EPhysicsForceMode mode=FORCEMODE_FORCE);
    void AddLocalForceAtPosition(const CVector3 &force, const CVector3 &pos, EPhysicsForceMode mode=FORCEMODE_FORCE);
    void AddLocalForceAtLocalPosition(const CVector3&force, const CVector3&pos, EPhysicsForceMode mode=FORCEMODE_FORCE);

    void AddTorque(const CVector3 &torque, EPhysicsForceMode mode=FORCEMODE_FORCE);
    void AddLocalTorque(const CVector3 &torque, EPhysicsForceMode mode=FORCEMODE_FORCE);

    // sleep
    bool IsSleeping();
    void Sleep();
    void WakeUp();

    // character controller
    void AddToPhysicsWorld(CPhysicsWorld *world, const CControllerDesc &desc);
    bool IsController();
    // return collide flags (CONTROLLER_COLLIDE_XXX)
    int MoveController(const CVector3 &move, float minDist, CCollisionResultList *hits=0, int mask=0xffffffff, bool hitObjectOnce=true);

    // ray cast testing
    bool RayCast(const CVector3 &orig, const CVector3 &dir, CCollisionResult *hit=0, int mask=0xffffffff);
    bool LineCast(const CVector3 &p1, const CVector3 &p2, CCollisionResult *hit=0, int mask=0xffffffff);

    // overlap testing (box matrix can't contains scale);
    int BoxOverlap(const CAABox &box, CCollisionResultList *hits=0, int mask=0xffffffff);
    int BoxOverlap(const COBox &box, CCollisionResultList *hits=0, int mask=0xffffffff);
    int SphereOverlap(const CSphere &s, CCollisionResultList *hits=0, int mask=0xffffffff);
    int CapsuleOverlap(const CVector3 &p1, const CVector3 &p2, float radius, CCollisionResultList *hits=0, int mask=0xffffffff);

    void Draw(IPlotter3D &plotter, int flag);

    int m_iUserData;
    const void *m_pUserData;

	CPhysicsObjectImpl *m_pImpl;

private:

    void DeleteActorAndShapeImpl();

    CMatrix m_Matrix;
    CVector3 m_Scale;
    CCollisionShape *m_pCollisionShape;
    CPhysicsWorld *m_pWorld;
    bool m_bCollision;
};


}//GELib

#endif//__GELIB_PHYSICS_PHYSICSOBJECT_H__
