//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Physics/bullet/PhysicsImpl_bullet.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_PHYSICS_BULLET_PHYSICSIMPL_BULLET_H__
#define __GELIB_PHYSICS_BULLET_PHYSICSIMPL_BULLET_H__
#ifndef _LINUX
#ifndef GE_PHYSICS_PHYSICIMPL_BULLET_H
#define GE_PHYSICS_PHYSICIMPL_BULLET_H

#include "LinearMath/btTransform.h"
#include "LinearMath/btIDebugDraw.h"
#include <GELib/Math/GeMath.h>
#include <vector>

class btBroadphaseInterface;
class btCollisionDispatcher;
class btConstraintSolver;
class btDefaultCollisionConfiguration;
class btDynamicsWorld;
class btRigidBody;
struct btDefaultMotionState;
class btCollisionShape;

namespace ge
{

class IPlotter3D;

class CDebugDrawer;
class CPhysicsSdk;
class CPhysicsMaterial;

class CPhysicsSdkImpl
{
public:

	CPhysicsSdkImpl()
	{
		m_pDebugDrawer = 0;
	}

	CDebugDrawer *m_pDebugDrawer;
};

class CPhysicsObjectImpl
{
public:

	CPhysicsObjectImpl()
	{
		m_pActor = 0;
		m_pMotionState = 0;
		m_fMass = 0;
		m_Inertia = CVector3(0, 0, 0);
		m_iShapeDescId = 0;
		m_bEnableGravity = true;
	}

	class CShapeInfo
	{
	public:
		CShapeInfo()
		{
			m_iMask = 0;
			m_iGroup = 0;
			m_pShape = 0;
			m_pMaterial = 0;
		}

		int m_iMask;
		int m_iGroup;
		btCollisionShape *m_pShape;
		CPhysicsMaterial *m_pMaterial;
	};

	btRigidBody *m_pActor;
	btDefaultMotionState *m_pMotionState;
	float m_fMass;
	CVector3 m_Inertia;
	int m_iShapeDescId;
	bool m_bEnableGravity;
	std::vector<CShapeInfo> m_Shapes;
};

class CPhysicsWorldImpl
{
public:

	CPhysicsWorldImpl()
	{
		m_pBroadphase = 0;
		m_pDispatcher = 0;
		m_pSolver = 0;
		m_pCollisionConfiguration = 0;
		m_pWorld = 0;
	}

	btBroadphaseInterface *m_pBroadphase;
	btCollisionDispatcher *m_pDispatcher;
	btConstraintSolver	*m_pSolver;
	btDefaultCollisionConfiguration *m_pCollisionConfiguration;
	btDynamicsWorld *m_pWorld;
};

class CPhysicsJointImpl
{
public:

	CPhysicsJointImpl()
	{
	}
};

class CPhysicsMaterialImpl
{
public:

	CPhysicsMaterialImpl()
	{
	}

};

class CCollisionShapeImpl
{
public:

	CCollisionShapeImpl()
	{
		m_iCacheIdCounter = 0;
	}

	~CCollisionShapeImpl();

	CShapeDescCache* GetShapeDesc(int id);
    void DeleteShapeDesc(int id);

	class CShapeDescCache
	{
	public:
		void Release();
		CVector3 m_Scale;
		std::vector<btCollisionShape*> m_Descs;
		CPhysicsSdk *m_pSdk;
		int m_iRefCount;
		int m_iId;
	};

    int m_iCacheIdCounter;
    std::vector<CShapeDescCache> m_ShapeDescCache;
};

inline btVector3 CVector3ToBt(const ge::CVector3 &v)
{
    return btVector3(v.x, v.y, v.z);
}

inline ge::CVector3 BtToCVector3(const btVector3 &v)
{
	return ge::CVector3(v.m_floats[0], v.m_floats[1], v.m_floats[2]);
}

inline btTransform CMatrixToBt(const ge::CMatrix &m)
{
//    return btTransform(btMatrix3x3(m._00, m._01, m._02,
//								   m._10, m._11, m._12,
//								   m._20, m._21, m._22),
    return btTransform(btMatrix3x3(m._00, m._10, m._20,
								   m._01, m._11, m._21,
								   m._02, m._12, m._22),
						 btVector3(m._30, m._31, m._32));
}

inline ge::CMatrix BtToCMatrix(const btTransform &m)
{
	const btMatrix3x3 &r = m.getBasis();
	const btVector3 &p = m.getOrigin();
//    return ge::CMatrix(r[0].m_floats[0], r[0].m_floats[1], r[0].m_floats[2], 0,
//								 r[1].m_floats[0], r[1].m_floats[1], r[1].m_floats[2], 0,
//								 r[2].m_floats[0], r[2].m_floats[1], r[2].m_floats[2], 0,
    return ge::CMatrix(r[0].m_floats[0], r[1].m_floats[0], r[2].m_floats[0], 0,
								 r[0].m_floats[1], r[1].m_floats[1], r[2].m_floats[1], 0,
								 r[0].m_floats[2], r[1].m_floats[2], r[2].m_floats[2], 0,
								 p.m_floats[0], p.m_floats[1], p.m_floats[2], 1.0f);
}

inline btMatrix3x3 CMatrixToBtMatrix3x3(const ge::CMatrix &m)
{
//    return btMatrix3x3(m._00, m._01, m._02,
//					   m._10, m._11, m._12,
//					   m._20, m._21, m._22);
    return btMatrix3x3(m._00, m._10, m._20,
					   m._01, m._11, m._21,
					   m._02, m._12, m._22);
}

inline ge::CMatrix BtMatrix3x3ToCMatrix(const btMatrix3x3 &m)
{
//	return ge::CMatrix(m[0].m_floats[0], m[0].m_floats[1], m[0].m_floats[2], 0,
//								 m[1].m_floats[0], m[1].m_floats[1], m[1].m_floats[2], 0,
//								 m[2].m_floats[0], m[2].m_floats[1], m[2].m_floats[2], 0,
    return ge::CMatrix(m[0].m_floats[0], m[1].m_floats[0], m[2].m_floats[0], 0,
								 m[0].m_floats[1], m[1].m_floats[1], m[2].m_floats[1], 0,
								 m[0].m_floats[2], m[1].m_floats[2], m[2].m_floats[2], 0,
								 0, 0, 0, 1);
}


class CDebugDrawer : public btIDebugDraw
{
public:
	CDebugDrawer();
	virtual ~CDebugDrawer();
	virtual void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);
	virtual void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color);
	virtual void reportErrorWarning(const char *warningString);
	virtual void draw3dText(const btVector3 &location, const char *textString);
	virtual void setDebugMode(int debugMode) { m_iDebugMode = debugMode; }
	virtual int getDebugMode() const { return m_iDebugMode; }

	void SetPlotter(IPlotter3D *plotter) { m_pPlotter = plotter; }

private:
	int m_iDebugMode;
	IPlotter3D *m_pPlotter;
};


}//ge


#endif
#endif
#endif//__GELIB_PHYSICS_BULLET_PHYSICSIMPL_BULLET_H__
