//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/AnimationSample.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_ANIMATIONSAMPLE_H__
#define __GELIB_CHARACTER_ANIMATIONSAMPLE_H__

#include <vector>
#include <GELib/Math/GeMath.h>

namespace GELib
{

class CAnimationSample
{
public:
	CQuaternion m_Rotation;
	CVector3 m_Translation;
	CVector3 m_Scale;

	void Reset();
	void Multiply(float w);
	void Blend(const CAnimationSample &as, float w);
	void AdditiveBlend(const CAnimationSample &as, float w);
	void Accumulate(const CAnimationSample &as, float w);
	void Normalize();
	void FromMatrix(const CMatrix &m);
	void ToMatrix(CMatrix &m);
};

typedef std::vector<CAnimationSample> AnimationSampleList;


inline void CAnimationSample::Multiply(float w)
{
	m_Rotation *= w;
	m_Translation *= w;
	m_Scale *= w;
}

inline void CAnimationSample::Blend(const CAnimationSample &as, float w)
{
	m_Rotation = Nlerp(m_Rotation, as.m_Rotation, w);
	m_Translation = Lerp(m_Translation, as.m_Translation, w);
	m_Scale = Lerp(m_Scale, as.m_Scale, w);
}

inline void CAnimationSample::AdditiveBlend(const CAnimationSample &as, float w)
{
	m_Rotation = Nlerp(m_Rotation, as.m_Rotation * m_Rotation, w);
	m_Translation += as.m_Translation * w;
	m_Scale = Lerp(m_Scale, m_Scale * as.m_Scale, w);
}

inline void CAnimationSample::Accumulate(const CAnimationSample &as, float w)
{
	if (DotProduct(m_Rotation, as.m_Rotation) >= 0)
		m_Rotation += as.m_Rotation * w;
	else
		m_Rotation -= as.m_Rotation * w;
	m_Translation += as.m_Translation * w;
	m_Scale += as.m_Scale * w;
}

inline void CAnimationSample::Normalize()
{
	m_Rotation.Normalize();
}

}//GELib

#endif//__GELIB_CHARACTER_ANIMATIONSAMPLE_H__
