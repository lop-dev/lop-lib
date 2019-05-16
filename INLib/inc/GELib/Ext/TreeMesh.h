//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/TreeMesh.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_TREEMESH_H__
#define __GELIB_EXT_TREEMESH_H__

#include <vector>
#include <string>

#include <GELib/Renderer/RenderMesh.h>
#include "LocalMesh.h"

namespace GELib
{

class CIOStream;
class CStdMaterial;


class CTreeMeshShaderParameters : public CMeshShaderParameters
{
public:
    CShaderParameter m_WorldMatrix;
    CShaderParameter m_WorldInvMatrix;
    CShaderParameter m_PrevWorldMatrix;
    CShaderParameter m_TreeWind;
    CShaderParameter m_TreeBend;
};

class CTreeMeshShaderData
{
public:
    CMatrix m_WorldTMatrix;
    CMatrix m_WorldInvTMatrix;
    CMatrix m_PrevWorldTMatrix;
    CVector4 m_MaterialParameters[2];
    CVector4 m_Wind;
    CVector4 m_Bend;
};

const int MESHCLASS_TREE = 6;

class CTreeMesh : public CLocalMesh
{
public:

    CTreeMesh();
    virtual ~CTreeMesh();
    virtual int GetClassId() { return MESHCLASS_TREE; }

    // called by CRenderer
    virtual const char * GetShaderName();
    virtual void CreateShaderParameters(CMeshShader *shader);
    virtual void SetShaderParameters(CMeshShader *shader, CRendererContext &rc);

protected:


};



}//GELib

#endif//__GELIB_EXT_TREEMESH_H__
