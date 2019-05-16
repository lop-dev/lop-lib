//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxAreaMesh.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXAREAMESH_H__
#define __GELIB_FX_FXAREAMESH_H__

#include <GELib/Ext/LocalMesh.h>

namespace GELib
{

class CDynamicVertexBuffer;

class CFxAreaVertex
{
public:
    CVector3 m_Position;
    int m_PackedTexCoord;
    int m_PackedTangent;
    int m_PackedNormal;
};

const int MESHCLASS_AREA = 13;

class CFxAreaMesh : public CRenderMesh
{
public:

    CFxAreaMesh();
    virtual ~CFxAreaMesh();
    virtual int GetClassId() { return MESHCLASS_AREA; }

    CFxAreaVertex* LockVertexBuffer(int numQuads);
    void UnlockVertexBuffer();

    // called by CRenderer
    virtual const char * GetShaderName();
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

    CDynamicVertexBuffer *m_pDynamicVertexBuffer;
};



}//GELib

#endif//__GELIB_FX_FXAREAMESH_H__
