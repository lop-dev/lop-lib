//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/PrimitiveRenderObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_PRIMITIVERENDEROBJECT_H__
#define __GELIB_EXT_PRIMITIVERENDEROBJECT_H__

#include <GELib/Ext/BaseRenderObject.h>
#include "PrimitiveMesh.h"

namespace GELib
{


class CPrimitiveRenderObject : public CBaseRenderObject
{
public:
    CPrimitiveRenderObject();
    virtual ~CPrimitiveRenderObject();

	void SetMesh(CPrimitiveMesh *mesh);
	CPrimitiveMesh* GetMesh() { return m_pMesh; }

	void SetMaterial(CMaterialAsset *material);
	CMaterialAsset* GetMaterial() { return m_pMaterial; }

    void SetMatrix(const CMatrix &matrix);

    virtual void Render(CRenderer *renderer);
    virtual void UpdateRenderData(CRenderer *renderer);

private:

    void UpdateBoundingBox();

    CMatrix m_Matrix;
    CPrimitiveMesh *m_pMesh;
    CMaterialAsset *m_pMaterial;
    CLocalMeshShaderData m_ShaderData;
};



}//GELib

#endif//__GELIB_EXT_PRIMITIVERENDEROBJECT_H__
