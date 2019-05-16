//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/Fx.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FX_H__
#define __GELIB_FX_FX_H__

#include <vector>
#include <string>

#include <GELib/Math/GeMath.h>
#include <GELib/Ext/IAttachable.h>

namespace GELib
{

class CIOStream;
class IPlotter3D;
class IPlotter2D;
class CPhysicsObject;
class CLoadTask;


class CFxComponent;
class CFxCameraComponent;
class CFxDirectorComponent;
class CFxActComponent;
class CFxAsset;
class CFxPlayer;
class CFxCameraStatus;

const int FX_DRAWFLAG_EDITOR = 1 << 0;
const int FX_DRAWFLAG_IMAGE = 1 << 1;

class CFx : public IAttachable
{
public:
    CFx(CFxPlayer *player, CFxAsset *res);
    virtual ~CFx();

    // implement IAttachable
    virtual int GetAttachPoints(std::vector<IAttachPoint*> &points);
    virtual IAttachPoint* GetAttachPoint(const char *name);

    void SetMatrix(const CMatrix &matrix);    // matrix should not contains scale, use SetScale() instead
    void GetMatrix(CMatrix &matrix) const { matrix = m_Matrix; }
    const CMatrix& GetMatrix() const { return m_Matrix; }

	CAABox GetBoundingBox();
    const CAABox& GetAttackBoundingBox() const { return m_AttackBox; }

	void Show(bool show);
	bool IsShow() const { return m_bShow; }

	void SetLodGroup(int g) { m_iLodGroup = (char)g; }
	int GetLodGroup() const { return m_iLodGroup; }

    void Attach(IAttachable *a0, IAttachable *a1=0);
    void Detach();
    IAttachable* GetAttachable(int index=0);	// index = 0 or 1

    void SetTime(float t);
    float GetTime() const { return m_fTime; }
	float GetPrevTime() { return m_fPrevTime; }
    float GetFrameTime() const { return m_fFrameTime; }
    void Update(float t);

    void SetColor(const CVector3 &c) { m_Color = c; }
    const CVector3& GetColor() const { return m_Color; }
    void SetOpacity(float a) {  m_fOpacity = a; }
    float GetOpacity() const { return m_fOpacity; }
    void  SetScale(float s) { m_fScale = s; }
    float GetScale() const { return m_fScale; }

	void SetMaterialParameter(int index, const CVector4 &param) { m_MaterialParameter = param; }
	const CVector4& GetMaterialParameter(int index) { return m_MaterialParameter; }

	void CullAnimation(bool c) { m_bCullAnimation = c; }
	bool IsCullAnimation() { return m_bCullAnimation; }
	void SetMaxVisibleDistance(float d) { m_fMaxVisibleDistance = d; }	// replace CFxAsset::SetMaxVisibleDistanceRatio()
	float GetMaxVisibleDistance() { return m_fMaxVisibleDistance; }
    float GetFxMaxVisibleDistance() const;
	void SetComputeLightOcclusion(bool o) { m_bComputeLightOcclusion = o; }
	bool IsComputeLightOcclusion() const { return m_bComputeLightOcclusion; }

    void SetSpeed(float s) { m_fSpeed = s; }
    float GetSpeed() const { return m_fSpeed; }

    void SetAutoDelete(bool a) { m_bAutoDelete = a; }
    bool GetAutoDelete() const { return m_bAutoDelete; }

    void Play();
    void Stop();
    bool IsStopped() const { return m_bStopped; }

    void SetLoop(bool loop) { m_bLoop = loop; }
    bool GetLoop() const { return m_bLoop; }

	void SetLoopCount(int count);
	int GetLoopCount() { return m_iLoopCount; }

	void Halt();
	void Unhalt();
	bool IsHalted();
	float GetHaltTime() { return m_fHaltTime; }

    void SetUserData(int data) { m_iUserData = data; }
    int  GetUserData() const { return m_iUserData; }
    void SetUserPointer(void *data) { m_pUserData = data; }
    void* GetUserPointer() const { return m_pUserData; }

    bool Attack(const CSphere &sphere);
    bool Attack(const COBox &box);
    bool Attack(CPhysicsObject *pobj, int mask);

	void SetDrawFlag(int f) { m_iDrawFlag = f; }
	int GetDrawFlag() { return m_iDrawFlag; }

    void Draw(IPlotter3D &plotter, IPlotter2D &canvas);
    void DrawAttack(IPlotter3D &plotter, IPlotter2D &canvas);

    CFxPlayer* GetPlayer() const { return m_pPlayer; }
    CFxAsset* GetResource() const { return m_pResource; }
    IAttachPoint* GetAttachPointByName(const char *name, bool &component);

    int GetNumComponents();
    CFxComponent* GetComponent(int index);
    CFxComponent* GetComponentByName(const char *name);

	void GetCameraStatus(CFxCameraStatus &cs, CFxDirectorComponent *director=0);	// director==0, use active director
	void GetCameraStatus(CFxCameraStatus &cs, CFxCameraComponent *camera);
	void GetCameraStatus(CFxCameraStatus &cs, CFxActComponent *act);
	CFxCameraComponent* GetActiveCameraComponent() { return m_pActiveCamera; }
	CFxActComponent* GetActiveActComponent() { return m_pActiveAct; }

    void Rendered();

	// used by CFxPlayer
	void DeleteMe() { m_bDeleteMe = true; }
	bool IsDeleteMe() { return m_bDeleteMe; }

private:

    void CreateComponents();
    void DeleteComponents();

	bool m_bNew;
    bool m_bDeleteMe;
	bool m_bShow;
    bool m_bLoop;
    bool m_bStopped;
    bool m_bAutoDelete;
	bool m_bCullAnimation;
	bool m_bComputeLightOcclusion;
	char m_iCullCounter;
	char m_iLodGroup;
    float m_fOpacity;
    float m_fScale;
    float m_fSpeed;
    float m_fTime;
	float m_fPrevTime;
    float m_fFrameTime;
	float m_fHaltTime;
	float m_fMaxVisibleDistance;
	int m_iLoopCount;
	int m_iLoopCounter;
    CVector3 m_Color;
	CVector4 m_MaterialParameter;

    int m_iUserData;
    void *m_pUserData;

    CMatrix m_Matrix;
    CAABox m_AttackBox;

	int m_iDrawFlag;

    IAttachable *m_pAttachable[2];
    CFxPlayer *m_pPlayer;
    CFxAsset *m_pResource;
	int m_iResourceChangeCounter;
    std::vector<CFxComponent*> m_Components;
	CFxCameraComponent *m_pActiveCamera;
	CFxDirectorComponent *m_pActiveDirector;
	CFxActComponent *m_pActiveAct;
};

class CFxCameraStatus
{
public:
	CFxCameraStatus();
	void Reset();

	bool m_bHasCamera;
	bool m_bHasDirector;
	bool m_bSubView;		// editor use

	CMatrix m_CameraMatrix;
	float m_fCameraFov;
	float m_fCameraNear;
	float m_fCameraFar;

    bool m_bEnableDof;
    float m_fFocalPlaneDistance;
    float m_fNearBlurPlaneDistance;
    float m_fFarBlurPlaneDistance;
    int m_iDofBlurRadius;
    float m_fDofBlurDeviation;

	float m_fPlaySpeed;
};


}//GELib

#endif//__GELIB_FX_FX_H__
