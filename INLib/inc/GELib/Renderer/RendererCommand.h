//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/RendererCommand.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_RENDERERCOMMAND_H__
#define __GELIB_RENDERER_RENDERERCOMMAND_H__

#include <GELib/GeConfig.h>

namespace GELib
{

class CRenderer;
class CStdVisibility;

GELIB_API bool ExecuteRendererCommand(CRenderer *renderer, CStdVisibility *vis, const char *commandStr);

}//GELib

#endif//__GELIB_RENDERER_RENDERERCOMMAND_H__
