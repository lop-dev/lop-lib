//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/RendererTypes.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_RENDERERTYPES_H__
#define __GELIB_RENDERER_RENDERERTYPES_H__

namespace GELib
{


enum EBlendMode
{
    BLEND_NONE = 0,
    BLEND_ALPHA = 1,
    BLEND_ADDITIVE = 2,
    BLEND_PREALPHA = 3,
	BLEND_NUM_MAX
};

enum EVolumeMode
{
    VOLUME_NONE = 0,
    VOLUME_FRONT = 1,
    VOLUME_BACK = 2
};

enum EResolutionMode
{
    RESOLUTION_FULL = 0,
    RESOLUTION_HALF = 1,
    RESOLUTION_QUARTER = 2
};

enum EBufferType
{
    BUFFER_DEFAULT,
    BUFFER_DEPTH,
    BUFFER_SHADOW,
};

enum ELightMapMode
{
    LIGHTMAP_NONE = 0,
    LIGHTMAP_MONO_SHADOW,
    LIGHTMAP_COLOR_SHADOW,
    LIGHTMAP_SIMPLE_LIGHT,
    LIGHTMAP_FULL_LIGHT,
};

enum EDrawMeshMode
{
    DRAW_MESH_NORMAL = 0,
    DRAW_MESH_SKY = 1,
    DRAW_MESH_PLANE_DECAL = 10,
    DRAW_MESH_VOLUME_DECAL,
// modified by bob -- start
    DRAW_MESH_WATER = 20,
// modified by bob -- end
};


}//GELib

#endif//__GELIB_RENDERER_RENDERERTYPES_H__
