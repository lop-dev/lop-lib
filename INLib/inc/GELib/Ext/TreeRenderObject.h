//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/TreeRenderObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_TREERENDEROBJECT_H__
#define __GELIB_EXT_TREERENDEROBJECT_H__

#include "BaseRenderObject.h"
#include "MeshAsset.h"
#include "TreeMesh.h"


namespace GELib
{

class CConvexVolume;
class CRenderer;
class CLightMapInfo;

class CTreeState
{
public:
    CTreeState();
    CVector3 m_Wind;
    float m_fTrunkTime;
    float m_fBranchTime;
    float m_fLeafTime;
    float m_fVariation;
};

class CTreeRenderObject : public CBaseRenderObject
{
public:
    CTreeRenderObject();
    virtual ~CTreeRenderObject();

    bool LoadMesh(const char *fname, const CLoadParameter &param);
    bool IsLoading();
	bool CreateLoadedResource();

	void SetMesh(CMeshAsset *mesh);
    CMeshAsset* GetMesh() { return m_pMesh; }

    CTreeState& GetTreeState() { return m_TreeState; }

    void SetMatrix(const CMatrix &matrix);
	const CMatrix& GetMatrix() { return m_Matrix; }

    void SetDefaultLod(int lod) { m_iDefaultLod = lod; }
    void ComputeVisibleDistance();
    void SetFadeDistance(float d) { m_fFadeDistance = d; }
    float GetFadeDistance() { return m_fFadeDistance; }
    void SetOpacity(float o) { m_fObjOpacity = o; }
    float GetOpacity() { return m_fObjOpacity; }

    virtual void Render(CRenderer *renderer);
    virtual void UpdateRenderData(CRenderer *renderer);

    bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
    bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);
    bool IntersectConvexVolume(const CConvexVolume &c);

    void SetLightMap(int lod, CLightMapInfo *lm);
    CLightMapInfo* GetLightMap(int lod=0) { return m_pLightMaps[lod]; }

private:

    void UpdateBoundingBox();
    void DeleteMesh();

    float m_fFadeDistance;
    float m_fObjOpacity;        // m_fOpacity is already defined in CRenderObject
    char m_iDefaultLod;
    CMatrix m_Matrix;
    CMeshAsset *m_pMesh;
    CTreeMeshShaderData m_ShaderData;
    CLightMapInfo *m_pLightMaps[LIGHTMAP_LOD_MAX];
    CTreeState m_TreeState;
};


}//GELib

#endif//__GELIB_EXT_TREERENDEROBJECT_H__
