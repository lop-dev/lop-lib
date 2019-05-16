//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/IPlotter3D.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_IPLOTTER3D_H__
#define __GELIB_RENDERER_IPLOTTER3D_H__

#include <GELib/GeConfig.h>
#include <GELib/Math/GeMath.h>
#include <GELib/Math/Frustum.h>
#include <GELib/Math/PolyMesh.h>


namespace GELib
{


class IPlotter3D
{
public:
    GELIB_API IPlotter3D() {}
    GELIB_API virtual ~IPlotter3D() {}

    GELIB_API virtual void Initialize() = 0;
    GELIB_API virtual void Uninitialize() = 0;

    GELIB_API virtual void SetViewProjectionMatrix(const CMatrix &viewMatrix, const CMatrix &projMatrix) = 0;
    GELIB_API virtual void GetViewProjectionMatrix(CMatrix &viewMatrix, CMatrix &projMatrix) = 0;
    GELIB_API virtual const CFrustum& GetFrustum() = 0;

    GELIB_API virtual void SetColor(float r, float g, float b, float a=1.0f) = 0;
    GELIB_API virtual void SetColor(int r, int g, int b, int a=255) = 0;
    GELIB_API virtual void SetColor(CRGBAColor c) = 0;
    GELIB_API virtual void SetColor(const CVector3 &c) = 0;
    GELIB_API virtual void SetColor(const CVector4 &c) = 0;
    GELIB_API virtual void SetDepthTest(bool on) = 0;
    GELIB_API virtual bool GetDepthTest() = 0;
    GELIB_API virtual void SetDepthWrite(bool on) = 0;
    GELIB_API virtual bool GetDepthWrite() = 0;
    GELIB_API virtual void Flush() = 0;

    // box matrix can contain scale, but sphere and cylinder can't
    GELIB_API virtual void DrawLine(const CVector3 &a, const CVector3 &b) = 0;
    GELIB_API virtual void DrawTriangle(const CVector3 &a, const CVector3 &b, const CVector3 &c) = 0;
    GELIB_API virtual void DrawBox(const CAABox &box) = 0;
    GELIB_API virtual void DrawBox(const CVector3 &bmin, const CVector3 &bmax) = 0;
    GELIB_API virtual void DrawBox(const COBox &box) = 0;
    GELIB_API virtual void DrawBox(const CMatrix &matrix, const CVector3 &extents) = 0;
    GELIB_API virtual void DrawSphere(const CSphere &s) = 0;
    GELIB_API virtual void DrawSphere(const CVector3 &c, float r) = 0;
    GELIB_API virtual void DrawSphere(const CMatrix &matrix, float r) = 0;
    GELIB_API virtual void DrawCylinder(const CVector3 &base, float r, float height) = 0;
    GELIB_API virtual void DrawCylinder(const CMatrix &matrix, float r, float height) = 0;
    GELIB_API virtual void DrawCapsule(const CCapsule &c) = 0;
    GELIB_API virtual void DrawCapsule(const CVector3 &p0, const CVector3 &p1, float r) = 0;
    GELIB_API virtual void DrawCircle(const CVector3 &c, float r, int axis=2) = 0;
    GELIB_API virtual void DrawCircle(const CVector3 &c, float r, const CVector3 &axis) = 0;
    GELIB_API virtual void DrawEllipsoid(const CVector3 &c, const CVector3 &r) = 0;
    GELIB_API virtual void DrawEllipsoid(const CMatrix &matrix, const CVector3 &r) = 0;
    GELIB_API virtual void DrawEllipse(const CVector3 &c, const CVector3 &r, int axis=2) = 0;
    GELIB_API virtual void DrawEllipse(const CVector3 &c, const CVector3 &xaxis, float rx, const CVector3 &yaxis, float ry) = 0;
    GELIB_API virtual void DrawMatrixAxes(const CMatrix &matrix, float scale=10.0f) = 0;
    GELIB_API virtual void DrawFrustum(const CFrustum &f) = 0;
    GELIB_API virtual void DrawPolyMesh(const CMatrix &matrix, const CPolyMesh &polyMesh) = 0;
    GELIB_API virtual void DrawPolygon(const CVector3 *v, int numVertices) = 0;

    GELIB_API virtual void FillTriangle(const CVector3 &a, const CVector3 &b, const CVector3 &c) = 0;
    GELIB_API virtual void FillBox(const CAABox &box) = 0;
    GELIB_API virtual void FillBox(const CVector3 &bmin, const CVector3 &bmax) = 0;
    GELIB_API virtual void FillBox(const COBox &box) = 0;
    GELIB_API virtual void FillBox(const CMatrix &matrix, const CVector3 &extents) = 0;
    GELIB_API virtual void FillSphere(const CSphere &s) = 0;
    GELIB_API virtual void FillSphere(const CVector3 &c, float r) = 0;
    GELIB_API virtual void FillCylinder(const CVector3 &base, float r, float height) = 0;
    GELIB_API virtual void FillCylinder(const CMatrix &matrix, float r, float height) = 0;
    GELIB_API virtual void FillFrustum(const CFrustum &f) = 0;
    GELIB_API virtual void FillPolyMesh(const CMatrix &matrix, const CPolyMesh &polyMesh) = 0;
    GELIB_API virtual void FillPolygon(const CVector3 *v, int numVertices) = 0;
};


}//GELib

#endif//__GELIB_RENDERER_IPLOTTER3D_H__
