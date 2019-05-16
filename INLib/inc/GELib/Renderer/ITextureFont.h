//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/ITextureFont.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_ITEXTUREFONT_H__
#define __GELIB_RENDERER_ITEXTUREFONT_H__

#include <GELib/GeConfig.h>

namespace GELib
{

class ITextureFont
{
public:
    GELIB_API ITextureFont() {}
    GELIB_API virtual ~ITextureFont() {}

    GELIB_API virtual void Uninitialize() = 0;

    class GELIB_API Glyph
    {
    public:
        short x, y, w, h;
        short offsetx, offsety;
        short advancex;
        int texture;
    };

    GELIB_API virtual Glyph* GetGlyph(int c) = 0;
    GELIB_API virtual int GetGlyphAdvance(int c) = 0;
    GELIB_API virtual int GetSize() = 0;
    GELIB_API virtual int GetHeight() = 0;

    GELIB_API virtual int GetNumTextures() = 0;
    GELIB_API virtual int GetTexture(int index) = 0;
};


}//GELib

#endif//__GELIB_RENDERER_ITEXTUREFONT_H__
