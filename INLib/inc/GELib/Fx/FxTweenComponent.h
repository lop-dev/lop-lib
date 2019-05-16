//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxTweenComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXTWEENCOMPONENT_H__
#define __GELIB_FX_FXTWEENCOMPONENT_H__

#include "FxComponent.h"

namespace GELib
{


class CFxTweenComponentResource : public CFxComponentResource
{
public:
    CFxTweenComponentResource(CFxAsset *fxRes);
    virtual ~CFxTweenComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);

    virtual int GetClassId() { return FXCLASS_TWEEN; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    std::string m_DummyName[2];
	EFxAttachTime m_DummyAttachTime[2];

private:

};


class CFxTweenComponent : public CFxComponent
{
public:
    CFxTweenComponent(CFx *fx, CFxTweenComponentResource *res);
    virtual ~CFxTweenComponent();

    virtual void SetRelatedComponents();

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

	virtual void GetMatrix(CMatrix &matrix) { matrix = m_TweenMatrix; }		// implement IAttachPoint

    CFxTweenComponentResource* TweenResource() { return (CFxTweenComponentResource*)m_pResource; }

private:

	void UpdateTween(float t);
    IAttachPoint *m_pDummy[2];
	CVector3 m_Line[2];
	float m_fDummyDetachTime[2];
	CMatrix m_TweenMatrix;
};


}//GELib

#endif//__GELIB_FX_FXTWEENCOMPONENT_H__
