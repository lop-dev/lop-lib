//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/SkinMesh.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_SKINMESH_H__
#define __GELIB_CHARACTER_SKINMESH_H__

#include <vector>
#include <string>

#include <GELib/Renderer/RenderMesh.h>
#include <GELib/Ext/FaceCamera.h>
#include <GELib/Utility/DataBuffer.h>

namespace GELib
{

class CIOStream;
class CStdMaterial;
class CRenderBatch;
class CMaterialAsset;
class CMeshUtility;
class CLoadParameter;
class CSkinMeshAsset;
class CBaseAsset;


const int MAX_CHUNK_BONES = 36;		// = SkinMesh.hlsl MAX_CHUNK_BONES
const int MAX_CHUNK_BONES_EX = 72;	// = SkinMeshEx.hlsl MAX_CHUNK_BONES
const int MAX_BONE_BLENDS = 4;		// = SkinMesh.hlsl MAX_BONE_BLENDS

class CSkinMeshAsset;

class CSkinMeshShaderParameters : public CMeshShaderParameters
{
public:
    CShaderParameter m_WorldMatrix;
    CShaderParameter m_WorldInvMatrix;
    CShaderParameter m_PrevWorldMatrix;
    CShaderParameter m_BoneMatrices;
    CShaderParameter m_PrevBoneMatrices;
};


class CSkinMeshShaderData
{
public:
    CSkinMeshShaderData();
    CMatrix *m_pWorldTMatrix;
    CMatrix *m_pWorldInvTMatrix;
    CMatrix *m_pPrevWorldTMatrix;
    float *m_pBoneMatrices;
    float *m_pPrevBoneMatrices;
    CVector4 *m_pMaterialParameters;
    EFaceCameraMode m_eFaceCameraMode;
};

const int MESHCLASS_SKIN = 2;

class CSkinMesh : public CRenderMesh
{
public:

    CSkinMesh();
    virtual ~CSkinMesh();
    virtual int GetClassId() { return MESHCLASS_SKIN; }

    bool Save(CBaseAsset *container, CIOStream &stream);
    bool Load(CIOStream &stream, const CLoadParameter &param);
	bool Import(CMeshUtility &meshUtil, CSkinMeshAsset *group, int lod, const CLoadParameter &param);
    void Unload();
	bool CreateLoadedResource();

    const CAABox& GetBoundingBox() { return m_Box; }
   int GetNumChunks() { return (int)m_Chunks.size(); }
    CMaterialAsset* GetMaterial(int chunk);

	int GetNumVertices() { return m_iNumVertices; }
	int GetNumIndices() { return m_iNumIndices; }

	void SetNumBlendWeights(int n) { m_iNumBlendWeights = n; }
	int GetNumBlendWeights() { return m_iNumBlendWeights; }

    int GetNumBones() { return (int)m_BoneIndices.size(); }
    short GetBoneIndex(int index) { return m_BoneIndices[index]; }

	void SetNumTexCoords(int n) { m_iNumTexCoords = n; }
    int  GetNumTexCoords() { return m_iNumTexCoords; }
    void SetUVAtlasTexCoordIndex(int index);

    // called by CRenderer
    virtual const char * GetShaderName();
	virtual const char * GetShaderDefinitions();
    virtual int GetShaderHash();
    virtual int GetUVAtlasTexCoordIndex();
    virtual void CreateShaderParameters(CMeshShader *shader);
    virtual void SetShaderParameters(CMeshShader *shader, CRendererContext &rc);
    virtual void SetBuffers(CRendererContext &rc);
    virtual int DrawChunk(int chunk, CRendererContext &rc);

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
    int m_iNumBlendWeights;
	int m_iMaxBonesPerChunk;
    bool m_bHasVertexColors;

    class Chunk
    {
    public:
        CMaterialAsset *m_pMaterial;
        int m_iFirstIndex;
        int m_iNumTriangles;
        std::vector<short> m_BoneIndices;
    };

    std::vector<Chunk> m_Chunks;
    std::vector<short> m_BoneIndices;

	class CLoadedResource
	{
	public:
		CDataBuffer m_VertexBuffer;
		CDataBuffer m_IndexBuffer;
	};
	CLoadedResource *m_pLoadedResource;
};


inline CMaterialAsset* CSkinMesh::GetMaterial(int chunk)
{
    //assert(chunk>=0 && chunk<GetNumChunks());
    if (chunk>=0 && chunk<GetNumChunks())
        return m_Chunks[chunk].m_pMaterial;
    return 0;
}




}//GELib

#endif//__GELIB_CHARACTER_SKINMESH_H__
