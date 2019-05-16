//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Physics/PhysicsJoint.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_PHYSICS_PHYSICSJOINT_H__
#define __GELIB_PHYSICS_PHYSICSJOINT_H__

#include <GELib/Math/GeMath.h>


namespace GELib
{


class CPhysicsWorld;
class CPhysicsObject;
class CPhysicsJointImpl;

enum EPhysicsJointType
{
    JOINT_NONE = 0,
    JOINT_SPHERICAL,
    JOINT_REVOLUTE,
    JOINT_PRISMATIC,
    JOINT_CYLINDRICAL,
    JOINT_FIXED,
    JOINT_DISTANCE,
    JOINT_POINT_IN_PLANE,
    JOINT_POINT_ON_LINE,
    JOINT_PULLEY,
    JOINT_D6,
};

class CPhysicsJointDesc
{
public:
    CPhysicsJointDesc();
    virtual ~CPhysicsJointDesc();

    EPhysicsJointType GetType() const { return m_Type; }

    CPhysicsObject *m_pObject[2];
    CVector3 m_LocalNormal[2];
    CVector3 m_LocalAxis[2];
    CVector3 m_LocalAnchor[2];

protected:
    EPhysicsJointType m_Type;
};

class CPhysicsSphericalJointDesc : public CPhysicsJointDesc
{
public:
    CPhysicsSphericalJointDesc();
};

class CPhysicsRevoluteJointDesc : public CPhysicsJointDesc
{
public:
    CPhysicsRevoluteJointDesc();
};

class CPhysicsPrismaticJointDesc : public CPhysicsJointDesc
{
public:
    CPhysicsPrismaticJointDesc();
};

class CPhysicsCylindricalJointDesc : public CPhysicsJointDesc
{
public:
    CPhysicsCylindricalJointDesc();
};

class CPhysicsFixedJointDesc : public CPhysicsJointDesc
{
public:
    CPhysicsFixedJointDesc();
};

class CPhysicsDistanceJointDesc : public CPhysicsJointDesc
{
public:
    CPhysicsDistanceJointDesc();
};

class CPhysicsPointInPlaneJointDesc : public CPhysicsJointDesc
{
public:
    CPhysicsPointInPlaneJointDesc();
};

class CPhysicsPointOnLineJointDesc : public CPhysicsJointDesc
{
public:
    CPhysicsPointOnLineJointDesc();
};

class CPhysicsPulleyJointDesc : public CPhysicsJointDesc
{
public:
    CPhysicsPulleyJointDesc();
};

class CPhysicsD6JointDesc : public CPhysicsJointDesc
{
public:
    CPhysicsD6JointDesc();
};


class CPhysicsJoint
{
public:

    friend class CPhysicsWorld;

    CPhysicsJoint();
    virtual ~CPhysicsJoint();

    EPhysicsJointType GetType() const { return m_Type; }

    CPhysicsWorld* GetPhysiscWorld() { return m_pWorld; }

    CPhysicsObject* GetObject(int index);

    void SetBreakable(float maxForce, float maxTorque);
    bool IsBroken();

    int m_iUserData;
    const void *m_pUserData;

	CPhysicsJointImpl *m_pImpl;

protected:

    EPhysicsJointType m_Type;
    CPhysicsWorld *m_pWorld;
    CPhysicsObject *m_pObject[2];
};


class CPhysicsSphericalJoint : public CPhysicsJoint
{
public:
    CPhysicsSphericalJoint();
};

class CPhysicsRevoluteJoint : public CPhysicsJoint
{
public:
    CPhysicsRevoluteJoint();
};

class CPhysicsPrismaticJoint : public CPhysicsJoint
{
public:
    CPhysicsPrismaticJoint();
};

class CPhysicsCylindricalJoint : public CPhysicsJoint
{
public:
    CPhysicsCylindricalJoint();
};

class CPhysicsFixedJoint : public CPhysicsJoint
{
public:
    CPhysicsFixedJoint();
};

class CPhysicsDistanceJoint : public CPhysicsJoint
{
public:
    CPhysicsDistanceJoint();
};

class CPhysicsPointInPlaneJoint : public CPhysicsJoint
{
public:
    CPhysicsPointInPlaneJoint();
};

class CPhysicsPointOnLineJoint : public CPhysicsJoint
{
public:
    CPhysicsPointOnLineJoint();
};

class CPhysicsPulleyJoint : public CPhysicsJoint
{
public:
    CPhysicsPulleyJoint();
};


class CPhysicsD6Joint : public CPhysicsJoint
{
public:
    CPhysicsD6Joint();
};



}//GELib
#endif//__GELIB_PHYSICS_PHYSICSJOINT_H__
