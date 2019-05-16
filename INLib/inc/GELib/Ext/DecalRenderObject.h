//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/DecalRenderObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_DECALRENDEROBJECT_H__
#define __GELIB_EXT_DECALRENDEROBJECT_H__

#include "BaseRenderObject.h"
#include "DecalMesh.h"
#include "MeshAsset.h"


namespace GELib
{

class CRenderer;
class CLightMapInfo;
class CMaterialAsset;

class CDecalRenderObject : public CBaseRenderObject
{
public:
    CDecalRenderObject();
    virtual ~CDecalRenderObject();

    CDecalMesh* GetMesh() { return &m_Mesh; }
    void SetMaterial(CMaterialAsset *material) { m_pMaterial = material; }
    CMaterialAsset* GetMaterial() { return m_pMaterial; }

    void SetMatrix(const CMatrix &matrix);

    void ComputeVisibleDistance(float ratio);
    void SetFadeDistance(float d) { m_fFadeDistance = d; }
    float GetFadeDistance() { return m_fFadeDistance; }
    void SetOpacity(float o) { m_fObjOpacity = o; }
    float GetOpacity() { return m_fObjOpacity; }

    virtual void Render(CRenderer *renderer);
    virtual void UpdateRenderData(CRenderer *renderer);

    void SetLightMap(CLightMapInfo *lm);
    CLightMapInfo* GetLightMap() { return m_pLightMap; }

private:

    void UpdateBoundingBox();

    float m_fFadeDistance;
    float m_fObjOpacity;        // m_fOpacity is already defined in CRenderObject
    CMatrix m_Matrix;
    CDecalMesh m_Mesh;
    CMaterialAsset *m_pMaterial;
    CLocalMeshShaderData m_ShaderData;
    GELib::CLightMapInfo *m_pLightMap;
};


}//GELib

#endif//__GELIB_EXT_DECALRENDEROBJECT_H__
