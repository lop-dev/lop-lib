//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/CharacterDesc.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_CHARACTERDESC_H__
#define __GELIB_CHARACTER_CHARACTERDESC_H__

#include <vector>
#include <string>

#include <GELib/Math/GeMath.h>
#include <GELib/Ext/BaseAsset.h>
#include "CharacterTypes.h"

namespace GELib
{

class CIOStream;

class CCharacterDescManager;
class CCharacter;

const int ASSETCLASS_CHARACTER_DESC = 6;

class CCharacterDesc : public CBaseAsset
{
public:

    CCharacterDesc(CCharacterDescManager *manager=0, const char *cacheKey=0);
    virtual ~CCharacterDesc();

	virtual int GetClassId() { return ASSETCLASS_CHARACTER_DESC; }
    CCharacterDescManager* GetManager() { return m_pManager; }
    virtual void ManagerDelete();

    bool LoadScript(const void *file, int fileSize, const char *path=0);
    void Unload();

    class CNameIndex
    {
    public:
        CNameIndex(const char *name, int index) { m_Name = name; m_iIndex = index; }
        std::string m_Name;
        int m_iIndex;
    };

	enum EBoneType
	{
		MOVE_BONE = 1,
		LEFT_FOOT_BONE,
		RIGHT_FOOT_BONE,
		LEFT_FOOT_HEIGHT_BONE,
		RIGHT_FOOT_HEIGHT_BONE,
		TRANSLATE_ANIM_BONE,
		SCALE_ANIM_BONE
	};

    std::vector<CNameIndex> m_Parts;
    std::vector<CNameIndex> m_Skeletons;
    std::vector<CNameIndex> m_Skins;
    std::vector<CNameIndex> m_Fxs;
	std::vector<std::string> m_AnimationSets;

    class CHide
    {
    public:
        CHide(int skinIndex, int meshIndex) { m_iSkinIndex = skinIndex; m_iMeshIndex = meshIndex; }
        int m_iSkinIndex;
        int m_iMeshIndex;
    };
    std::vector<CHide> m_HideSkins;
    std::vector<CHide> m_HideMeshes;

    class CShape
    {
    public:
		CShape();

        int m_iId;
        int m_iPart;
        ECharShapeType m_eType;
        std::string m_Bone;
        CVector3 m_Offset;
        CVector3 m_Size;
		int m_iFlag;
    };
    std::vector<CShape> m_Shapes;

	class CDynamicBone
	{
	public:
		CDynamicBone();

        int m_iId;
        std::string m_Bone;
		float m_fDamping;
		float m_fDampingDecay;
		float m_fElasticity;
		float m_fElasticityDecay;
		float m_fStiffness;
		float m_fStiffnessDecay;
		float m_fInert;
		float m_fInertDecay;
		float m_fInertFactor;
		float m_fRadius;
		float m_fRadiusDecay;
		CVector3 m_Force;
	    long long m_iColliderMask;
	    long long m_iInteriorMask;
		bool m_bRestForce;
		float m_fEndBoneLength;
		CVector3 m_EndBoneOffset;
	};

	class CBoneSpring
	{
	public:
		CBoneSpring();

		float m_fElasticity;
		std::vector<std::string> m_Bones;
	};

	class CIkBone
	{
	public:
		CIkBone();

        int m_iId;
		int m_iType;
		float m_fAdjustRange;
		float m_fAdjustSpeed;
		float m_fTouchGroundThreshold;
		std::vector<std::string> m_Bones;
	};

    std::vector<CDynamicBone> m_DynamicBones;
    std::vector<CBoneSpring> m_BoneSprings;
    std::vector<CIkBone> m_IkBones;
	std::vector<CNameIndex> m_SpecialBones;

    long long m_iRenderBoundingShapeMask;
    long long m_iHurtCollisionShapeMask;
	float m_fFootHeightOffsetSpeed;

    CVector3 m_CollisionSceneSize;
    enum
	{
        NUM_COLLISION_CHARACTER_SIZE = 4
    };
    CVector3 m_CollisionCharacterSize[NUM_COLLISION_CHARACTER_SIZE];
    CVector3 m_CollisionHurtSize[NUM_COLLISION_CHARACTER_SIZE];

	class CBoneWeight
	{
	public:
		CBoneWeight();
		std::string m_Bone;
		float m_fWeight;
		bool m_bApplyChildren;
	};

	class CBlendNode
	{
	public:
		CBlendNode();
		std::string m_Name;
		std::string m_Class;
		std::string m_Anim;
		int m_iParent;
		float m_fWeight;
		float m_fSpeed;
		enum
		{
			MAIN = 1 << 0,
			SYNC = 1 << 1,
			ADDITIVE = 1 << 2,
		};
		int m_iFlags;
		std::vector<CBoneWeight> m_BoneWeights;
	};
	std::vector<CBlendNode> m_BlendNodes;

private:

    CCharacterDescManager *m_pManager;
};



}//GELib


#endif//__GELIB_CHARACTER_CHARACTERDESC_H__
