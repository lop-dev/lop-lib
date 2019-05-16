//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxCollisionComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXCOLLISIONCOMPONENT_H__
#define __GELIB_FX_FXCOLLISIONCOMPONENT_H__

#include "FxComponent.h"

namespace GELib
{

class CCollisionShape;
class CPhysicsObject;

class CFxCollisionComponentResource : public CFxComponentResource
{
public:
    CFxCollisionComponentResource(CFxAsset *fxRes);
    virtual ~CFxCollisionComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);

    virtual int GetClassId() { return FXCLASS_COLLISION; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    virtual void GetAssets(CAssetHelper &helper);

    bool LoadCollisionShape(const char *fname, const CLoadParameter &param);
    void SetCollisionShape(CCollisionShape *col);
    CCollisionShape* GetCollisionShape();
    void DeleteCollisionShape();

	CVector3 m_Scale;
    int m_iCollisionMask;
    short m_iPhysicsMaterial;

private:

    CCollisionShape *m_pCollisionShape;
};


class CFxCollisionComponent : public CFxComponent
{
public:
    CFxCollisionComponent(CFx *fx, CFxCollisionComponentResource *res);
    virtual ~CFxCollisionComponent();

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    CFxCollisionComponentResource* CollisionResource() { return (CFxCollisionComponentResource*)m_pResource; }

private:

    void UpdatePhysicsObject();
    CPhysicsObject *m_pPhysicsObject;
};


}//GELib

#endif//__GELIB_FX_FXCOLLISIONCOMPONENT_H__
