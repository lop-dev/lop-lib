//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Physics/PhysicsSdk.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_PHYSICS_PHYSICSSDK_H__
#define __GELIB_PHYSICS_PHYSICSSDK_H__

class NxPhysicsSDK;
class NxUserAllocator;

namespace GELib
{

class CPhysicsSdkImpl;

class CPhysicsSdk
{
public:
    CPhysicsSdk();
    virtual ~CPhysicsSdk();

    bool Initialize();
    void Uninitialize();

    void ConnectRemoteDebugger(const char *ip, int port);

	static int GetHeightFieldHoleMaterialId();

	CPhysicsSdkImpl *m_pImpl;

private:
};



}//GELib

#endif//__GELIB_PHYSICS_PHYSICSSDK_H__
