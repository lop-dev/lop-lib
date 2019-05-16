//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/TerrainMesh.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_TERRAINMESH_H__
#define __GELIB_EXT_TERRAINMESH_H__

#include <GELib/Renderer/RenderMesh.h>
#include "Terrain.h"

namespace GELib
{

class CIOStream;
class CStdMaterial;


class CTerrainMeshShaderParameters : public CMeshShaderParameters
{
public:
    CShaderParameter m_Position;
    CShaderParameter m_Scale;
    CShaderParameter m_GlobalTextureOffsetScale;
    CShaderParameter m_SectionTextureOffsetScale;
};

class CTerrainMeshShaderData
{
public:
    CVector3 m_Position;
    CVector4 m_Scale;
    CVector4 m_GlobalTextureOffsetScale;
    CVector4 m_SectionTextureOffsetScale;
};

const int MESHCLASS_TERRAIN = 8;

enum ETerrainMeshMode
{
	TERRAIN_MESH_DEFAULT,
	TERRAIN_MESH_FULL_RESOLUTION,
};

class CTerrainMesh : public CRenderMesh
{
public:

    CTerrainMesh();
    virtual ~CTerrainMesh();
    virtual int GetClassId() { return MESHCLASS_TERRAIN; }

    // called by CRenderer
    virtual const char * GetShaderName();
    virtual int GetUVAtlasTexCoordIndex();
    virtual void CreateShaderParameters(CMeshShader *shader);
    virtual void SetShaderParameters(CMeshShader *shader, CRendererContext &rc);
    virtual void SetBuffers(CRendererContext &rc);
    virtual int DrawChunk(int chunk, CRendererContext &rc);

	void UpdateBuffer(CTerrain *terrain, int lod, int patchX, int patchY, float skirtHeight, bool heightMapDirty, ETerrainMeshMode mode);

protected:

    int m_iVertexDecl;
    int m_iVertexBufferXY;
    int m_iVertexBufferZ;
    int m_IndexBuffer[TERRAIN_EXLODS];
    int m_iVertexStride;
    int m_iPatchSize;
	ETerrainMeshMode m_eMode;
	int m_iTempVertexBufferKey;
};


}//GELib

#endif//__GELIB_EXT_TERRAINMESH_H__
