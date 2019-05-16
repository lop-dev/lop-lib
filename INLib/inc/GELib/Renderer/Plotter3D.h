//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/Plotter3D.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_PLOTTER3D_H__
#define __GELIB_RENDERER_PLOTTER3D_H__

#include <vector>
#include <GELib/GeConfig.h>
#include <GELib/Math/GeMath.h>
#include <GELib/Math/Frustum.h>
#include <GELib/Math/PolyMesh.h>
#include "IPlotter3D.h"
#include "RenderStateSet.h"
#include "Shader.h"


namespace GELib
{


class CPlotter3DShader : public CShader
{
public:
	CPlotter3DShader();
	virtual bool Initialize();
    CShaderParameter m_ViewProjMatrix;
};

class CPlotter3D : public IPlotter3D
{
public:
    GELIB_API CPlotter3D();
    GELIB_API virtual ~CPlotter3D();

    GELIB_API virtual void Initialize();
    GELIB_API virtual void Uninitialize();

    GELIB_API virtual void SetViewProjectionMatrix(const CMatrix &viewMatrix, const CMatrix &projMatrix);
    GELIB_API virtual void GetViewProjectionMatrix(CMatrix &viewMatrix, CMatrix &projMatrix);
    GELIB_API virtual const CFrustum& GetFrustum();

    GELIB_API virtual void SetColor(float r, float g, float b, float a=1.0f);
    GELIB_API virtual void SetColor(int r, int g, int b, int a=255);
    GELIB_API virtual void SetColor(CRGBAColor c);
    GELIB_API virtual void SetColor(const CVector3 &c);
    GELIB_API virtual void SetColor(const CVector4 &c);
    GELIB_API virtual void SetDepthTest(bool on);
    GELIB_API virtual bool GetDepthTest();
    GELIB_API virtual void SetDepthWrite(bool on);
    GELIB_API virtual bool GetDepthWrite();
    GELIB_API virtual void Flush();

    // box matrix can contain scale, but sphere and cylinder can't
    GELIB_API virtual void DrawLine(const CVector3 &a, const CVector3 &b);
    GELIB_API virtual void DrawTriangle(const CVector3 &a, const CVector3 &b, const CVector3 &c);
    GELIB_API virtual void DrawBox(const CAABox &box);
    GELIB_API virtual void DrawBox(const CVector3 &bmin, const CVector3 &bmax);
    GELIB_API virtual void DrawBox(const COBox &box);
    GELIB_API virtual void DrawBox(const CMatrix &matrix, const CVector3 &extents);
    GELIB_API virtual void DrawSphere(const CSphere &s);
    GELIB_API virtual void DrawSphere(const CVector3 &c, float r);
    GELIB_API virtual void DrawSphere(const CMatrix &matrix, float r);
    GELIB_API virtual void DrawCylinder(const CVector3 &base, float r, float height);
    GELIB_API virtual void DrawCylinder(const CMatrix &matrix, float r, float height);
    GELIB_API virtual void DrawCapsule(const CCapsule &c);
    GELIB_API virtual void DrawCapsule(const CVector3 &p0, const CVector3 &p1, float r);
    GELIB_API virtual void DrawCircle(const CVector3 &c, float r, int axis=2);
    GELIB_API virtual void DrawCircle(const CVector3 &c, float r, const CVector3 &axis);
    GELIB_API virtual void DrawEllipsoid(const CVector3 &c, const CVector3 &r);
    GELIB_API virtual void DrawEllipsoid(const CMatrix &matrix, const CVector3 &r);
    GELIB_API virtual void DrawEllipse(const CVector3 &c, const CVector3 &r, int axis=2);
    GELIB_API virtual void DrawEllipse(const CVector3 &c, const CVector3 &xaxis, float rx, const CVector3 &yaxis, float ry);
    GELIB_API virtual void DrawMatrixAxes(const CMatrix &matrix, float scale=10.0f);
    GELIB_API virtual void DrawFrustum(const CFrustum &f);
    GELIB_API virtual void DrawPolyMesh(const CMatrix &matrix, const CPolyMesh &polyMesh);
    GELIB_API virtual void DrawPolygon(const CVector3 *v, int numVertices);

