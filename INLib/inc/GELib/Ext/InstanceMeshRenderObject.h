//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/InstanceMeshRenderObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_INSTANCEMESHRENDEROBJECT_H__
#define __GELIB_EXT_INSTANCEMESHRENDEROBJECT_H__

#include <vector>

#include "BaseRenderObject.h"
#include "MeshAsset.h"
#include "InstanceMesh.h"


namespace GELib
{

class CRenderer;
class CLightMapInfo;
class CColladaFile;


class CMeshUtility;
class CMaterialAsset;

class CInstanceMeshData
{
public:
    short m_Offset[3];
    char m_Rotation[3];
    unsigned char m_Scale;

    void FromMatrix(const CVector3 &basePosition, const CMatrix &m);
    void ToMatrix(const CVector3 &basePosition, CMatrix &m) const;
    void ToPositionRotationScale(const CVector3 &basePosition, CMatrix &posRot, float &scale) const;
    void ToPosition(const CVector3 &basePosition, CVector3 &p) const;
};
typedef std::vector<CInstanceMeshData> CInstanceMeshDataList;

class CInstanceMeshWindParameter
{
public:
    CInstanceMeshWindParameter();
    float m_fStrength;
    float m_fFrequency;
    float m_fDetail;
	CVector4 m_Collider;
};

class CInstanceMeshRenderObject : public CBaseRenderObject
{
public:
    CInstanceMeshRenderObject();
    virtual ~CInstanceMeshRenderObject();

    void SetMesh(CMeshAsset *mesh);
    CMeshAsset* GetMesh() { return m_pMesh; }

    virtual void Render(CRenderer *renderer);
    virtual void UpdateRenderData(CRenderer *renderer);

    void SetLightMap(CLightMapInfo *lm);
    CLightMapInfo* GetLightMap() { return m_pLightMap; }

    void SetBasePosition(const CVector3 &p) { m_BasePosition = p; }
    const CVector3& GetBasePosition() { return m_BasePosition; }
    CInstanceMeshDataList& GetInstances() { return m_Instances; }
    void UpdateInstances();
    void UpdateBoundingBox();

    void SetFadeDistance(float d) { m_fFadeDistance = d; }
    float GetFadeDistance() { return m_fFadeDistance; }

    void SetWindParameter(CInstanceMeshWindParameter *w) { m_pWindParameter = w; }

private:

    void DeleteMesh();

    CMeshAsset *m_pMesh;
    CInstanceMeshDataList m_Instances;
    std::vector<CInstanceMeshShaderData> m_ShaderData;
    CLightMapInfo *m_pLightMap;
    float m_fFadeDistance;
    CVector3 m_BasePosition;
    CInstanceMeshWindParameter *m_pWindParameter;
	int m_iTempMemoryKey;
};


}//GELib

#endif//__GELIB_EXT_INSTANCEMESHRENDEROBJECT_H__
