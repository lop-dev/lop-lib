//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/AnimPlayNode.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_ANIMPLAYNODE_H__
#define __GELIB_CHARACTER_ANIMPLAYNODE_H__

#include "AnimNode.h"
#include "AnimationControl.h"
#include "AnimationSet.h"

namespace GELib
{

class CAnimationControl;

class CAnimPlayNode : public CAnimNode
{
public:
	CAnimPlayNode(CAnimBlendTree *tree);
	virtual ~CAnimPlayNode();

	virtual EAnimNodeClass GetClassId() { return ANIM_NODE_PLAY; }

    virtual void Setup();
	virtual float GetPlayDuration();
	virtual float GetPlaySpeed();
	virtual void NormalizeSpeed_R(float speed);
	virtual void Play_R();
	virtual void Update_R(float t);
    virtual bool SampleAnimation(AnimationSampleList &samples);

    void SetAnimation(CAnimationAsset *a);
    CAnimationAsset* GetAnimation();

	void SetAnimation(CAnimationDesc *a);
	CAnimationDesc* GetAnimationDesc() { return m_pAnimationDesc; }

    void SetLoop(bool loop);
	bool GetLoop();
    void SetTransition(int mode, float time);
    bool GetTransition(CAnimationDesc *a, CAnimationDesc *prevAnim, int &mode, float &time);
	float GetTransitionStartTime() { return m_fTransitionStartTime; }	// editor use
    void SetTime(float t);
    float GetTime();
    void SetNormalizedTime(float t);
    float GetNormalizedTime();
    void Play();

	// animation status
    bool IsEnd();
    bool IsJustLoop();
    bool IsTransition();

    // animation period, event
    CAnimDescPeriod* GetPeriod(int periodId);
	void GenerateEvent(float t0, float t1);

	CAnimationControl* GetAnimationControl() { return &m_Control; }

protected:

	CAnimationControl m_Control;
	CAnimationDesc *m_pAnimationDesc;
	float m_fDescSpeed;
    float m_fTransitionStartTime;
    float m_fTransitionNormalizedTime;
    float m_fTransitionNormalizedLoopTime;
	bool m_bJustPlay;
	bool m_bSkipMoveMatrix;
	CQuaternion m_PrevRotation;
	CVector3 m_PrevTranslation;
};


}//GELib

#endif//__GELIB_CHARACTER_ANIMPLAYNODE_H__
