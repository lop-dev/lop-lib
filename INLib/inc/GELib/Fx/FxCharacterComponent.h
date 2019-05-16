//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxCharacterComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXCHARACTERCOMPONENT_H__
#define __GELIB_FX_FXCHARACTERCOMPONENT_H__

#include "FxComponent.h"
#include <GELib/Ext/FaceCamera.h>

namespace GELib
{

class CCharacter;


class CFxCharacterComponentResource : public CFxComponentResource
{
public:
    CFxCharacterComponentResource(CFxAsset *fxRes);
    virtual ~CFxCharacterComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();

    virtual int GetClassId() { return FXCLASS_CHARACTER; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    virtual void GetAssets(CAssetHelper &helper);

    bool LoadCharacter(const char *fname, const CLoadParameter &param);
    void DeleteCharacter();
    CCharacter* GetCharacter();

    EFaceCameraMode m_eFaceCameraMode;

    bool m_bCastDynamicShadow;
    bool m_bMotionBlur;
	bool m_bUseEnvironmentLight;
	bool m_bOnGround;
	bool m_bUseMoveMatrix;
	bool m_bHandleAnimationEvent;
	bool m_bGameCharacter;
    float m_fShadowResolutionFactor;
    int m_iLightGroups;
    std::string m_GameCharacterName;

private:

    CCharacter *m_pCharacter;
};


class CFxCharacterComponent : public CFxComponent
{
public:
    CFxCharacterComponent(CFx *fx, CFxCharacterComponentResource *res);
    virtual ~CFxCharacterComponent();

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    CFxCharacterComponentResource* CharacterResource() { return (CFxCharacterComponentResource*)m_pResource; }

	virtual IAttachable* GetAttachable();

	// accumulated move matrix
	const CMatrix& GetMoveMatrix() { return m_MoveMatrix; }

private:
    void UpdateCharacter(float t);
    void UpdateCharacter2(float t);

	bool m_bGameCharacter;
    CCharacter *m_pCharacter;
	CMatrix m_MoveMatrix;
};


}//GELib

#endif//__GELIB_FX_FXCHARACTERCOMPONENT_H__
