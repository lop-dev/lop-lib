//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/LakeMesh.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_LAKEMESH_H__
#define __GELIB_EXT_LAKEMESH_H__

#include <vector>
#include <string>

#include <GELib/Ext/WaterMesh.h>

namespace GELib
{
class CIOStream;
class CStdMaterial;
class CMaterialAsset;
class CMeshUtility;
class CBaseRenderObject;
class CMeshAsset;

const float DEFAULT_LAKE_SURFACE_SIZE = 1024.0f;

class CLakeMeshShaderParameters : public CMeshShaderParameters
{
public:
	CShaderParameter m_MapScaleLevelAndHeight;
	CShaderParameter m_HeightMap;
	CShaderParameter m_InteractionHeightMap;
	CShaderParameter m_InteractionMapArea;
	CShaderParameter m_WorldMatrix;
	CShaderParameter m_WorldInvMatrix;
	CShaderParameter m_WindVelocityCoord;
};

class CLakeMeshShaderData
{
public:
    CLakeMeshShaderData() { m_fAnimatedTextureIndex = -1.0f; }
	CMatrix m_WorldTMatrix;
	CMatrix m_WorldInvTMatrix;
    float m_MaterialParameters[4];
    float m_fAnimatedTextureIndex;
};

class CLakeMesh : public CWaterMesh
{
public:

    GELIB_API CLakeMesh();
    GELIB_API virtual ~CLakeMesh();
    GELIB_API virtual int GetClassId();
    GELIB_API virtual int GetShaderHash();
	GELIB_API virtual bool CreateMesh(const SWaterDesc& _waterDesc);
	GELIB_API virtual bool UpdateDesc(const SWaterDesc& _waterDesc);
	GELIB_API virtual bool UpdateData(CWaterSurface *_pSurface);
	GELIB_API virtual void* NewShareData() {return new CLakeMeshShaderData();}
	GELIB_API virtual void DeleteShaderData(void *sd) { delete (CLakeMeshShaderData*)sd;}
	GELIB_API virtual void SetVisible(bool _bVisibale) {m_bVisible = _bVisibale;}
	GELIB_API virtual bool GetVisible( ) { return m_bVisible;}

    GELIB_API virtual int GetNumChunks();
	GELIB_API virtual CMaterialAsset* GetMaterial(int chunk);
	GELIB_API virtual void SetMaterial(CMaterialAsset* pSeaMaterial);
	GELIB_API virtual CStdMaterial* GetRenderMaterial(int chunk);

    // called by CRenderer

	GELIB_API virtual const char * GetShaderName();
	GELIB_API virtual const char * GetShaderDefinitions();
	GELIB_API virtual void CreateShaderParameters(CMeshShader *shader);
	GELIB_API virtual void SetShaderParameters(CMeshShader *shader, CRendererContext &rc);
    GELIB_API virtual int GetUVAtlasTexCoordIndex();
	GELIB_API virtual void SetBuffers(CRendererContext &rc);
    GELIB_API virtual int DrawChunk(int chunk, CRendererContext &rc);
    GELIB_API virtual int DrawChunks(int fromChunk, int toChunk, CRendererContext &rc);

    GELIB_API int  GetNumTexCoords();
    GELIB_API void SetUVAtlasTexCoordIndex(int index);

	GELIB_API virtual void SetHightMap(int _iHightMap)
	{
		m_iHeightMapRef = _iHightMap;
	}

	GELIB_API virtual void GetBoundingBox(CAABox& _box);
	GELIB_API virtual bool SetMesh(CMeshAsset* _pMesh);
	GELIB_API virtual CMeshAsset* GetMesh()
	{
		return m_pMeshAsset;
	}

	GELIB_API virtual bool GetMeshData(CMeshUtility& meshUtil,bool bOnlyVertexData);

	GELIB_API static void GetDefaultMeshDesc(SWaterDesc& _waterDesc);

    GELIB_API void SetShaderQuality(int iQuality)
    {
        m_iRenderQuality = iQuality;
    }

protected:

	void _FreeMesh();
	void _FillVertexAndIndexBuffer();
	void _ResetMeshSize();

protected:

	CAABox	m_Box;

    int m_iVertexDecl;
    int m_iVertexBuffer;
    int m_iIndexBuffer;

	int m_iWidth;
	int m_iHeight;
    int m_iVertexStride;
    int m_iNumVertices;
    int m_iIndexStride;
    int m_iNumIndices;
    int m_iNumTexCoords;
    int m_iUVAtlasTexCoordIndex;
	int m_iHeightMapRef;
    bool m_bHasVertexColors;
	bool m_bVisible;
	CMeshAsset* m_pMeshAsset;
	CVector4 m_vWindVelocityCoord;

	//sea mesh describe
	bool	m_bComputeWithGPU;
	bool	m_bVertexWave;
	float	m_fSizeWidth;
	float	m_fSizeHeight;
	float	m_fLakeLevel;
	float	m_fMaxWaveHeight;

    class Chunk
    {
    public:
		CMaterialAsset *m_pMaterial;
        int m_iFirstIndex;
        int m_iNumTriangles;
    };
    std::vector<Chunk> m_Chunks;

    int m_iRenderQuality;
};

inline int CLakeMesh::GetNumChunks()
{
    return (int)m_Chunks.size();
}

inline CMaterialAsset* CLakeMesh::GetMaterial(int chunk)
{
    //assert(chunk>=0 && chunk<GetNumChunks());
    if (chunk>=0 && chunk<GetNumChunks())
        return m_Chunks[chunk].m_pMaterial;
    return 0;
}

}//GELib

#endif//__GELIB_EXT_LAKEMESH_H__
