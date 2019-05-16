//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/ShadowMap.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_SHADOWMAP_H__
#define __GELIB_RENDERER_SHADOWMAP_H__

#include <GELib/GeConfig.h>

namespace GELib
{


GELIB_API enum EShadowMapMethod
{
    SHADOW_MAP_METHOD_AUTO, // pssm for directionals, per-object for others
	SHADOW_MAP_METHOD_PER_OBJECT,
	SHADOW_MAP_METHOD_PARALLEL_SPLIT,
	SHADOW_MAP_METHOD_CASCADE,
};

namespace
{
	int const PSSM_MAX_NUMBER = 3;
#if defined(_XBOX)
    int const DEFAULT_SHADOW_DEPTH_SIZE = 880;
#else
    int const DEFAULT_SHADOW_DEPTH_SIZE = 1024;
#endif
    int const DEFAULT_PSSM_NUMBER = 3;
    float const DEFAULT_PSSM_LAMBDA = .5f;
    //float const PSSM_MIN_FAR_IN_BACK_OF_LIGHT = 600.f;
    float const DEFAULT_PSSM_MANUAL_SPLIT_DEPTH[PSSM_MAX_NUMBER - 1] = {300.f, 900.f};
    float const DEFAULT_SHADOW_LIMIT = 3000.f;
    float const DEFAULT_SHADOW_PCF_RADIUS = 1.5f;
    bool const DEFAULT_SHADOW_PCF_HIGH_QUALITY = false;
    float const DEFAULT_PSSM_FADE_BEGIN = 600.f;
    float const DEFAULT_PSSM_FADE_RANGE = 0;
}


}//GELib

#endif//__GELIB_RENDERER_SHADOWMAP_H__
