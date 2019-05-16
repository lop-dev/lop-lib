//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/InstanceMesh.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_INSTANCEMESH_H__
#define __GELIB_EXT_INSTANCEMESH_H__

#include <vector>
#include <string>

#include "LocalMesh.h"

namespace GELib
{

class CIOStream;


const int MAX_MESH_INSTANCES = 72;		// = InstanceMesh.hlsl MAX_MESH_INSTANCES

class CInstanceMeshShaderParameters : public CMeshShaderParameters
{
public:
    CShaderParameter m_WorldMatrix;
    CShaderParameter m_WindVector;
    CShaderParameter m_Collider;
    CShaderParameter m_Size;
    CShaderParameter m_VisibleCount;
};

class CInstanceMeshShaderData
{
public:
    float *m_pWorldTMatrix;
    CVector4 m_WindVector;
    CVector4 m_MaterialParameters[2];
	CVector4 m_Collider;
	CVector2 m_Size;
	int m_iNumInstances;
};

const int MESHCLASS_INSTANCE = 7;

class CInstanceMesh : public CLocalMesh
{
public:

    CInstanceMesh();
    virtual ~CInstanceMesh();
    virtual int GetClassId() { return MESHCLASS_INSTANCE; }

    virtual void Unload();
	virtual bool CreateLoadedResource();

	void EnableWind(bool enable) { m_bEnableWind = enable; }
	bool IsEnableWind() { return m_bEnableWind; }

    // called by CRenderer
    virtual const char * GetShaderName();
    virtual int GetShaderHash();
    virtual void CreateShaderParameters(CMeshShader *shader);
    virtual void SetShaderParameters(CMeshShader *shader, CRendererContext &rc);
    virtual void SetBuffers(CRendererContext &rc);
    virtual int DrawChunk(int chunk, CRendererContext &rc);

private:

	bool m_bEnableWind;
    int m_iInstanceVertexBuffer;
    int m_iInstanceIndexBuffer;
};

}//GELib

#endif//__GELIB_EXT_INSTANCEMESH_H__
