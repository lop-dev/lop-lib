//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/BufferedPlotter3D.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_BUFFEREDPLOTTER3D_H__
#define __GELIB_RENDERER_BUFFEREDPLOTTER3D_H__

#include "IPlotter3D.h"
#include <GELib/Utility/IOStream.h>

namespace GELib
{

class CBufferedPlotter3D : public IPlotter3D
{
public:
    GELIB_API CBufferedPlotter3D();
    GELIB_API virtual ~CBufferedPlotter3D();

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

    GELIB_API void Draw(IPlotter3D &plotter);
    GELIB_API void ClearBuffer();

private:

    void WriteOp(short op);
    void ReadOp(short &op);
    void WriteFrustum(const CFrustum &f);
    void ReadFrustum(CFrustum &f);
    void WritePolyMesh(const CPolyMesh &polyMesh);
    void ReadPolyMesh(CPolyMesh &polyMesh);
    void WritePolygon(const CVector3 *v, int numVertices);
    void ReadPolygon(std::vector<CVector3> &vertices);

    CMatrix m_ViewMatrix;
    CMatrix m_ProjMatrix;
    CFrustum m_Frustum;
    bool m_bDepthTest;
    bool m_bDepthWrite;
    int m_iOpCount;

    CMemoryWriteStream m_Stream;
    CMemoryStream m_ReadStream;
};

inline void CBufferedPlotter3D::WriteOp(short op)
{
    WriteStream(m_Stream, op);
    ++m_iOpCount;
}

inline void CBufferedPlotter3D::ReadOp(short &op)
{
    ReadStream(m_ReadStream, op);
}


}//GELib

#endif//__GELIB_RENDERER_BUFFEREDPLOTTER3D_H__
