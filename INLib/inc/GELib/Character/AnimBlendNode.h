//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/AnimBlendNode.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_ANIMBLENDNODE_H__
#define __GELIB_CHARACTER_ANIMBLENDNODE_H__

#include "AnimNode.h"

namespace GELib
{

const float ANIM_BLEND_WEIGHT_MIN = 0.001f;

class CAnimBlendNode : public CAnimNode
{
public:
	CAnimBlendNode(CAnimBlendTree *tree);
	virtual ~CAnimBlendNode();

	virtual EAnimNodeClass GetClassId() { return ANIM_NODE_BLEND; }

	virtual float GetPlayDuration();
	virtual void NormalizeSpeed_R(float speed);
	virtual void Update_R(float t);
    virtual bool SampleAnimation(AnimationSampleList &samples);

	void SetSync(bool sync) { m_bSync = sync; }
	bool GetSync() { return m_bSync; }

protected:

	bool m_bSync;

};


}//GELib

#endif//__GELIB_CHARACTER_ANIMBLENDNODE_H__
