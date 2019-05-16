//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/TextureFont.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_TEXTUREFONT_H__
#define __GELIB_RENDERER_TEXTUREFONT_H__

#include <GELib/GeConfig.h>
#include <GELib/Renderer/ITextureFont.h>
#include <vector>

namespace GELib
{


class CTextureFont : public ITextureFont
{
public:
    GELIB_API CTextureFont();
    GELIB_API virtual ~CTextureFont();

    GELIB_API bool Load(const char *name, const char *path=0);
    GELIB_API bool Load(const void *textureFileInMemory, int textureFileSize, const void *abcFileInMemory, int abcFileSize, const char *name=0);

    GELIB_API virtual void Uninitialize();

    GELIB_API virtual Glyph* GetGlyph(int c);
    GELIB_API virtual int GetGlyphAdvance(int c);
    GELIB_API virtual int GetSize();
    GELIB_API virtual int GetHeight();

    GELIB_API virtual int GetNumTextures();
    GELIB_API virtual int GetTexture(int index);

protected:
    int m_iSize;
    int m_iMaxChar;
    std::vector<unsigned short> m_TranslatorTable;
    std::vector<Glyph> m_Glyphs;
    int m_iTexture;
};



}//GELib

#endif//__GELIB_RENDERER_TEXTUREFONT_H__
