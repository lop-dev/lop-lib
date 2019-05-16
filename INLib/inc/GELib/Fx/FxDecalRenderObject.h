//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxDecalRenderObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXDECALRENDEROBJECT_H__
#define __GELIB_FX_FXDECALRENDEROBJECT_H__

#include <GELib/Ext/BaseRenderObject.h>
#include <GELib/Ext/LocalMesh.h>
#include <GELib/Ext/DecalMesh.h>


namespace GELib
{

class CRenderer;
class CMaterialAsset;
class CFxDecalComponent;

class CFxDecalRenderObject : public CBaseRenderObject
{
public:
    CFxDecalRenderObject();
    virtual ~CFxDecalRenderObject();

    void SetComponent(CFxDecalComponent *c);

    CDecalMesh* GetMesh() { return &m_Mesh; }
    void SetMaterial(CMaterialAsset *material) { m_pMaterial = material; }
    CMaterialAsset* GetMaterial() { return m_pMaterial; }

    void SetMatrix(const CMatrix &matrix);

    virtual void Render(CRenderer *renderer);
    virtual void UpdateRenderData(CRenderer *renderer);

private:

    void UpdateBoundingBox();

	CFxDecalComponent *m_pComponent;
    CMatrix m_Matrix;
    CDecalMesh m_Mesh;
    CMaterialAsset *m_pMaterial;
    CLocalMeshShaderData m_ShaderData;
};


}//GELib

#endif//__GELIB_FX_FXDECALRENDEROBJECT_H__
