//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Physics/CollisionResult.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_PHYSICS_COLLISIONRESULT_H__
#define __GELIB_PHYSICS_COLLISIONRESULT_H__

#include <vector>
#include <GELib/Math/GeMath.h>


namespace GELib
{

class CPhysicsObject;

class CCollisionResult
{
public:
    CCollisionResult() { t=0; object=0; shapeIndex = 0; materialId = 0; triangleId = 0; length = 0; }
    CVector3 point;
    CVector3 normal;
    CPhysicsObject *object;
    unsigned short shapeIndex;
	unsigned short materialId;
	unsigned int triangleId;
    float t;                    // not useful in MoveController

    // MoveController only
    CVector3 dir;
    float length;
};

class CCollisionResultList
{
public:
    void Clear() { m_CollisionResults.clear(); }
    int Size() { return (int)m_CollisionResults.size(); }
    void Resize(int s) { m_CollisionResults.resize(s); }
    CCollisionResult& operator[](int i) { return m_CollisionResults[i]; }
    CCollisionResult& Back() { return m_CollisionResults.back(); }

public:
    std::vector<CCollisionResult> m_CollisionResults;
};


}//GELib

#endif//__GELIB_PHYSICS_COLLISIONRESULT_H__
