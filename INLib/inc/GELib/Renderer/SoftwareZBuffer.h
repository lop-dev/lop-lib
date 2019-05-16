//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/SoftwareZBuffer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_SOFTWAREZBUFFER_H__
#define __GELIB_RENDERER_SOFTWAREZBUFFER_H__

#include <GELib/Math/GeMath.h>
#include "swr/geometry_processor.h"
#include "swr/rasterizer_subdivaffine.h"

namespace GELib
{


class CSoftwareZBuffer
{
public:

	CSoftwareZBuffer();
	~CSoftwareZBuffer();

	void SetSize(int w, int h);
	void GetSize(int &w, int &h);

	void SetViewProjectionMatrix(const CMatrix &mat);
	void SetWorldMatrix(const CMatrix &mat);

	void Begin();
	void DrawTriangles(const void *vertices, int vertexStride, const int *indices, int numIndices);
	void End();

	bool IsOccluded(const CAABox &box);
	bool IsOccluded(const CSphere &sphere);

	int GetNumTriangles();
	unsigned short * GetZBuffer();

private:

	void AllocateBuffer();
	void FreeBuffer();
	void SetShaderParameter();

	swr::GeometryProcessor m_GeometryProcessor;
	swr::RasterizerSubdivAffine m_Rasterizer;

	int m_iWidth;
	int m_iHeight;
	CMatrix m_ViewProjMatrix;
	CMatrix m_WorldMatrix;
	int m_iNearPlaneBoxCornerIndex;
	unsigned short *m_pZBuffer;
	int m_iNumTriangles;
	bool m_bShaderParameterDirty;
};


}//GELib

#endif//__GELIB_RENDERER_SOFTWAREZBUFFER_H__
