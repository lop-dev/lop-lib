//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/SkinMeshRenderObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_SKINMESHRENDEROBJECT_H__
#define __GELIB_CHARACTER_SKINMESHRENDEROBJECT_H__

#include <vector>
#include <bitset>
#include <GELib/Ext/BaseRenderObject.h>
#include "Skeleton.h"
#include "SkinMesh.h"

namespace GELib
{

class IPlotter3D;
class CSkinMeshAsset;
class CLightMapInfo;


class CSkinMeshRenderObject : public CBaseRenderObject
{
public:
    CSkinMeshRenderObject();
    virtual ~CSkinMeshRenderObject();

    virtual void Render(CRenderer *renderer);
    virtual void UpdateRenderData(CRenderer *renderer);

    void Draw(IPlotter3D &plotter);

    void SetSkeleton(CSkeleton *a);
    CSkeleton* GetSkeleton(CSkeleton *a) { return m_pSkeleton; }

    bool LoadSkin(const char *name, int index, const CLoadParameter &param);
    void SetSkin(CSkinMeshAsset *skin, int index);
    CSkinMeshAsset* GetSkin(int index);
    int GetNumSkins();
    void DeleteSkin(int index);

    void ShowSkin(int index, bool show);
    bool IsShowSkin(int index);
    void ShowSkinMesh(int skinIndex, int meshIndex, bool show);
    bool IsShowSkinMesh(int skinIndex, int meshIndex);

	void SetMaterialParameter(int index, const CVector4 &p);	// index = 0, 1

    bool IsLoading();
    bool CreateLoadedResource();

	int GetNumLods(int skinIndex, int meshIndex);
    void SetDefaultLod(int lod) { m_iDefaultLod = (char)lod; }
    void SetLodRangeRatio(int lod, float r);
    float GetLodRangeRatio(int lod) const;
    void SetMaxVisibleDistanceRatio(float r) { m_fMaxVisibleDistanceRatio = r; }
    float GetMaxVisibleDistanceRatio() const { return m_fMaxVisibleDistanceRatio; }
    void SetDynamicShadowDistanceRatio(float r) { m_fDynamicShadowDistanceRatio = r; }
    float GetDynamicShadowDistanceRatio() const { return m_fDynamicShadowDistanceRatio; }
    void ComputeVisibleDistance();
    void SetFadeDistance(float d) { m_fFadeDistance = d; }
    float GetFadeDistance() { return m_fFadeDistance; }
    void SetOpacity(float o) { m_fObjOpacity = o; }
    float GetOpacity() { return m_fObjOpacity; }

	const CAABox& GetBindPoseBoundingBox() { return m_BindPoseBox; }

    void SetLightMap(int lod, CLightMapInfo *lm);
    CLightMapInfo* GetLightMap(int lod=0) { return m_pLightMaps[lod]; }

	bool IsCulled();
	char GetCullCounter() { return m_iCullCounter; }

private:

	int GetLodByRangeRatio(float r);

    bool m_bLoading;
    bool m_bNeedBindSkinToSkeleton;
    char m_iDefaultLod;
	char m_iCullCounter;
	float m_fMaxVisibleDistanceRatio;
	float m_fDynamicShadowDistanceRatio;
	enum
	{
		LOD_NUM = 3
	};
	float m_LodRangeRatios[LOD_NUM];
    float m_fFadeDistance;
    float m_fObjOpacity;        // m_fOpacity is already defined in CRenderObject

    CSkeleton *m_pSkeleton;

    void CheckBind();
    void BindSkinToSkeleton();
    void ComputeBindPoseBoundingBox();

    class CSkinSkeletonBinding      // skin bone to skeleton bone mapping
    {
    public:
        CSkinSkeletonBinding();
        void SetMeshVisible(int index, bool visible);
        bool IsMeshVisible(int index);

        CSkinMeshAsset *m_pSkin;
        bool m_bShow;

        CSkinMeshShaderData m_ShaderData;
        std::bitset<128> m_MeshVisibleBits;
        std::vector<short> m_SkeletonBoneIndices;
        std::vector<float> m_SkinMatrices;
    };
    std::vector<CSkinSkeletonBinding> m_Binds;
    CAABox m_BindPoseBox;

    // shader parameter
    CMatrix m_WorldTMatrix;
    CMatrix m_WorldInvTMatrix;
    CMatrix m_PrevWorldTMatrix;
	CVector4 m_MaterialParameters[2];
	CVector4 m_SetMaterialParameters[2];

    CLightMapInfo *m_pLightMaps[LIGHTMAP_LOD_MAX];
};


}//GELib

#endif//__GELIB_CHARACTER_SKINMESHRENDEROBJECT_H__
