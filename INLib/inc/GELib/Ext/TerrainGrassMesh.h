//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/TerrainGrassMesh.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_TERRAINGRASSMESH_H__
#define __GELIB_EXT_TERRAINGRASSMESH_H__

#include <vector>
#include <string>

#include <GELib/Renderer/RenderMesh.h>
#include "TerrainGrass.h"

namespace GELib
{

class CIOStream;
class CStdMaterial;



class CTerrainGrassMeshShaderParameters : public CMeshShaderParameters
{
public:
    CShaderParameter m_CameraPosition;
    CShaderParameter m_TerrainPosition;
    CShaderParameter m_Offset;
    CShaderParameter m_TextureOffsetScale;
    CShaderParameter m_GrassParameter;
    CShaderParameter m_Collider;
};

class CTerrainGrassMeshShaderData
{
public:
    CVector3 m_TerrainPosition;
    CVector3 m_Offset;
    CVector4 m_TextureOffsetScale;
    CVector4 m_GrassParameter[TERRAIN_GRASS_INFO_MAX];
    CVector4 m_Collider;	// xyz: position, w: radius
};

const int MESHCLASS_TERRAIN_GRASS = 9;

class CTerrain;


class CTerrainGrassMesh : public CRenderMesh
{
public:

    CTerrainGrassMesh();
    virtual ~CTerrainGrassMesh();
    virtual int GetClassId() { return MESHCLASS_TERRAIN_GRASS; }

    // called by CRenderer
    virtual const char * GetShaderName();
    virtual int GetUVAtlasTexCoordIndex();
    virtual void CreateShaderParameters(CMeshShader *shader);
    virtual void SetShaderParameters(CMeshShader *shader, CRendererContext &rc);
    virtual void SetBuffers(CRendererContext &rc);
    virtual int DrawChunk(int chunk, CRendererContext &rc);

	void UpdateBuffer(CTerrain *terrain, int cellX, int cellY);

protected:

    int m_iVertexDecl;
    int m_iVertexBuffer;
    int m_iIndexBuffer;
    int m_iVertexStride;
	int m_iCount;
};



}//GELib

#endif//__GELIB_EXT_TERRAINGRASSMESH_H__
