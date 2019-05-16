//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/Character.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_CHARACTER_H__
#define __GELIB_CHARACTER_CHARACTER_H__

#include <string>
#include <vector>
#include <GELib/Utility/StringMap.h>
#include <GELib/Ext/IAttachable.h>
#include "Skeleton.h"
#include "AnimationSet.h"
#include "AnimationSample.h"
#include "CharacterDesc.h"
#include "CharacterTypes.h"
#include "DynamicBone.h"
#include "IkBone.h"
#include "SkinMeshRenderObject.h"

namespace GELib
{

class IPlotter2D;
class IPlotter3D;
class CAssetHelper;
class CRenderWorld;
class CPhysicsWorld;
class CFx;
class CFxAsset;
class CFxPlayer;
class CAudio;


class CAnimationAsset;
class CAnimBlendTree;
class CAnimPlayNode;

class CCharShape
{
public:
	friend class CCharacter;

	CCharShape() { m_bDirty = true; m_pShapeDesc = 0; m_pBone = 0; }
	ECharShapeType GetType() const { return m_pShapeDesc->m_eType; }
    int GetId() const { return m_pShapeDesc->m_iId; }
    int GetPart() const { return m_pShapeDesc->m_iPart; }
	CBone* GetBone() { return m_pBone; }

	// update and cast type
	CSphere& GetSphere() { Update(); return Sphere(); }
	COBox& GetBox() { Update(); return Box(); }
	CCapsule& GetCapsule() { Update(); return Capsule(); }

	void Update();

	// cast type
	CSphere& Sphere() { return *(CSphere*)m_Floats; }
	COBox& Box() { return *(COBox*)m_Floats; }
	CCapsule& Capsule() { return *(CCapsule*)m_Floats; }

private:

	float m_Floats[20];
	bool m_bDirty;
	CCharacterDesc::CShape *m_pShapeDesc;
	CBone *m_pBone;
};

class CCharacter : public IAttachable
{
public:
    CCharacter();
    virtual ~CCharacter();

    void Initialize(CRenderWorld *renderWorld, CPhysicsWorld *physicsWorld, CFxPlayer *fxPlayer);
	CRenderWorld* GetRenderWorld() { return m_pRenderWorld; }
	CPhysicsWorld* GetPhysicsWorld() { return m_pPhysicsWorld; }
	CFxPlayer* GetFxPlayer() { return m_pFxPlayer; }
	void AddToRenderWorld();
	bool IsInRenderWorld();

    // implement IAttachable
    virtual int GetAttachPoints(std::vector<IAttachPoint*> &points);
    virtual IAttachPoint* GetAttachPoint(const char *name);

    void Update(float t);
    void SetDrawFlag(int flag) { m_iDrawFlag = flag; }
    void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    void SetMatrix(const CMatrix &matrix);	// can have scale
    const CMatrix& GetMatrix();
	float GetScale() { return m_fScale; }

    // load from desc
    bool LoadDesc(const char *fname, int index, const CLoadParameter &param);
    CCharacterDesc* GetDesc(int index);
    int GetNumDescs();
    void DeleteDesc(int index);
	void Unload();
    bool IsLoading();
    bool CreateLoadedResource();

	// show / hide skin and attached fx
	void Show(bool show);
	bool IsShow() { return m_bShow; }
	void ShowDesc(int index, bool show);

	void SetColor(const CVector3 &c);
	void SetOpacity(float o);

    // internal object
    CSkeleton* GetSkeleton() { return m_pSkeleton; }
    CSkinMeshRenderObject* GetRenderObject() { return m_pRenderObject; }
    CAnimBlendTree* GetBlendTree() { return m_pBlendTree; }
	void ChangeBlendTree(int n);		// editor use, n=0: default tree, n=1: desc tree
	bool IsDefaultBlendTree() { return m_bDefaultBlendTree; }

    void Build();   // call Build() after modify skeleton or skin

    void GetAssets(CAssetHelper &helper);

	// animation set
	bool LoadAnimationSet(const char *fname, const CLoadParameter &param);
    void GetAnimationSets(std::vector<CAnimationSet*> &a) { a = m_AnimationSets; }

	// play animation
    void SetAnimation(CAnimationAsset *a);
    CAnimationAsset* GetAnimation();

	// play animation with desc
    CAnimationDesc* GetAnimationDesc(const char *name, int *setIdx=0, int *descIdx=0);
    CAnimationDesc* GetAnimationDesc(int setIdx, int descId);
    void SetAnimation(const char *name);
	void SetAnimation(int setIdx, int descId);
    void SetAnimation(CAnimationDesc *a);
    CAnimationDesc* GetAnimationDesc();

    void SetAnimationLoop(bool loop);
	bool GetAnimationLoop();
    void SetAnimationSpeed(float s);
    float GetAnimationSpeed();
    void SetAnimationTransition(int mode, float time);
    bool GetAnimationTransition(CAnimationDesc *anim, CAnimationDesc *prevAnim, int &mode, float &time);
	void GetAnimationTransitionTime(float &startTime, float &duration);		// editor use
    void SetAnimationTime(float t);
    float GetAnimationTime();
    void GetAnimationMinMaxTime(float &minTime, float &maxTime, bool loop);
    void PlayAnimation();
    void PauseAnimation(bool pause);

	// animation status
    bool IsAnimationEnd();
    bool IsAnimationPaused();
    bool IsAnimationJustLoop();
    bool IsTransitionAnimation();

    // animation period, event
    CAnimDescPeriod* GetAnimationPeriod(int periodId);
    AnimationEventList& GetAnimationEvents();

	// cull bone update, event
	void CullAnimation(bool c) { m_bCullAnimation = c; }
	bool IsCullAnimation() { return m_bCullAnimation; }

