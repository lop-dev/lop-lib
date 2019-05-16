//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/AnimIdentityNode.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_ANIMIDENTITYNODE_H__
#define __GELIB_CHARACTER_ANIMIDENTITYNODE_H__

#include "AnimNode.h"

namespace GELib
{

class CAnimIdentityNode : public CAnimNode
{
public:
	CAnimIdentityNode(CAnimBlendTree *tree);
	virtual ~CAnimIdentityNode();

	virtual EAnimNodeClass GetClassId() { return ANIM_NODE_IDENTITY; }

    virtual bool SampleAnimation(AnimationSampleList &samples);

};


}//GELib

#endif//__GELIB_CHARACTER_ANIMIDENTITYNODE_H__
