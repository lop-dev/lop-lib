//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/DynamicBone.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_DYNAMICBONE_H__
#define __GELIB_CHARACTER_DYNAMICBONE_H__

#include <vector>
#include <GELib/Math/GeMath.h>
#include "CharacterDesc.h"

namespace GELib
{

class IPlotter2D;
class IPlotter3D;


class CBone;
class CCharacter;
class CDynamicBoneManager;

class CBoneParticle
{
public:

	CBoneParticle();

	CBone *m_pBone;
	short m_iParentIndex;

	float m_fDamping;
	float m_fElasticity;
	float m_fStiffness;
	float m_fInert;
	float m_fRadius;

	CVector3 m_Position;
	CVector3 m_PrevPosition;
	CVector3 m_EndBoneOffset;
};

class CDynamicBone
{
public:

	CDynamicBone();

	void Setup(CDynamicBoneManager *manager, CBone *root, const CCharacterDesc::CDynamicBone &desc);
	int GetParticleIndexByName(const std::string &name);
	CBoneParticle* GetParticle(int index);

	int GetId() { return m_iId; }
	void SetWeight(float w);
	float GetWeight() { return m_fWeight; }

	void UpdateParticles1();
	void UpdateParticles2();
	void ApplyParticlesToBones();
	void SkipUpdateParticles();

    void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

	void ResetPosition();

private:

	void AppendParticles(CBone *b, int parentIndex, const CCharacterDesc::CDynamicBone &desc,
						float damping, float elasticity, float stiffness, float inert, float radius);
	void Collide(CBoneParticle &p, float particleRadius, const CSphere &sphere);
	void Collide(CBoneParticle &p, float particleRadius, const CCapsule &capsule);
	void Inside(CBoneParticle &p, float particleRadius, const CSphere &sphere);
	void Inside(CBoneParticle &p, float particleRadius, const CCapsule &capsule);

	int m_iId;
	float m_fWeight;
	float m_fInertFactor;
	CVector3 m_Force;
	CVector3 m_LocalForce;
	long long m_iColliderMask;
	long long m_iInteriorMask;
	bool m_bRestForce;
	std::vector<CBoneParticle> m_Particles;
	CDynamicBoneManager *m_pManager;
};

class CBoneSpring
{
public:

	CBoneSpring();

	void Update();
    void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

	int m_iParticleIndex[2];
	float m_fElasticity;
	CDynamicBoneManager *m_pManager;
};

class CDynamicBoneManager
{
public:

	CDynamicBoneManager();

	void Append(CCharacter *c, const CCharacterDesc *desc);
	void Clear();

	CCharacter* GetCharacter() { return m_pCharacter; }
	float GetCharacterScale();
	const CVector3& GetCharacterMove() { return m_CharacterMove; }

	void ResetPosition();

	void SetWeight(float w);
	void SetWeight(int id, float w);
	float GetWeight(int id);

	void SetForce(const CVector3 f) { m_Force = f; }
	const CVector3& GetForce() { return m_Force; }

	void Update(float t);
    void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

	// bone spring use
	int GetParticleIndexByName(const std::string &name);
	CBoneParticle* GetParticle(int index);

private:

	float m_fTime;
	CCharacter *m_pCharacter;
	CVector3 m_Force;
	CVector3 m_CharacterMove;
	CVector3 m_CharacterPrevPosition;

	std::vector<CDynamicBone> m_DynamicBones;
	std::vector<CBoneSpring> m_BoneSprings;
};


}//GELib

#endif//__GELIB_CHARACTER_DYNAMICBONE_H__
