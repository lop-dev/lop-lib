//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/AnimationControl.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_ANIMATIONCONTROL_H__
#define __GELIB_CHARACTER_ANIMATIONCONTROL_H__

#include "AnimationAsset.h"
#include "AnimationSample.h"

namespace GELib
{

class CSkeleton;

typedef std::vector<CAnimationSample> AnimationSampleList;

class CAnimationControl
{
public:
    CAnimationControl();
    virtual ~CAnimationControl();

    void SetSkeleton(CSkeleton *skeleton);

    void SetAnimation(CAnimationAsset *animation);
    CAnimationAsset* GetAnimation();

    void Update(float t);
    bool SampleAnimation(AnimationSampleList &samples);

    // time, speed
    void SetTime(float t);
    float GetTime() const { return m_fTime; }
    void SetNormalizedTime(float t);
    float GetNormalizedTime();
    void SetNormalizedLoopTime(float t);
    float GetNormalizedLoopTime();
    void SetSpeed(float s) { m_fSpeed = s; }
    float GetSpeed() const { return m_fSpeed; }

    void SetPlayRange(float fromTime, float toTime) { m_fPlayRange[0] = fromTime; m_fPlayRange[1] = toTime; }
    void GetPlayRange(float &fromTime, float &toTime) const { fromTime = m_fPlayRange[0]; toTime = m_fPlayRange[1]; }
    void SetLoop(bool loop) { m_bLoop = loop; }
    bool GetLoop() const { return m_bLoop; }
    void SetLoopRange(float fromTime, float toTime) { m_fLoopRange[0] = fromTime; m_fLoopRange[1] = toTime; }
    void GetLoopRange(float &fromTime, float &toTime) const { fromTime = m_fLoopRange[0]; toTime = m_fLoopRange[1]; }

    void GetMinMaxTime(float &minTime, float &maxTime, bool loop);

    // transition
    void SetTransitionTime(float t) { m_fTransitionTime = t; }
    float GetTransitionTime() const { return m_fTransitionTime; }
    void SetWaitTransition(bool wait) { m_bWaitTransition = wait; }
    bool GetWaitTransition() const { return m_bWaitTransition; }
    bool IsTransition() const;

    // play status
    bool IsPlayEnd() const { return m_bPlayEnd; }
    bool IsJustLoop() const { return m_bJustLoop; }

private:

    CSkeleton *m_pSkeleton;
    CAnimationAsset *m_pAnimation;

    bool m_bLoop;
    bool m_bPlayEnd;
    bool m_bJustLoop;
    float m_fTime;
    float m_fSpeed;
    float m_fPlayRange[2];
    float m_fLoopRange[2];

    float m_fTransitionTime;
    float m_fTransitionPlayTime;
    bool m_bWaitTransition;
    bool m_bSwapSampleIndex;
    int m_iSampleIndex;

    class CTrackInfo
    {
    public:
        CTrackInfo() { m_iKeyIndexHint = 0; }
        int m_iKeyIndexHint;

		// transition temp variables
        CQuaternion m_Rotation[2];
        CVector3 m_Translation[2];
        CVector3 m_Scale[2];
    };
    std::vector<CTrackInfo> m_TrackInfos;
};

inline bool CAnimationControl::IsTransition() const
{
    return m_fTransitionTime > 0 && m_fTransitionPlayTime < m_fTransitionTime;
}


}//GELib

#endif//__GELIB_CHARACTER_ANIMATIONCONTROL_H__
