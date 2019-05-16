//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/AnimDifferenceNode.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_ANIMDIFFERENCENODE_H__
#define __GELIB_CHARACTER_ANIMDIFFERENCENODE_H__

#include "AnimBlendNode.h"

namespace GELib
{

class CAnimDifferenceNode : public CAnimBlendNode
{
public:
	CAnimDifferenceNode(CAnimBlendTree *tree);
	virtual ~CAnimDifferenceNode();

	virtual EAnimNodeClass GetClassId() { return ANIM_NODE_DIFFERENCE; }

	virtual float GetPlayDuration();
	virtual void Update_R(float t);
    virtual bool SampleAnimation(AnimationSampleList &samples);

};


}//GELib

#endif//__GELIB_CHARACTER_ANIMDIFFERENCENODE_H__
