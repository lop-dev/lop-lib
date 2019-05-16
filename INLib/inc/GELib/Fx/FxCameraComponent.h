//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxCameraComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXCAMERACOMPONENT_H__
#define __GELIB_FX_FXCAMERACOMPONENT_H__

#include "FxComponent.h"

namespace GELib
{

class CAudio;


class CFxCameraComponentResource : public CFxComponentResource
{
public:
    CFxCameraComponentResource(CFxAsset *fxRes);
    virtual ~CFxCameraComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);

    virtual int GetClassId() { return FXCLASS_CAMERA; }
    virtual CFxComponent* CreateInstance(CFx *fx);

	float m_fNear;
	float m_fFar;
	std::string m_LookTargetName;

	bool m_bEnableDof;
	int m_iDofBlurRadius;
	float m_fDofBlurDeviation;

	class CShake
	{
	public:
		CShake();
		char m_iWaveType;	// 0: none, 1: sin, 2: triangle, 3: noise
		char m_iNoiseRoughness;
		float m_fFrequency;
		CVector3 m_Direction;
		CVector3 m_Rotation;
	};

	enum { SHAKE_NUM = 10 };
	CShake m_Shakes[SHAKE_NUM];

private:

};


class CFxCameraComponent : public CFxComponent
{
public:
    CFxCameraComponent(CFx *fx, CFxCameraComponentResource *res);
    virtual ~CFxCameraComponent();

    virtual void SetRelatedComponents();

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);
	virtual void DrawSelected(IPlotter3D &plotter, IPlotter2D &canvas);

	float GetFov();
	float GetNear();
	float GetFar();

	bool IsEnableDof();
	int GetDofBlurRadius();
	float GetDofBlurDeviation();
	void GetFocusDistance(float &focusDist, float &nearDist, float &farDist);

    CFxCameraComponentResource* CameraResource() { return (CFxCameraComponentResource*)m_pResource; }

private:

	void LookAt(const CVector3 &p);

    IAttachPoint *m_pLookTarget;

};



}//GELib

#endif//__GELIB_FX_FXCAMERACOMPONENT_H__
