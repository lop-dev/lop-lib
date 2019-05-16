//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Physics/physx3/PhysicsImpl_physx3.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_PHYSICS_PHYSX3_PHYSICSIMPL_PHYSX3_H__
#define __GELIB_PHYSICS_PHYSX3_PHYSICSIMPL_PHYSX3_H__

#include <vector>
#include <GELib/Math/GeMath.h>
#include <GELib/Physics/PhysicsMaterial.h>
#include "PxPhysicsAPI.h"



namespace GELib
{

class IPlotter3D;
class CPhysicsSdk;
class CShapeUserData;

class CPhysicsSdkImpl
{
public:

	CPhysicsSdkImpl()
	{
		m_pFundation = 0;
		m_pPhysics = 0;
		m_pCooking = 0;
		m_pDebuggerConnection = 0;
	}

	physx::PxFoundation *m_pFundation;
    physx::PxPhysics *m_pPhysics;
	physx::PxCooking *m_pCooking;
	PVD::PvdConnection *m_pDebuggerConnection;
	CPhysicsMaterial m_DefaultMaterial;
};

class CPhysicsObjectImpl
{
public:

	CPhysicsObjectImpl()
	{
		m_pActor = 0;
		m_pController = 0;
		m_fMass = 1.0f;
		m_iShapeDescId = 0;
	}

	physx::PxActor *m_pActor;
	physx::PxController *m_pController;
	float m_fMass;
    int m_iShapeDescId;
};

class CPhysicsWorldImpl
{
public:

	CPhysicsWorldImpl()
	{
		m_pScene = 0;
		m_pControllerManager = 0;
		m_pCpuDispatcher = 0;
		m_fAccumulator = 0;
		m_iMaterialIdCounter = 0;
		for (int i=0; i<32; ++i)
			m_GroupCollisionFlags[i] = 0xffffffff;
	}

    physx::PxScene *m_pScene;
    physx::PxControllerManager *m_pControllerManager;
	physx::PxDefaultCpuDispatcher *m_pCpuDispatcher;
	float m_fAccumulator;
	int m_iMaterialIdCounter;
	int m_GroupCollisionFlags[32];
};

class CPhysicsJointImpl
{
public:

	CPhysicsJointImpl()
	{
		m_pJoint = 0;
	}

	physx::PxJoint *m_pJoint;
};

class CPhysicsMaterialImpl
{
public:

	CPhysicsMaterialImpl()
	{
		m_pMaterial = 0;
	}

	physx::PxMaterial *m_pMaterial;
};

class CPxShapeDesc
{
public:
	CPxShapeDesc()
	{
		m_pGeometry = 0;
		userData = 0;
		m_bMultiMaterials = false;
	}

	physx::PxGeometry *m_pGeometry;
	physx::PxTransform m_LocalPose;
	CShapeUserData *userData;
	bool m_bMultiMaterials;
};

class CCollisionShapeImpl
{
public:

	CCollisionShapeImpl()
	{
		m_iCacheIdCounter = 0;
	}

	~CCollisionShapeImpl();

	class CShapeDescCache
	{
	public:
		void Release();
		CVector3 m_Scale;
		std::vector<CPxShapeDesc*> m_Descs;
		CPhysicsSdk *m_pSdk;
		int m_iRefCount;
		int m_iId;
	};

	CShapeDescCache* GetShapeDesc(int id);
    void DeleteShapeDesc(int id);

    int m_iCacheIdCounter;
    std::vector<CShapeDescCache> m_ShapeDescCache;
};

class MemoryOutputStream: public physx::PxOutputStream
{
public:
	MemoryOutputStream();
	virtual	~MemoryOutputStream();

	physx::PxU32 write(const void* src, physx::PxU32 count);

	physx::PxU32 getSize()	const	{	return mSize; }
	physx::PxU8* getData()	const	{	return mData; }
private:
	physx::PxU8* mData;
	physx::PxU32 mSize;
	physx::PxU32 mCapacity;
};

class MemoryInputData: public physx::PxInputData
{
public:
	MemoryInputData(physx::PxU8* data, physx::PxU32 length);

	physx::PxU32 read(void* dest, physx::PxU32 count);
	physx::PxU32 getLength() const;
	void seek(physx::PxU32 pos);
	physx::PxU32 tell() const;

private:
	physx::PxU32 mSize;
	const physx::PxU8* mData;
	physx::PxU32 mPos;
};

inline physx::PxVec3 CVector3ToPx(const CVector3 &v)
{
    return physx::PxVec3(v.x, v.y, v.z);
}

inline CVector3 PxToCVector3(const physx::PxVec3 &v)
{
    return CVector3(v.x, v.y, v.z);
}

inline physx::PxMat33 CMatrixToPxMat33(const CMatrix &m)
{
    physx::PxMat33 mat;
	mat.column0 = physx::PxVec3(m._00, m._01, m._02);
	mat.column1 = physx::PxVec3(m._10, m._11, m._12);
	mat.column2 = physx::PxVec3(m._20, m._21, m._22);

    return mat;
}

inline CMatrix PxToCMatrix(const physx::PxMat33 &m)
{
    CMatrix mat;
	mat._00 = m.column0.x;
	mat._01 = m.column0.y;
	mat._02 = m.column0.z;
	mat._10 = m.column1.x;
	mat._11 = m.column1.y;
	mat._12 = m.column1.z;
	mat._20 = m.column2.x;
	mat._21 = m.column2.y;
	mat._22 = m.column2.z;
    return mat;
}


inline physx::PxMat44 CMatrixToPx(const CMatrix &m)
{
	return physx::PxMat44((float*)m.m_m16);
}

inline CMatrix PxToCMatrix(const physx::PxMat44 &m)
{
    return CMatrix((const float*)&m);
}

inline physx::PxTransform CMatrixToPxTransform(const CMatrix &m)
{
	return physx::PxTransform(CMatrixToPx(m));
}

inline physx::PxBounds3 CAABoxToPx(const CAABox &b)
{
    physx::PxBounds3 box;
	box.minimum.x = b.m_Min.x;
	box.minimum.y = b.m_Min.y;
	box.minimum.z = b.m_Min.z;
	box.maximum.x = b.m_Max.x;
	box.maximum.y = b.m_Max.y;
	box.maximum.z = b.m_Max.z;
    return box;
}

inline CAABox PxToCAABox(const physx::PxBounds3 &b)
{
    CAABox box;
    box.m_Min.x = b.minimum.x;
    box.m_Min.y = b.minimum.y;
    box.m_Min.z = b.minimum.z;
    box.m_Max.x = b.maximum.x;
    box.m_Max.y = b.maximum.y;
    box.m_Max.z = b.maximum.z;
    return box;
}

void DrawPxActor(IPlotter3D &plotter, physx::PxActor *actor);


}//GELib


#endif//__GELIB_PHYSICS_PHYSX3_PHYSICSIMPL_PHYSX3_H__
