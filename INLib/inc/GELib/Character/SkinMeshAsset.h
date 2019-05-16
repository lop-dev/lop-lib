//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/SkinMeshAsset.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_SKINMESHASSET_H__
#define __GELIB_CHARACTER_SKINMESHASSET_H__

#include <vector>
#include <string>

#include <GELib/Ext/BaseAsset.h>
#include <GELib/Math/GeMath.h>
#include "LodSkinMesh.h"

namespace GELib
{

class CIOStream;
class CMeshUtility;
class CSkinMeshManager;

const int ASSETCLASS_SKIN_MESH = 7;


class CSkinMeshAsset : public CBaseAsset
{
public:
    CSkinMeshAsset(CSkinMeshManager *manager, const char *cacheKey=0);
    virtual ~CSkinMeshAsset();

	virtual int GetClassId() { return ASSETCLASS_SKIN_MESH; }
    CSkinMeshManager* GetManager() { return m_pManager; }
    virtual void ManagerDelete();

    bool Save(CIOStream &stream);
    bool Load(CIOStream &stream, const CLoadParameter &param);
    bool Import(const std::vector< std::vector<CMeshUtility*> > &meshUtils, const CLoadParameter &param);
    void Unload();
	virtual bool CreateLoadedResource();

    int GetNumMeshes();
    CLodSkinMesh* GetMesh(int index);
    const CAABox& GetBoundingBox() { return m_Box; }

    class CBone
    {
    public:
        std::string m_Name;
        CMatrix m_BindInvMatrix;
        CAABox m_Box;
        char m_iMaxLod;
    };

    int GetNumBones() { return (int)m_Bones.size(); }
    CBone* GetBone(int index) { return &m_Bones[index]; }
    int AppendUniqueBone(const std::string &name, const CMatrix &bindInvMatrix, const CAABox &box, int lod); // return index

    void SetMaxVisibleDistanceRatio(float s) { m_fMaxVisibleDistanceRatio = s; }
    float GetMaxVisibleDistanceRatio() { return m_fMaxVisibleDistanceRatio; }
    void SetMinVisibleDistanceRatio(float s) { m_fMinVisibleDistanceRatio = s; }
    float GetMinVisibleDistanceRatio() { return m_fMinVisibleDistanceRatio; }
    void SetDynamicShadowDistanceRatio(float s) { m_fDynamicShadowDistanceRatio = s; }
    float GetDynamicShadowDistanceRatio() { return m_fDynamicShadowDistanceRatio; }
    void SetAlwaysVisibleDistance(float s) { m_fAlwaysVisibleDistance = s; }
    float GetAlwaysVisibleDistance() { return m_fAlwaysVisibleDistance; }
    void SetInvisibleCastShadow(bool cast) { m_bInvisibleCastShadow = cast; }
    bool GetInvisibleCastShadow() { return m_bInvisibleCastShadow; }

private:

    CAABox m_Box;
    float m_fMaxVisibleDistanceRatio;
    float m_fMinVisibleDistanceRatio;
    float m_fDynamicShadowDistanceRatio;
    float m_fAlwaysVisibleDistance;
    bool m_bInvisibleCastShadow;

    std::vector<CLodSkinMesh> m_Meshes;
    std::vector<CBone> m_Bones;

    CSkinMeshManager *m_pManager;
	bool m_bNeedCreateLoadedResource;
};

inline int CSkinMeshAsset::GetNumMeshes()
{
    return (int)m_Meshes.size();
}

inline CLodSkinMesh* CSkinMeshAsset::GetMesh(int index)
{
    //assert(index>=0 && index<GetNumMeshes());
    if (index>=0 && index<GetNumMeshes())
        return &m_Meshes[index];
    return 0;
}



}//GELib

#endif//__GELIB_CHARACTER_SKINMESHASSET_H__
