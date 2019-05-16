//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/LocalMesh.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_LOCALMESH_H__
#define __GELIB_EXT_LOCALMESH_H__

#include <vector>
#include <string>

#include <GELib/Renderer/RenderMesh.h>
#include <GELib/Utility/DataBuffer.h>
#include "FaceCamera.h"

namespace GELib
{

class CIOStream;
class CStdMaterial;
class CBaseAsset;
class CMaterialAsset;
class CMeshUtility;
class CLoadParameter;


class CLocalMeshShaderParameters : public CMeshShaderParameters
{
public:
    CShaderParameter m_WorldMatrix;
    CShaderParameter m_WorldInvMatrix;
    CShaderParameter m_PrevWorldMatrix;
};

class CLocalMeshShaderData
{
public:
    CLocalMeshShaderData();
    CMatrix m_WorldTMatrix;
    CMatrix m_WorldInvTMatrix;
    CMatrix m_PrevWorldTMatrix;
    CVector4 m_MaterialParameters[2];
    EFaceCameraMode m_eFaceCameraMode;
    float m_fAnimatedTextureIndex;
};


const int MESHCLASS_LOCAL = 1;

class CLocalMesh : public CRenderMesh
{
public:

    CLocalMesh();
    virtual ~CLocalMesh();
    virtual int GetClassId() { return MESHCLASS_LOCAL; }

    bool Save(CBaseAsset *container, CIOStream &stream);
    bool Load(CIOStream &stream, const CLoadParameter &param);
    bool Import(CMeshUtility &meshUtil, const CLoadParameter &param);
    virtual void Unload();
	virtual bool CreateLoadedResource();
    void ConvertToMeshUtility(CMeshUtility &meshUtil);

    const CAABox& GetBoundingBox() { return m_Box; }
     int GetNumChunks();
    CMaterialAsset* GetMaterial(int chunk);

	int GetNumVertices() { return m_iNumVertices; }
	int GetNumIndices() { return m_iNumIndices; }

	void SetNumTexCoords(int n) { m_iNumTexCoords = n; }
    int  GetNumTexCoords() { return m_iNumTexCoords; }
    void SetUVAtlasTexCoordIndex(int index);

    // called by CRenderer
    virtual const char * GetShaderName();
    virtual int GetUVAtlasTexCoordIndex();
    virtual void CreateShaderParameters(CMeshShader *shader);
    virtual void SetShaderParameters(CMeshShader *shader, CRendererContext &rc);
    virtual void SetBuffers(CRendererContext &rc);
    virtual int DrawChunk(int chunk, CRendererContext &rc);
    virtual int DrawChunks(int fromChunk, int toChunk, CRendererContext &rc);

protected:

    CAABox m_Box;

    int m_iVertexDecl;
    int m_iVertexBuffer;
    int m_iIndexBuffer;

    int m_iVertexStride;
    int m_iNumVertices;
    int m_iIndexStride;
    int m_iNumIndices;
    int m_iNumTexCoords;
    int m_iUVAtlasTexCoordIndex;
    bool m_bHasVertexColors;

    class Chunk
    {
    public:
        CMaterialAsset *m_pMaterial;
        int m_iFirstIndex;
        int m_iNumTriangles;
    };

    std::vector<Chunk> m_Chunks;

	class CLoadedResource
	{
	public:
		CDataBuffer m_VertexBuffer;
		CDataBuffer m_IndexBuffer;
	};
	CLoadedResource *m_pLoadedResource;
};

inline int CLocalMesh::GetNumChunks()
{
    return (int)m_Chunks.size();
}

inline CMaterialAsset* CLocalMesh::GetMaterial(int chunk)
{
    //assert(chunk>=0 && chunk<GetNumChunks());
    if (chunk>=0 && chunk<GetNumChunks())
        return m_Chunks[chunk].m_pMaterial;
    return 0;
}



}//GELib

#endif//__GELIB_EXT_LOCALMESH_H__
