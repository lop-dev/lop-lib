//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxActComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXACTCOMPONENT_H__
#define __GELIB_FX_FXACTCOMPONENT_H__

#include "FxComponent.h"

namespace GELib
{

class CActAsset;
class CActPlayer;


class CFxActComponentResource : public CFxComponentResource
{
public:
    CFxActComponentResource(CFxAsset *fxRes);
    virtual ~CFxActComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();

    virtual int GetClassId() { return FXCLASS_ACT; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    virtual void GetAssets(CAssetHelper &helper);

    bool LoadAct(const char *fname, const CLoadParameter &param);
    void DeleteAct();

	CActAsset* GetAct() { return m_pAct; }

	float m_fSpeed;
	float m_fSkipTime;

private:

    CActAsset *m_pAct;
};


class CFxActComponent : public CFxComponent
{
public:
    CFxActComponent(CFx *fx, CFxActComponentResource *res);
    virtual ~CFxActComponent();

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);
	virtual void DrawSelected(IPlotter3D &plotter, IPlotter2D &canvas);

	bool GetCamera(CMatrix &matrix, float &fov, float &znear, float &zfar);

    CFxActComponentResource* ActResource() { return (CFxActComponentResource*)m_pResource; }

private:
    void UpdateAct();

    CActPlayer *m_pActPlayer;
};


}//GELib

#endif//__GELIB_FX_FXACTCOMPONENT_H__
