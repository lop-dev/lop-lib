//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxAreaRenderObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXAREARENDEROBJECT_H__
#define __GELIB_FX_FXAREARENDEROBJECT_H__

#include <GELib/Ext/BaseRenderObject.h>
#include "FxAreaMesh.h"

namespace GELib
{


class CFxAreaComponent;

class CFxAreaRenderObject : public CBaseRenderObject
{
public:
    CFxAreaRenderObject();
    virtual ~CFxAreaRenderObject();

    void SetComponent(CFxAreaComponent *c);
    void SetMaterial(CMaterialAsset *material);

    virtual void Render(CRenderer *renderer);
    virtual void UpdateRenderData(CRenderer *renderer);

private:

    CFxAreaMesh m_Mesh;
    CMaterialAsset *m_pMaterial;
    CFxAreaComponent *m_pComponent;
    CLocalMeshShaderData m_ShaderData;
};



}//GELib

#endif//__GELIB_FX_FXAREARENDEROBJECT_H__
