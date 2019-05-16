//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/RenderMesh.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_RENDERMESH_H__
#define __GELIB_RENDERER_RENDERMESH_H__

#include <vector>
#include <GELib/GeConfig.h>
#include "RendererTypes.h"
#include "MeshShader.h"
#include <GELib/Math/GeMath.h>


namespace GELib
{


class CRenderMesh
{
public:
    GELIB_API CRenderMesh();
    GELIB_API virtual ~CRenderMesh();
    GELIB_API virtual int GetClassId();

    // called by renderer in main thread
    GELIB_API virtual const char * GetShaderName();
    GELIB_API virtual const char * GetShaderDefinitions();
    GELIB_API virtual int GetShaderHash();
    GELIB_API virtual int GetUVAtlasTexCoordIndex();

    GELIB_API virtual void CreateShaderParameters(CMeshShader *shader);
    GELIB_API virtual void SetShaderParameters(CMeshShader *shader, CRendererContext &rc);

    // called by renderer in render thread
    GELIB_API virtual void SetBuffers(CRendererContext &rc);
    GELIB_API virtual int DrawChunk(int chunk, CRendererContext &rc);
    GELIB_API virtual int DrawChunks(int fromChunk, int toChunk, CRendererContext &rc);
};


}//GELib


#endif//__GELIB_RENDERER_RENDERMESH_H__
