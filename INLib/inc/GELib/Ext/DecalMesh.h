//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/DecalMesh.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_DECALMESH_H__
#define __GELIB_EXT_DECALMESH_H__

#include <vector>
#include <string>

#include <GELib/Renderer/RenderMesh.h>
#include <GELib/Utility/DataBuffer.h>

namespace GELib
{

class CIOStream;
class CStdMaterial;

class CMeshAsset;
class CTerrain;
class CLoadParameter;

class CDecalAccepter
{
public:
    CDecalAccepter(CMeshAsset *mesh, const CMatrix &matrix) { m_pMesh = mesh; m_Matrix = matrix; m_pTerrain = 0; }
    CDecalAccepter(CTerrain *terrain) { m_pTerrain = terrain; m_Matrix.SetIdentity(); m_pMesh = 0; }
    CMeshAsset *m_pMesh;
    CTerrain *m_pTerrain;
    CMatrix m_Matrix;
};

typedef std::vector<CDecalAccepter> CDecalAccepterList;

const int MESHCLASS_DECAL = 5;

class CDecalMesh : public CRenderMesh
{
public:

    CDecalMesh();
    virtual ~CDecalMesh();
    virtual int GetClassId() { return MESHCLASS_DECAL; }

    bool Save(CIOStream &stream);
    bool Load(CIOStream &stream, const CLoadParameter &param);
    bool SkipLoad(CIOStream &stream);
    void Unload();
	virtual bool CreateLoadedResource();

    void Project(const COBox &decalBox, CDecalAccepterList &accepters, const CVector2 &texCoordRepeat);

    const CAABox& GetBoundingBox() { return m_Box; }

    // called by CRenderer
    virtual const char * GetShaderName();
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

	class CLoadedResource
	{
	public:
		CDataBuffer m_VertexBuffer;
		CDataBuffer m_IndexBuffer;
	};
	CLoadedResource *m_pLoadedResource;
};



}//GELib

#endif//__GELIB_EXT_DECALMESH_H__