    GELIB_API virtual void FillTriangle(const CVector3 &a, const CVector3 &b, const CVector3 &c);
    GELIB_API virtual void FillBox(const CAABox &box);
    GELIB_API virtual void FillBox(const CVector3 &bmin, const CVector3 &bmax);
    GELIB_API virtual void FillBox(const COBox &box);
    GELIB_API virtual void FillBox(const CMatrix &matrix, const CVector3 &extents);
    GELIB_API virtual void FillSphere(const CSphere &s);
    GELIB_API virtual void FillSphere(const CVector3 &c, float r);
    GELIB_API virtual void FillCylinder(const CVector3 &base, float r, float height);
    GELIB_API virtual void FillCylinder(const CMatrix &matrix, float r, float height);
    GELIB_API virtual void FillFrustum(const CFrustum &f);
    GELIB_API virtual void FillPolyMesh(const CMatrix &matrix, const CPolyMesh &polyMesh);
    GELIB_API virtual void FillPolygon(const CVector3 *v, int numVertices);

private:

    class Vertex
    {
    public:
        CVector3 position;
        int color;

        void Set(const CVector3 &p, int c)
        {
            position = p;
            color = c;
        }
        void Set(float x, float y, float z, int c)
        {
            position.Set(x, y, z);
            color = c;
        }
    };

    class PrimitiveBuffer
    {
    public:
        PrimitiveBuffer();
        void Initialize();
        void Uninitialize();

        GELib::EPrimitiveType m_ePrimitiveType;
        std::vector<Vertex> m_VertexBuffer;
        std::vector<unsigned short> m_IndexBuffer;
        int m_iNumVertices;
        int m_iNumIndices;
        int m_iMaxVertices;
        int m_iMaxIndices;
        int m_iCurVertex;
        int m_iCurIndex;
    };

    void DrawHalfCircle(const CVector3 &c, float r, const CVector3 &ax, const CVector3 &ay);

    Vertex* AllocateBuffer(PrimitiveBuffer &buffer, int vcount, int icount);
    Vertex* AllocateLine(int vcount, int icount);
    Vertex* AllocateTriangle(int vcount, int icount);
    void AddLineIndices(unsigned short idx1, unsigned short idx2);
    void AddTriangleIndices(unsigned short idx1, unsigned short idx2, unsigned short idx3);

    PrimitiveBuffer m_LineBuffer;
    PrimitiveBuffer m_TriangleBuffer;

    CMatrix m_ViewMatrix;
    CMatrix m_ProjMatrix;
    CMatrix m_ViewProjTMatrix;
    CFrustum m_Frustum;
	CPlotter3DShader *m_pShader;
	bool m_bShaderCached;
    int m_iVertexDecl;
    int m_iColor;
    EPrimitiveType m_ePrimitiveType;
    CRenderStateSet m_RenderStates;

    enum { CIRCLE_SIDES_MAX = 24 };
    int m_iRGB[3];
    float m_fSin[CIRCLE_SIDES_MAX];
    float m_fCos[CIRCLE_SIDES_MAX];
};


inline CPlotter3D::Vertex* CPlotter3D::AllocateLine(int vcount, int icount)
{
    return AllocateBuffer(m_LineBuffer, vcount, icount);
}

inline CPlotter3D::Vertex* CPlotter3D::AllocateTriangle(int vcount, int icount)
{
    return AllocateBuffer(m_TriangleBuffer, vcount, icount);
}

inline void CPlotter3D::AddLineIndices(unsigned short idx1, unsigned short idx2)
{
    m_LineBuffer.m_IndexBuffer[m_LineBuffer.m_iCurIndex++] = (unsigned short)m_LineBuffer.m_iCurVertex + idx1;
    m_LineBuffer.m_IndexBuffer[m_LineBuffer.m_iCurIndex++] = (unsigned short)m_LineBuffer.m_iCurVertex + idx2;
}

inline void CPlotter3D::AddTriangleIndices(unsigned short idx1, unsigned short idx2, unsigned short idx3)
{
    m_TriangleBuffer.m_IndexBuffer[m_TriangleBuffer.m_iCurIndex++] = (unsigned short)m_TriangleBuffer.m_iCurVertex + idx1;
    m_TriangleBuffer.m_IndexBuffer[m_TriangleBuffer.m_iCurIndex++] = (unsigned short)m_TriangleBuffer.m_iCurVertex + idx2;
    m_TriangleBuffer.m_IndexBuffer[m_TriangleBuffer.m_iCurIndex++] = (unsigned short)m_TriangleBuffer.m_iCurVertex + idx3;
}


}//GELib

#endif//__GELIB_RENDERER_PLOTTER3D_H__
