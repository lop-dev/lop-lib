//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/PackVertex.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_PACKVERTEX_H__
#define __GELIB_RENDERER_PACKVERTEX_H__
#include <GELib/GeConfig.h>
#include <GELib/Math/GeMath.h>

namespace GELib
{

class CVertexSpec;

void AddPackedTexCoordVetexSpec(CVertexSpec &vs, int numTexCoords);
void AddPackedTangentVertexSpec(CVertexSpec &vs);
void AddPackedBlendIndicesVertexSpec(CVertexSpec &vs);
void AddPackedBlendWeightsVertexSpec(CVertexSpec &vs);
void AddPackedColorVertexSpec(CVertexSpec &vs);

int PackNormal(const CVector3 &n, float sign=-1.0f);
void UnpackNormal(int pack, CVector3 &n, float &sign);

int PackTexCoord(const CVector2 &tc);
void UnpackTexCoord(int pack, CVector2 &tc);

int PackBlendIndices(int a, int b, int c, int d);
void UnpackBlendIndices(int pack, int &a, int &b, int &c, int &d);

int PackBlendWeights(float a, float b, float c, float d);
void UnpackBlendWeights(int pack, float &a, float &b, float &c, float &d);

int PackColor(float r, float g, float b, float a);
void UnpackColor(int pack, float &r, float &g, float &b, float &a);
int PackColor(int r, int g, int b, int a);
void UnpackColor(int pack, int &r, int &g, int &b, int &a);

int* GetTextureArrayPackedTexCoord(int textureArrayType, int index, float indexf);

}//GELib
#endif//__GELIB_RENDERER_PACKVERTEX_H__
