//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxDirectorComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXDIRECTORCOMPONENT_H__
#define __GELIB_FX_FXDIRECTORCOMPONENT_H__

#include "FxComponent.h"

namespace GELib
{


class CFxDirectorComponentResource : public CFxComponentResource
{
public:
    CFxDirectorComponentResource(CFxAsset *fxRes);
    virtual ~CFxDirectorComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);

    virtual int GetClassId() { return FXCLASS_DIRECTOR; }
    virtual CFxComponent* CreateInstance(CFx *fx);

	int m_iDirectorType;
    std::string m_DirectorParam;

private:

};


class CFxDirectorComponent : public CFxComponent
{
public:
    CFxDirectorComponent(CFx *fx, CFxDirectorComponentResource *res);
    virtual ~CFxDirectorComponent();

    virtual void SetTime(float t);
	virtual void DrawSelected(IPlotter3D &plotter, IPlotter2D &canvas);

	float GetSpeed();
	CFxComponent* GetCurrentCameraComponent();

    CFxDirectorComponentResource* DirectorResource() { return (CFxDirectorComponentResource*)m_pResource; }

private:

};


}//GELib

#endif//__GELIB_FX_FXDIRECTORCOMPONENT_H__
