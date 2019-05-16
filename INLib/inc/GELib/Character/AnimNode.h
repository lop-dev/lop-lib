//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/AnimNode.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_ANIMNODE_H__
#define __GELIB_CHARACTER_ANIMNODE_H__

#include <string>
#include <vector>
#include "AnimationSet.h"
#include "AnimationSample.h"
#include "CharacterDesc.h"


namespace GELib
{

class CAnimBlendTree;
class CAnimationControl;
class CSkeleton;

enum EAnimNodeClass
{
	ANIM_NODE,
	ANIM_NODE_PLAY,
	ANIM_NODE_PLAY_END,
	ANIM_NODE_IDENTITY,
	ANIM_NODE_IDENTITY_END,
	ANIM_NODE_BLEND,
		ANIM_NODE_LAYER_BLEND,
		ANIM_NODE_LAYER_BLEND_END,
		ANIM_NODE_DIFFERENCE,
		ANIM_NODE_DIFFERENCE_END,
	ANIM_NODE_BLEND_END,
	ANIM_NODE_END,
};

class CAnimNode
{
public:
	CAnimNode(CAnimBlendTree *tree);
	virtual ~CAnimNode();

	virtual EAnimNodeClass GetClassId() { return ANIM_NODE; }
	bool IsA(EAnimNodeClass cls, EAnimNodeClass clsEnd);

	void SetDesc(CCharacterDesc::CBlendNode *desc);
	const std::string& GetName();

	void SetWeight(float w) { m_fWeight = w; }
	float GetWeight() { return m_fWeight; }

	void SetChildrenWeightLinear(float w);

	void SetSpeed(float s) { m_fSpeed = s; }
	float GetSpeed() { return m_fSpeed; }

    virtual void Setup();
	virtual float GetPlayDuration();
	virtual float GetPlaySpeed();			// speed * desc speed
	virtual void NormalizeSpeed_R(float speed);
	virtual void Play_R();
	virtual void Update_R(float t);
    virtual bool SampleAnimation(AnimationSampleList &samples);

	CAnimBlendTree* GetTree() { return m_pTree; }
	CAnimNode* GetParent() { return m_pParent; }

	void AddChild(CAnimNode *n);
	void RemoveChild(int index);
	CAnimNode* GetChild(int index);
	int GetNumChildren();

protected:

	float m_fWeight;
	float m_fSpeed;

	CCharacterDesc::CBlendNode *m_pDesc;
	CAnimBlendTree *m_pTree;
	CAnimNode *m_pParent;
	std::vector<CAnimNode*> m_Children;
};


}//GELib

#endif//__GELIB_CHARACTER_ANIMNODE_H__
