//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Physics/physx/PhysicsImpl_physx.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_PHYSICS_PHYSX_PHYSICSIMPL_PHYSX_H__
#define __GELIB_PHYSICS_PHYSX_PHYSICSIMPL_PHYSX_H__
#ifndef _LINUX
#ifndef GELIB_PHYSICIMPL_PHYSX_H
#define GELIB_PHYSICIMPL_PHYSX_H

#include <vector>
#include <GELib/Math/GeMath.h>
#include <GELib/Physics/PhysicsMaterial.h>
#include "NxPhysics.h"
#include "NxStream.h"

class NxPhysicsSDK;
class NxUserAllocator;
class NxActor;
class NxController;
class NxScene;
class NxDebugRenderable;
class NxControllerManager;
class NxJoint;
class NxMaterial;
class NxShapeDesc;

namespace GELib
{

class IPlotter3D;
class CPhysicsSdk;

class CPhysicsSdkImpl
{
public:

	CPhysicsSdkImpl()
	{
		m_pSdk = 0;
		m_pAllocator = 0;
	}

    NxPhysicsSDK *m_pSdk;
    NxUserAllocator *m_pAllocator;
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

    NxActor *m_pActor;
    NxController *m_pController;
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
	}

    NxScene *m_pScene;
    NxControllerManager *m_pControllerManager;
	CPhysicsMaterial m_DefaultMaterial;
};

class CPhysicsJointImpl
{
public:

	CPhysicsJointImpl()
	{
		m_pJoint = 0;
	}

    NxJoint *m_pJoint;
};

class CPhysicsMaterialImpl
{
public:

	CPhysicsMaterialImpl()
	{
		m_pMaterial = 0;
	}

    NxMaterial *m_pMaterial;
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
		std::vector<NxShapeDesc*> m_Descs;
		CPhysicsSdk *m_pSdk;
		int m_iRefCount;
		int m_iId;
	};

	CShapeDescCache* GetShapeDesc(int id);
    void DeleteShapeDesc(int id);

    int m_iCacheIdCounter;
    std::vector<CShapeDescCache> m_ShapeDescCache;
};


class NxMemoryWriteBuffer : public ::NxStream
{
	public:
								NxMemoryWriteBuffer();
	virtual						~NxMemoryWriteBuffer();

	virtual		NxU8			readByte()								const	{ NX_ASSERT(0);	return 0;	}
	virtual		NxU16			readWord()								const	{ NX_ASSERT(0);	return 0;	}
	virtual		NxU32			readDword()								const	{ NX_ASSERT(0);	return 0;	}
	virtual		float			readFloat()								const	{ NX_ASSERT(0);	return 0.0f;}
	virtual		double			readDouble()							const	{ NX_ASSERT(0);	return 0.0;	}
	virtual		void			readBuffer(void* buffer, NxU32 size)	const	{ NX_ASSERT(0);				}

	virtual		NxStream&		storeByte(NxU8 b);
	virtual		NxStream&		storeWord(NxU16 w);
	virtual		NxStream&		storeDword(NxU32 d);
	virtual		NxStream&		storeFloat(NxReal f);
	virtual		NxStream&		storeDouble(NxF64 f);
	virtual		NxStream&		storeBuffer(const void* buffer, NxU32 size);

				NxU32			currentSize;
				NxU32			maxSize;
				NxU8*			data;
};

class NxMemoryReadBuffer : public ::NxStream
{
	public:
								NxMemoryReadBuffer(const NxU8* data);
	virtual						~NxMemoryReadBuffer();

	virtual		NxU8			readByte()								const;
	virtual		NxU16			readWord()								const;
	virtual		NxU32			readDword()								const;
	virtual		float			readFloat()								const;
	virtual		double			readDouble()							const;
	virtual		void			readBuffer(void* buffer, NxU32 size)	const;

	virtual		NxStream&		storeByte(NxU8 b)							{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeWord(NxU16 w)							{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeDword(NxU32 d)							{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeFloat(NxReal f)						{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeDouble(NxF64 f)						{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeBuffer(const void* buffer, NxU32 size)	{ NX_ASSERT(0);	return *this;	}

	mutable		const NxU8*		buffer;
};


inline NxVec3 CVector3ToNx(const CVector3 &v)
{
    return NxVec3(v.x, v.y, v.z);
}

inline CVector3 NxToCVector3(const NxVec3 &v)
{
    return CVector3(v.x, v.y, v.z);
}

inline NxMat34 CMatrixToNx(const CMatrix &m)
{
    NxMat34 mat(false);
    mat.setColumnMajor44(m.m_m16);
    return mat;
}

inline NxMat33 CMatrixToNxMat33(const CMatrix &m)
{
    NxMat33 mat;
    mat.setColumnMajorStride4(m.m_m16);
    return mat;
}

inline CMatrix NxToCMatrix(const NxMat34 &m)
{
    CMatrix mat;
    m.getColumnMajor44(mat.m_m16);
    return mat;
}

inline CMatrix NxToCMatrix(const NxMat33 &m)
{
    CMatrix mat;
    m.getColumnMajorStride4(mat.m_m16);
    return mat;
}

inline NxBounds3 CAABoxToNx(const CAABox &b)
{
    NxBounds3 box;
	box.min.x = b.m_Min.x;
	box.min.y = b.m_Min.y;
	box.min.z = b.m_Min.z;
	box.max.x = b.m_Max.x;
	box.max.y = b.m_Max.y;
	box.max.z = b.m_Max.z;
    return box;
}

inline CAABox NxToCAABox(const NxBounds3 &b)
{
    CAABox box;
    box.m_Min.x = b.min.x;
    box.m_Min.y = b.min.y;
    box.m_Min.z = b.min.z;
    box.m_Max.x = b.max.x;
    box.m_Max.y = b.max.y;
    box.m_Max.z = b.max.z;
    return box;
}

void DrawNxActor(IPlotter3D &plotter, NxActor *actor);



}//GELib


#endif
#endif
#endif//__GELIB_PHYSICS_PHYSX_PHYSICSIMPL_PHYSX_H__
