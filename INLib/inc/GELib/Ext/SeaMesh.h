//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/SeaMesh.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_SEAMESH_H__
#define __GELIB_EXT_SEAMESH_H__

#include <vector>
#include <string>
#include "WaterMesh.h"

namespace GELib
{
class CIOStream;
class CStdMaterial;
class CMaterialAsset;

class CSeaMeshShaderParameters : public GELib::CMeshShaderParameters
{

public:
	GELib::CShaderParameter m_CameraPosition;
	GELib::CShaderParameter m_ProjCorner;
	GELib::CShaderParameter m_MapScaleLevelAndHeight;
	GELib::CShaderParameter m_HeightMap;
	GELib::CShaderParameter m_NormalMap;
	//GELib::CShaderParameter m_InteractionHeightMap;
	//GELib::CShaderParameter m_InteractionMapArea;
};

class CSeaMeshShaderData
{
public:
    CSeaMeshShaderData() { m_fAnimatedTextureIndex = -1.0f; }
    float m_MaterialParameters[4];
    float m_fAnimatedTextureIndex;
};

class CSeaMesh : public CWaterMesh
{
public:

    GELIB_API CSeaMesh();
    GELIB_API virtual ~CSeaMesh();
    GELIB_API virtual int GetClassId();
    GELIB_API virtual int GetShaderHash();
	GELIB_API virtual bool CreateMesh(const SWaterDesc& _waterDesc);
	GELIB_API virtual bool UpdateDesc(const SWaterDesc& _waterDesc);
	GELIB_API virtual bool UpdateData(CWaterSurface *_pSurface);
	GELIB_API virtual void* NewShareData() {return new CSeaMeshShaderData();}
	GELIB_API virtual void DeleteShaderData(void *sd) { delete (CSeaMeshShaderData*)sd;}
	GELIB_API virtual void SetVisible(bool _bVisibale) {m_bVisible = _bVisibale;}
	GELIB_API virtual bool GetVisible( ) { return m_bVisible;}

    GELIB_API virtual int GetNumChunks();
	GELIB_API virtual CMaterialAsset* GetMaterial(int chunk);
	GELIB_API virtual void SetMaterial(CMaterialAsset* pSeaMaterial);
	GELIB_API virtual CStdMaterial* GetRenderMaterial(int chunk);

    GELIB_API virtual void GetBoundingBox(CAABox& box)
    {
        box = m_Box;
    }

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

	GELIB_API static void GetDefaultMeshDesc(SWaterDesc& _waterDesc);

    GELIB_API void SetShaderQuality(int iQuality)
    {
        m_iRenderQuality = iQuality;
    }

protected:

	void _FreeSeaMesh();
	void _FillVertexAndIndexBuffer();

protected:

	CAABox	 m_Box;
	CVector3 m_projectPosition;
	CVector4 m_ProjCorner[4];

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

	//sea mesh describe
	bool	m_bComputeWithGPU;
	bool	m_bVertexWave;
	float	m_fSizeWidth;
	float	m_fSizeHeight;
	float	m_fSeaLevel;
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

inline int CSeaMesh::GetNumChunks()
{
    return (int)m_Chunks.size();
}

inline CMaterialAsset* CSeaMesh::GetMaterial(int chunk)
{
    if (chunk>=0 && chunk<GetNumChunks())
        return m_Chunks[chunk].m_pMaterial;
    return 0;
}

}//GELib

#endif//__GELIB_EXT_SEAMESH_H__
