//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxParticleMesh.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXPARTICLEMESH_H__
#define __GELIB_FX_FXPARTICLEMESH_H__

#include <vector>
#include <string>

#include <GELib/Renderer/RenderMesh.h>

namespace GELib
{


class CDynamicVertexBuffer;



class CFxParticleMeshShaderParameters : public CMeshShaderParameters
{
public:
    CShaderParameter m_WorldMatrix;
    CShaderParameter m_WorldInvMatrix;
    CShaderParameter m_CameraRight;
    CShaderParameter m_CameraUp;
    CShaderParameter m_CameraPosition;
    CShaderParameter m_OrientationScale;
};

class CFxParticleMeshShaderData
{
public:
    CMatrix m_WorldTMatrix;
    CMatrix m_WorldInvTMatrix;
    CVector2 m_OrientationScale;
};

class CFxParticleVertex
{
public:
    CVector3 m_Position;
    int m_iTexCoord;
    CVector3 m_PrevPosition;
    unsigned int m_iSize;
    float m_fRotation;
    int m_iColor;
};

const int MESHCLASS_PARTICLE = 3;

class CFxParticleMesh : public CRenderMesh
{
public:

    CFxParticleMesh();
    virtual ~CFxParticleMesh();
    virtual int GetClassId() { return MESHCLASS_PARTICLE; }

    CFxParticleVertex* LockVertexBuffer(int numParticles);
    void UnlockVertexBuffer();

    void Clear();

    // called by CRenderer
    virtual const char * GetShaderName();
    virtual void CreateShaderParameters(CMeshShader *shader);
    virtual void SetShaderParameters(CMeshShader *shader, CRendererContext &rc);
    virtual void SetBuffers(CRendererContext &rc);
    virtual int DrawChunk(int chunk, CRendererContext &rc);

private:

    int m_iVertexDecl;
    int m_iVertexBuffer;
    int m_iIndexBuffer;

    int m_iVertexStride;
    int m_iNumVertices;
    int m_iVertexBaseIndex;

    CDynamicVertexBuffer *m_pDynamicVertexBuffer;
};


}//GELib

#endif//__GELIB_FX_FXPARTICLEMESH_H__