    // root motion
    const CMatrix& GetMoveMatrix() { return m_MoveMatrix; }
	CBone* GetMoveBone() { 	return m_pMoveBone; }
	void EnableMoveBone(bool enable) { m_bEnableMoveBone = enable; }
	bool IsEnableMoveBone() { return m_bEnableMoveBone; }
	void UpdateMoveBoneOnly(bool b);	// update MoveMatrix only, skeleton not updated
	bool IsUpdateMoveBoneOnly();

	// foot height
	void EnableFootHeight(bool enable) { m_bEnableFootHeight = enable; }
	bool IsEnableFootHeight() { return m_bEnableFootHeight; }

	// dynamic bone
    CDynamicBoneManager* GetDynamicBone() { return &m_DynamicBone; }
	void EnableDynamicBone(bool enable);
	bool IsEnableDynamicBone() { return m_bEnableDynamicBone; }
	void ResetDynamicBone();

	// ik
    CIkBoneManager* GetIkBone() { return &m_IkBone; }
	void EnableIK(bool enable);
	bool IsEnableIK() { return m_bEnableIK; }

	void AdjustHeight(float offset);	// called by IkBone

    // fx, audio
    CFx* PlayFx(const char *name, bool attach=true, bool syncSpeed=true);
	void ClearFx();
	void SetFxOpacity(float o);
    CAudio* PlayAudio(const char *name);
	void ClearAudio();

    // shapes bind to bone
    int GetNumShapes() { return (int)m_Shapes.size(); }
    CCharShape& GetShape(int index) { return m_Shapes[index]; }

    void SetRenderBoundingShapeMask(long long mask) { m_iRenderBoundingShapeMask = mask; }
    long long GetRenderBoundingShapeMask() { return m_iRenderBoundingShapeMask; }

    void SetHurtCollisionShapeMask(long long mask) { m_iHurtCollisionShapeMask = mask; }
    long long GetHurtCollisionShapeMask() { return m_iHurtCollisionShapeMask; }

	void SetLoadTask(CLoadTask *task) { m_pLoadTask = task; }
    CLoadTask* GetLoadTask() const { return m_pLoadTask; }

private:

    void NewModel();

	bool LoadFromDesc(CCharacterDesc *desc, int index, const CLoadParameter &param);
	bool _LoadDesc(const char *fname, int index, const CLoadParameter &param);

    void BuildAttachPoints();
	void CreateBlendTree(CCharacterDesc *desc);
	void CreateDefaultBlendTree();
    void AppendShapes(CCharacterDesc *desc);
	void SetSpecialBone(CCharacterDesc *desc);
	void HideSkins(CCharacterDesc *desc);
	void CreateDescFx();
	void ClearDescFx();

	void PurgeFx();
	void PurgeAudio();

	void ScaleBone(const char *name, const CVector3 &scale);
	void ComputeMoveMatrix(AnimationSampleList &samples, bool alignSkeleton);
	void AdjustHeightByFoot(float t);

    void UpdateRenderObject();
    void ComputeRenderBoundingBox();

    CRenderWorld *m_pRenderWorld;
    CPhysicsWorld *m_pPhysicsWorld;
    CFxPlayer *m_pFxPlayer;
    CLoadTask *m_pLoadTask;

	CAnimBlendTree *m_pBlendTree;
	CAnimPlayNode *m_pPlayNode;
    CSkeleton *m_pSkeleton;
    CSkinMeshRenderObject *m_pRenderObject;
	float m_fScale;
	bool m_bDefaultBlendTree;

	class CDesc
	{
	public:
		CDesc() { m_pDesc = 0; m_bShow = true; }
		CCharacterDesc *m_pDesc;
		bool m_bShow;
	};
    std::vector<CDesc> m_Descs;
    std::vector<CAnimationSet*> m_AnimationSets;
	AnimationSampleList m_AnimationSamples;

	bool m_bShow;
    bool m_bPause;
	bool m_bRootMatrixDirty;
	bool m_bCullAnimation;
	bool m_bRenderObjectCulled;
	bool m_bFirstUpdate;
    int m_iDrawFlag;

    class CAttachPoint : public IAttachPoint
    {
    public:
        CBone *m_pBone;
        virtual const char * GetName();
        virtual void GetMatrix(CMatrix &matrix);
    };
    std::vector<CAttachPoint> m_AttachPoints;

	class CCharFx
	{
	public:
		CCharFx(CFx *fx, int flag)
		{
			m_pFx = fx;
			m_iFlag = flag;
		}
		CFx *m_pFx;
		int m_iFlag;
	};
    std::vector<CCharFx> m_Fxs;
    std::vector<CCharFx> m_DescFxs;
    std::vector<CAudio*> m_Audios;

	class CDescFxAsset
	{
	public:
		CDescFxAsset(int idx, CFxAsset *a)
		{
			m_iDescIndex = idx;
			m_pFxAsset = a;
		}
		int m_iDescIndex;
		CFxAsset *m_pFxAsset;
	};
    std::vector<CDescFxAsset> m_DescFxAssets;

    std::vector<CCharShape> m_Shapes;
    long long m_iRenderBoundingShapeMask;
    long long m_iHurtCollisionShapeMask;

    CBone *m_pMoveBone;
    CMatrix m_MoveMatrix;
	bool m_bEnableMoveBone;

	CBone *m_pFootBones[2];
    CBone *m_pFootHeightBones[2];
	bool m_bEnableFootHeight;
	float m_fFootHeightOffset;
	float m_fFootHeightOffsetSpeed;

	bool m_bEnableDynamicBone;
	bool m_bEnableIK;
	CDynamicBoneManager m_DynamicBone;
	CIkBoneManager m_IkBone;
};




}//GELib

#endif//__GELIB_CHARACTER_CHARACTER_H__
