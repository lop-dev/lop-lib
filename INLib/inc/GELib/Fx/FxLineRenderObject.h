//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxLineRenderObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXLINERENDEROBJECT_H__
#define __GELIB_FX_FXLINERENDEROBJECT_H__

#include <GELib/Ext/BaseRenderObject.h>
#include "FxTrailMesh.h"

namespace GELib
{

class CFxLineComponent;

class CFxLineRenderObject : public CBaseRenderObject
{
public:
    CFxLineRenderObject();
    virtual ~CFxLineRenderObject();

    void SetComponent(CFxLineComponent *c);
    void SetMaterial(CMaterialAsset *material);

	void SetFaceCamera(bool f);

    virtual void Render(CRenderer *renderer);
    virtual void UpdateRenderData(CRenderer *renderer);

private:

    CFxTrailMesh m_Mesh;
    CMaterialAsset *m_pMaterial;
    CFxLineComponent *m_pComponent;
    CLocalMeshShaderData m_ShaderData;
};



}//GELib

#endif//__GELIB_FX_FXLINERENDEROBJECT_H__
