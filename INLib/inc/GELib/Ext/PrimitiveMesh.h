//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/PrimitiveMesh.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_PRIMITIVEMESH_H__
#define __GELIB_EXT_PRIMITIVEMESH_H__

#include <vector>
#include <string>

#include <GELib/Ext/LocalMesh.h>

namespace GELib
{


class CPrimitiveVertex;

const int MESHCLASS_PRIMITIVE = 11;

class CPrimitiveMesh : public CRenderMesh
{
public:

    CPrimitiveMesh();
    virtual ~CPrimitiveMesh();
    virtual int GetClassId() { return MESHCLASS_PRIMITIVE; }

	void BuildBoard(const CAABox &box);

    const CAABox& GetBoundingBox() const { return m_Box; }

    // called by CRenderer
    virtual const char * GetShaderName();
    virtual void CreateShaderParameters(CMeshShader *shader);
    virtual void SetShaderParameters(CMeshShader *shader, CRendererContext &rc);
    virtual void SetBuffers(CRendererContext &rc);
    virtual int DrawChunk(int chunk, CRendererContext &rc);

 protected:

	CPrimitiveVertex* AllocateVertexBuffer(int n);
	unsigned short* AllocateIndexBuffer(int n);

	void Unload();

    int m_iVertexDecl;
	int m_iVertexBuffer;
	int m_iIndexBuffer;
	int m_iNumVertices;
	int m_iNumIndices;
	CAABox m_Box;
};




}//GELib

#endif//__GELIB_EXT_PRIMITIVEMESH_H__
