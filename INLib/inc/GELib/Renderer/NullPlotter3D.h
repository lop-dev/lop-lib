//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/NullPlotter3D.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_NULLPLOTTER3D_H__
#define __GELIB_RENDERER_NULLPLOTTER3D_H__

#include "IPlotter3D.h"

namespace GELib
{

class CNullPlotter3D : public IPlotter3D
{
public:
    GELIB_API CNullPlotter3D();
    GELIB_API virtual ~CNullPlotter3D();

    GELIB_API virtual void Initialize();
    GELIB_API virtual void Uninitialize();

    GELIB_API virtual void SetViewProjectionMatrix(const CMatrix &viewMatrix, const CMatrix &projMatrix);
    GELIB_API virtual void SetViewProjectionMatrix(const CMatrix &viewProjMatrix);
    GELIB_API virtual void GetViewProjectionMatrix(CMatrix &viewMatrix, CMatrix &projMatrix);
    GELIB_API virtual void GetViewProjectionMatrix(CMatrix &viewProjMatrix);
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
    GELIB_API virtual void DrawCapsule(const CVector3 &p0, const CVector3 &p1, float r);
    GELIB_API virtual void DrawCircle(const CVector3 &c, float r, int axis=2);
    GELIB_API virtual void DrawCircle(const CVector3 &c, float r, const CVector3 &axis);
    GELIB_API virtual void DrawEllipsoid(const CVector3 &c, const CVector3 &r);
    GELIB_API virtual void DrawEllipsoid(const CMatrix &matrix, const CVector3 &r);
    GELIB_API virtual void DrawEllipse(const CVector3 &c, const CVector3 &r, int axis=2);
    GELIB_API virtual void DrawEllipse(const CVector3 &c, const CVector3 &xaxis, float rx, const CVector3 &yaxis, float ry);
    GELIB_API virtual void DrawMatrixAxes(const CMatrix &matrix, float scale=10.0f);
    GELIB_API virtual void DrawFrustum(const CFrustum &f);
    GELIB_API virtual void DrawPolyMesh(const CPolyMesh &polyMesh);
    GELIB_API virtual void DrawPolyMesh(const CMatrix &matrix, const CPolyMesh &polyMesh);
    GELIB_API virtual void DrawPolygon(const CVector3 *v, int numVertices);

    GELIB_API virtual void FillTriangle(const CVector3 &a, const CVector3 &b, const CVector3 &c);
    GELIB_API virtual void FillBox(const CAABox &box);
    GELIB_API virtual void FillBox(const CVector3 &bmin, const CVector3 &bmax);
    GELIB_API virtual void FillBox(const COBox &box);
    GELIB_API virtual void FillBox(const CMatrix &matrix, const CVector3 &extents);
    GELIB_API virtual void FillSphere(const CSphere &s);
    GELIB_API virtual void FillSphere(const CVector3 &c, float r);
    GELIB_API virtual void FillFrustum(const CFrustum &f);

private:
    CMatrix m_ViewMatrix;
    CMatrix m_ProjMatrix;
    CMatrix m_ViewProjMatrix;
    CFrustum m_Frustum;
    bool m_bDepthTest;
    bool m_bDepthWrite;
};



}//GELib

#endif//__GELIB_RENDERER_NULLPLOTTER3D_H__
