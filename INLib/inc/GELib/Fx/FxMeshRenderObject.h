//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxMeshRenderObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXMESHRENDEROBJECT_H__
#define __GELIB_FX_FXMESHRENDEROBJECT_H__

#include <GELib/Ext/BaseRenderObject.h>
#include <GELib/Ext/LocalMesh.h>

namespace GELib
{

class CMeshAsset;
class CMaterialAsset;
class CFxComponent;

class CFxMeshRenderObject : public CBaseRenderObject
{
public:
    CFxMeshRenderObject();
    virtual ~CFxMeshRenderObject();

    void SetMatrix(const CMatrix &matrix);
    void SetFaceCamera(EFaceCameraMode face);

    void SetComponent(CFxComponent *c);
    void SetMesh(CMeshAsset *mesh);
    void SetMaterial(CMaterialAsset *material);
    void SetParticleIndex(int index);

    virtual void Render(CRenderer *renderer);
    virtual void UpdateRenderData(CRenderer *renderer);

private:

    void UpdateBoundingBox();

    CFxComponent *m_pComponent;
    int m_iParticleIndex;
    CMatrix m_Matrix;
    CMeshAsset *m_pMesh;
    CMaterialAsset *m_pMaterial;
    CLocalMeshShaderData m_ShaderData;
};



}//GELib

#endif//__GELIB_FX_FXMESHRENDEROBJECT_H__
