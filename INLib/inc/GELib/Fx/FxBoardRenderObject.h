//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxBoardRenderObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXBOARDRENDEROBJECT_H__
#define __GELIB_FX_FXBOARDRENDEROBJECT_H__

#include <GELib/Ext/BaseRenderObject.h>
#include "FxBoardMesh.h"

namespace GELib
{


class CFxBoardComponent;

class CFxBoardRenderObject : public CBaseRenderObject
{
public:
    CFxBoardRenderObject();
    virtual ~CFxBoardRenderObject();

    void SetComponent(CFxBoardComponent *c);
    void SetMaterial(CMaterialAsset *material);

    void SetMatrix(const CMatrix &matrix);
    void SetFaceCamera(EFaceCameraMode face);

    virtual void Render(CRenderer *renderer);
    virtual void UpdateRenderData(CRenderer *renderer);

private:

    void UpdateBoundingBox();

    CMatrix m_Matrix;
    CFxBoardMesh m_Mesh;
    CMaterialAsset *m_pMaterial;
    CFxBoardComponent *m_pComponent;
    CLocalMeshShaderData m_ShaderData;
};



}//GELib

#endif//__GELIB_FX_FXBOARDRENDEROBJECT_H__
