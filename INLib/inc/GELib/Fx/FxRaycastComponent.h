//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxRaycastComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXRAYCASTCOMPONENT_H__
#define __GELIB_FX_FXRAYCASTCOMPONENT_H__

#include "FxComponent.h"

namespace GELib
{


class CFxRaycastComponentResource : public CFxComponentResource
{
public:
    CFxRaycastComponentResource(CFxAsset *fxRes);
    virtual ~CFxRaycastComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);

    virtual int GetClassId() { return FXCLASS_RAYCAST; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    std::string m_TargetName;
	bool m_bAlignNormal;
	float m_fDistance;
	float m_fNotHitDistance;

private:

};


class CFxRaycastComponent : public CFxComponent
{
public:
    CFxRaycastComponent(CFx *fx, CFxRaycastComponentResource *res);
    virtual ~CFxRaycastComponent();

    virtual void SetRelatedComponents();

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    virtual void GetMatrix(CMatrix &matrix) { matrix = m_HitMatrix; }		// implement IAttachPoint

    CFxRaycastComponentResource* RaycastResource() { return (CFxRaycastComponentResource*)m_pResource; }

private:

	void UpdateRay(float t);
    IAttachPoint *m_pTarget;
	CVector3 m_Line[2];
	CMatrix m_HitMatrix;
};


}//GELib

#endif//__GELIB_FX_FXRAYCASTCOMPONENT_H__
