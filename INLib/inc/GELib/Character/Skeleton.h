//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/Skeleton.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_SKELETON_H__
#define __GELIB_CHARACTER_SKELETON_H__

#include <vector>
#include <GELib/Math/GeMath.h>
#include <GELib/Utility/StringMap.h>
#include "AnimationAsset.h"
#include "AnimationSample.h"

namespace GELib
{

class IPlotter2D;
class IPlotter3D;


class CBone
{
public:
	CBone();
	const std::string& GetName();

	CAnimationAsset::CTrack* GetRefBone();
	CAnimationAsset *m_pRefSkeleton;
	short m_iRefBoneIndex;

	short m_iIndex;
	CBone *m_pParent;
	std::vector<CBone*> m_Children;
	CMatrix m_LocalMatrix;
	CMatrix m_Matrix;

	enum EBoneFlag
	{
		BF_TRANSLATE_ANIM = 1 << 0,
		BF_SCALE_ANIM = 1 << 1,
		BF_MOVE_BONE = 1 << 2
	};
	int m_iFlags;
};

class CSkeleton
{
public:
    CSkeleton();
    virtual ~CSkeleton();

    // skeleton
    bool LoadSkeleton(const char *name, int index, const CLoadParameter &param);
    void SetSkeleton(CAnimationAsset *skeleton, int index);
    CAnimationAsset* GetSkeleton(int index);
    int GetNumSkeletons();
    void DeleteSkeleton(int index);
    bool IsLoading();
    void CheckBuildBones();

    void InitializeAnimationSamples(AnimationSampleList &samples);
    void ComputeBoneLocalMatrix(AnimationSampleList &samples);
    void ComputeBoneGlobalMatrix();

	void UpdateMoveBoneOnly(bool b) { m_bUpdateMoveBoneOnly = b; }
	bool IsUpdateMoveBoneOnly() { return m_bUpdateMoveBoneOnly; }

    void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    // CBone* is invalid if skeleton changed
    int GetNumBones();
    CBone* GetBone(int index);
    CBone* GetBoneByName(const char *name);

	// root matrix
    const CMatrix& GetMatrix() const;
    void SetMatrix(const CMatrix &m);

private:

    void BuildBones();

    bool m_bLoading;
    bool m_bNeedBuildBones;
	bool m_bUpdateMoveBoneOnly;

    std::vector<CAnimationAsset*> m_RefSkeletons;
    std::vector<CBone> m_Bones;
	typedef StringVectorMap<CBone*> BoneNameMap;
    BoneNameMap m_BoneNameMap;
};


}//GELib

#endif//__GELIB_CHARACTER_SKELETON_H__
