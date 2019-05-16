//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxInstanceMeshRenderObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXINSTANCEMESHRENDEROBJECT_H__
#define __GELIB_FX_FXINSTANCEMESHRENDEROBJECT_H__

#include <vector>

#include <GELib/Ext/BaseRenderObject.h>
#include <GELib/Ext/MeshAsset.h>
#include <GELib/Ext/InstanceMesh.h>


namespace GELib
{

class CMeshAsset;
class CMaterialAsset;
class CFxComponent;

class CFxInstanceMeshRenderObject : public CBaseRenderObject
{
public:
    CFxInstanceMeshRenderObject();
    virtual ~CFxInstanceMeshRenderObject();

    void SetComponent(CFxComponent *c);
    void SetMesh(CMeshAsset *mesh);
    void SetMaterial(CMaterialAsset *material);
	void SetMaxInstances(int n);

    virtual void Render(CRenderer *renderer);
    virtual void UpdateRenderData(CRenderer *renderer);

private:

    CFxComponent *m_pComponent;
    CMeshAsset *m_pMesh;
    CMaterialAsset *m_pMaterial;
    std::vector<CInstanceMeshShaderData> m_ShaderData;
	std::vector<float> m_InstanceMatrix;
};


}//GELib

#endif//__GELIB_FX_FXINSTANCEMESHRENDEROBJECT_H__
