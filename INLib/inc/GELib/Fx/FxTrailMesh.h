//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxTrailMesh.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXTRAILMESH_H__
#define __GELIB_FX_FXTRAILMESH_H__

#include <vector>
#include <string>

#include <GELib/Ext/LocalMesh.h>

namespace GELib
{

class CDynamicVertexBuffer;

class CFxTrailMeshShaderParameters : public CMeshShaderParameters
{
public:
    CShaderParameter m_CameraPosition;
};

class CFxTrailVertex
{
public:
    CVector3 m_Position;
    int m_PackedTexCoord;
    int m_PackedTangent;
	union
	{
	    int m_PackedNormal;
		float m_fWidth;
	};
};

const int MESHCLASS_TRAIL = 4;

class CFxTrailMesh : public CRenderMesh
{
public:

    CFxTrailMesh();
    virtual ~CFxTrailMesh();
    virtual int GetClassId() { return MESHCLASS_TRAIL; }

	void SetFaceCamera(bool f) { m_bFaceCamera = f; }

    CFxTrailVertex* LockVertexBuffer(int numVertices);
    void UnlockVertexBuffer();

    void Clear();

    // called by CRenderer
    virtual const char * GetShaderName();
    virtual int GetShaderHash();
    virtual void CreateShaderParameters(CMeshShader *shader);
    virtual void SetShaderParameters(CMeshShader *shader, CRendererContext &rc);
    virtual void SetBuffers(CRendererContext &rc);
    virtual int DrawChunk(int chunk, CRendererContext &rc);

protected:

    int m_iVertexDecl;
    int m_iVertexBuffer;
    int m_iIndexBuffer;

    int m_iVertexStride;
    int m_iNumVertices;
    int m_iVertexBaseIndex;

	bool m_bFaceCamera;
    CDynamicVertexBuffer *m_pDynamicVertexBuffer;
};




}//GELib

#endif//__GELIB_FX_FXTRAILMESH_H__
