//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/AnimBlendTree.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_ANIMBLENDTREE_H__
#define __GELIB_CHARACTER_ANIMBLENDTREE_H__

#include "AnimationSample.h"
#include "AnimationSet.h"
#include <vector>

namespace GELib
{

class CSkeleton;
class CAnimNode;
class CAnimBlendLayer;

class CAnimBlendTree
{
public:
	CAnimBlendTree();
	virtual ~CAnimBlendTree();

	void SetSkeleton(CSkeleton *skeleton);
	CSkeleton* GetSkeleton() { return m_pSkeleton; }

	void SetRoot(CAnimNode *n) { m_pRoot = n; }
	CAnimNode* GetRoot() { return m_pRoot; }
	CAnimNode* FindNode(const char *name);

	void Setup();

	void Update(float t);
    bool SampleAnimation(AnimationSampleList &samples);

    AnimationEventList& GetAnimationEvents() { return m_AnimationEvents; }
	void AppendAnimationEvent(CAnimDescEvent *e);

private:

	void Setup_R(CAnimNode *n);
	CAnimNode* FindNode_R(CAnimNode *n, const std::string &name);

	CSkeleton *m_pSkeleton;
	CAnimNode *m_pRoot;
    AnimationEventList m_AnimationEvents;
};


}//GELib

#endif//__GELIB_CHARACTER_ANIMBLENDTREE_H__
