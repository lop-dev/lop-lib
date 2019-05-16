//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxTrailRenderObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXTRAILRENDEROBJECT_H__
#define __GELIB_FX_FXTRAILRENDEROBJECT_H__

#include <GELib/Ext/BaseRenderObject.h>
#include "FxTrailMesh.h"

namespace GELib
{

class CFxTrailComponent;

class CFxTrailRenderObject : public CBaseRenderObject
{
public:
    CFxTrailRenderObject();
    virtual ~CFxTrailRenderObject();

    void SetComponent(CFxTrailComponent *c);
    void SetMaterial(CMaterialAsset *material);

	void SetFaceCamera(bool f);

    virtual void Render(CRenderer *renderer);
    virtual void UpdateRenderData(CRenderer *renderer);

private:

    CFxTrailMesh m_Mesh;
    CMaterialAsset *m_pMaterial;
    CFxTrailComponent *m_pComponent;
    CLocalMeshShaderData m_ShaderData;
};



}//GELib

#endif//__GELIB_FX_FXTRAILRENDEROBJECT_H__
