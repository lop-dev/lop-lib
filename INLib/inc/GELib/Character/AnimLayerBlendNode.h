//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/AnimLayerBlendNode.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_ANIMLAYERBLENDNODE_H__
#define __GELIB_CHARACTER_ANIMLAYERBLENDNODE_H__

#include "AnimBlendNode.h"
#include "CharacterDesc.h"

namespace GELib
{

class CBone;



class CAnimLayerBlendNode : public CAnimBlendNode
{
public:
	CAnimLayerBlendNode(CAnimBlendTree *tree);
	virtual ~CAnimLayerBlendNode();

	virtual EAnimNodeClass GetClassId() { return ANIM_NODE_LAYER_BLEND; }

    virtual void Setup();
	virtual float GetPlayDuration();
	virtual void Update_R(float t);
    virtual bool SampleAnimation(AnimationSampleList &samples);

	void SetAdditiveBlend(bool a) { m_bAdditiveBlend = a; }
	bool IsAdditiveBlend() { return m_bAdditiveBlend; }

protected:

	void ApplyBoneWeight();
	void ApplyBoneWeight_R(CBone *bone, const CCharacterDesc::CBoneWeight &d);

	std::vector<float> m_BoneWeights;
	bool m_bAdditiveBlend;
};


}//GELib

#endif//__GELIB_CHARACTER_ANIMLAYERBLENDNODE_H__
