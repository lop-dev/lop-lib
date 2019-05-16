//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/MeshAsset.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_MESHASSET_H__
#define __GELIB_EXT_MESHASSET_H__

#include <vector>
#include "LocalMesh.h"
#include "CollisionMesh.h"
#include "BaseAsset.h"

namespace GELib
{


class CIOStream;
class CStdMaterial;
class CMeshUtility;
class CMeshManager;

const int ASSETCLASS_MESH = 4;


class CMeshAsset : public CBaseAsset
{
public:
    CMeshAsset(CMeshManager *manager, const char *cacheKey=0);
    virtual ~CMeshAsset();

	virtual int GetClassId() { return ASSETCLASS_MESH; }
    CMeshManager* GetManager() { return m_pManager; }
    virtual void ManagerDelete();

    enum ERenderType
    {
        RT_LOCAL = 0,
        RT_TREE,
        RT_INSTANCE,
        RT_MAX
    };

	void SetRenderType(ERenderType t) { m_eRenderType = t; }		// set before load
    ERenderType GetRenderType() const { return m_eRenderType; }

    bool Save(CIOStream &stream);
    bool Load(CIOStream &stream, const CLoadParameter &param);
    bool Import(const std::vector<CMeshUtility*> &meshUtils, const CLoadParameter &param);
    void Unload();
	virtual bool CreateLoadedResource();

    int GetNumLods();
    CLocalMesh* GetMesh(int lod);
    int GetLodByRangeRatio(float r);
    const CAABox& GetBoundingBox() const { return m_Box; }
    CCollisionMesh* GetCollisionMesh() { return &m_CollisionMesh; }

    void SetMaxVisibleDistanceRatio(float r) { m_fMaxVisibleDistanceRatio = r; }
    float GetMaxVisibleDistanceRatio() const { return m_fMaxVisibleDistanceRatio; }
    void SetNestedLodDistanceRatio(float r) { m_fNestedLodDistanceRatio = r; }
    float GetNestedLodDistanceRatio() const { return m_fNestedLodDistanceRatio; }
    void SetDynamicShadowDistanceRatio(float r) { m_fDynamicShadowDistanceRatio = r; }
    float GetDynamicShadowDistanceRatio() const { return m_fDynamicShadowDistanceRatio; }
    void SetAlwaysVisibleDistance(float r) { m_fAlwaysVisibleDistance = r; }
    float GetAlwaysVisibleDistance() const { return m_fAlwaysVisibleDistance; }
    void SetInvisibleCastShadow(bool cast) { m_bInvisibleCastShadow = cast; }
    bool GetInvisibleCastShadow() const { return m_bInvisibleCastShadow; }
    void SetLodRangeRatio(int lod, float r);
    float GetLodRangeRatio(int lod) const;

    void GenerateUVAtlas(int texCoordIndex);

    class CTreeParameter
    {
    public:
        CTreeParameter() { m_fTrunkBend = m_fBranchBend = m_fLeafBend = m_fLeafDetail = 0;
                           m_fTrunkFrequency = m_fBranchFrequency = m_fLeafFrequency = 1.0f; }
        float m_fTrunkBend;
        float m_fTrunkFrequency;
        float m_fBranchBend;
        float m_fBranchFrequency;
        float m_fLeafBend;
        float m_fLeafFrequency;
        float m_fLeafDetail;
    };

    void SetTreeParameter(const CTreeParameter &p) { m_TreeParameter = p; }
    const CTreeParameter& GetTreeParameter() const { return m_TreeParameter; }
	void EnableWind(bool enable);	// instance mesh wind
	bool IsEnableWind();

private:

    CAABox m_Box;
    float m_fMaxVisibleDistanceRatio;
    float m_fNestedLodDistanceRatio;
    float m_fDynamicShadowDistanceRatio;
    float m_fAlwaysVisibleDistance;
    bool m_bInvisibleCastShadow;

    CTreeParameter m_TreeParameter;

    class CLod
    {
    public:
        CLocalMesh *m_pMesh;
        float m_fRangeRatio;
    };
    std::vector<CLod> m_Lods;
    ERenderType m_eRenderType;

    CCollisionMesh m_CollisionMesh;

    CMeshManager *m_pManager;
	bool m_bNeedCreateLoadedResource;
};


inline int CMeshAsset::GetNumLods()
{
    return (int)m_Lods.size();
}

inline CLocalMesh* CMeshAsset::GetMesh(int lod)
{
    //assert(lod>=0 && lod<GetNumLods());
    if (lod>=0 && lod<GetNumLods())
        return m_Lods[lod].m_pMesh;
    return 0;
}



}//GELib

#endif//__GELIB_EXT_MESHASSET_H__
