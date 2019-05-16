//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxPlayer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXPLAYER_H__
#define __GELIB_FX_FXPLAYER_H__

#include <list>
#include <GELib/Math/GeMath.h>
#include <GELib/Math/RandomNumber.h>
#include <GELib/Ext/IAttachable.h>
#include <GELib/Ext/AssetLoadTask.h>

namespace GELib
{

class IPlotter3D;
class IPlotter2D;
class CRenderWorld;
class CPhysicsWorld;
class CFx;
class CFxAsset;
class IFxCallback;
class CFxCameraComponent;

class CFxPlayer
{
public:
    CFxPlayer();
    virtual ~CFxPlayer();

    void Initialize(CRenderWorld *renderWorld, CPhysicsWorld *physicsWorld);

    CRenderWorld* GetRenderWorld();
    CPhysicsWorld* GetPhysicsWorld();
    CRandomNumber& GetRandom() { return m_Random; }

	void SetCallback(IFxCallback *cb) { m_pCallback = cb; }
	IFxCallback* GetCallback() const { return m_pCallback; }

    void SetCameraMatrix(const CMatrix &matrix);
    const CMatrix& GetCameraMatrix() const;
    IAttachPoint* GetCameraAttachPoint();

    void SetLod(int lod);       // 0: low, 1: medium, 2: high, default 2
    int GetLod() const { return m_iLod; }

    void EnableSound(bool enable) { m_bEnableSound = enable; }
    bool IsEnableSound() const { return m_bEnableSound; }

	// editor use
	void SetFastUpdate(bool enable) { m_bFastUpdate = enable; }
	bool IsFastUpdate() { return m_bFastUpdate; }

    CFx* Play(CFxAsset *res);
    CFx* Play(const char *fname);
    CFx* Play(const char *fname, const CLoadParameter &param);

    void Delete(CFx *fx);
	void DeleteWaitObjects();

    void Update(float t);
    void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

private:
	// it's passible to spawn new fx when update fx, so use std::list
	typedef std::list<CFx*> FxList;
    FxList m_Fxs;

    class CCameraAttachPoint : public IAttachPoint
    {
    public:
        CCameraAttachPoint() { m_Matrix.SetIdentity(); }
        virtual const char * GetName() { return "Camera"; }
        virtual void GetMatrix(CMatrix &matrix) { matrix = m_Matrix; }
        CMatrix m_Matrix;
    };
    CCameraAttachPoint m_CameraAttachPoint;

    CRenderWorld *m_pRenderWorld;
    CPhysicsWorld *m_pPhysicsWorld;
    CRandomNumber m_Random;
	IFxCallback *m_pCallback;
    int m_iLod;
    bool m_bEnableSound;
	bool m_bFastUpdate;
};




}//GELib

#endif//__GELIB_FX_FXPLAYER_H__
