//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxBoardMesh.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXBOARDMESH_H__
#define __GELIB_FX_FXBOARDMESH_H__

#include <GELib/Ext/LocalMesh.h>

namespace GELib
{

class CFxBoardVertex
{
public:
    CVector3 m_Position;
    int m_PackedTexCoord;
    int m_PackedTangent;
    int m_PackedNormal;
};

const int MESHCLASS_BOARD = 10;

class CFxBoardMesh : public CRenderMesh
{
public:

    CFxBoardMesh();
    virtual ~CFxBoardMesh();
    virtual int GetClassId() { return MESHCLASS_BOARD; }

    CFxBoardVertex* LockVertexBuffer();
    void UnlockVertexBuffer();

    // called by CRenderer
    virtual const char * GetShaderName();
    virtual void CreateShaderParameters(CMeshShader *shader);
    virtual void SetShaderParameters(CMeshShader *shader, CRendererContext &rc);
    virtual void SetBuffers(CRendererContext &rc);
    virtual int DrawChunk(int chunk, CRendererContext &rc);

 protected:

    int m_iVertexDecl;
	CFxBoardVertex m_VertexBuffer[4];
};



}//GELib

#endif//__GELIB_FX_FXBOARDMESH_H__
