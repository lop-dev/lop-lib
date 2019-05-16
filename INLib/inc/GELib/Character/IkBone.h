//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/IkBone.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_IKBONE_H__
#define __GELIB_CHARACTER_IKBONE_H__

#include <vector>
#include <GELib/Math/GeMath.h>
#include "CharacterDesc.h"

namespace GELib
{

class IPlotter2D;
class IPlotter3D;


class CBone;
class CCharacter;
class CPhysicsWorld;


class CIkBoneManager;

enum EIkType
{
	IK_FREE,
	IK_FOOT,
};

class CIkBone
{
public:

	CIkBone();

	bool Setup(CIkBoneManager *manager, const CCharacterDesc::CIkBone *desc);
	EIkType GetType() { return (EIkType)m_pDesc->m_iType; }
	const CCharacterDesc::CIkBone* GetDesc() { return m_pDesc; }

	void Enable(bool e) { m_bEnable = e; }
	bool IsEnabled() { return m_bEnable; }

	void SetTargetPosition(const CVector3 &p) { m_TargetPosition = p; }
	const CVector3& GetTargetPosition() { return m_TargetPosition; }
	void ResetTargetPosition();

	bool FindGround(CPhysicsWorld *pw, float &groundz, float &offset);
	void SetTargetPositionToGround();

	void Update(float t);
    void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

private:

	bool m_bEnable;
	const CCharacterDesc::CIkBone *m_pDesc;
	int m_iNumBones;
	float m_fGroundHeight;
	CBone *m_Bones[4];
	CIkBoneManager *m_pManager;
	CVector3 m_TargetPosition;
};


class CIkBoneManager
{
public:

	CIkBoneManager();

	void Append(CCharacter *c, const CCharacterDesc *desc);
	void Clear();

	CCharacter* GetCharacter() { return m_pCharacter; }
	const CVector3& GetCharacterPosition();
	float GetCharacterScale();

	void EnableFootIk(bool e);
	void ClearFootIkOffset();

	void Update(float t);
    void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

private:

	CCharacter *m_pCharacter;
	float m_fCharacterPrevZ;
	float m_fOffset;
	float m_fSmoothOffset;
	float m_fOffsetSpeed;
	bool m_bEnableFootIk;

	std::vector<CIkBone> m_IkBones;
};


}//GELib

#endif//__GELIB_CHARACTER_IKBONE_H__
