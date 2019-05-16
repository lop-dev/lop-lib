//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Physics/PhysicsMaterial.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_PHYSICS_PHYSICSMATERIAL_H__
#define __GELIB_PHYSICS_PHYSICSMATERIAL_H__

#include <vector>


namespace GELib
{

class CPhysicsMaterialImpl;

class CPhysicsMaterialDesc
{
public:
    CPhysicsMaterialDesc();
    float m_fDynamicFriction;
    float m_fStaticFriction;
    float m_fRestitution;
};

class CPhysicsMaterial
{
public:
    CPhysicsMaterial();
    virtual ~CPhysicsMaterial();

	int GetId() { return m_iId; }

	int m_iId;
	CPhysicsMaterialImpl *m_pImpl;
};


}//GELib

#endif//__GELIB_PHYSICS_PHYSICSMATERIAL_H__
