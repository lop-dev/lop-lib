//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/FreeTypeFont.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_FREETYPEFONT_H__
#define __GELIB_RENDERER_FREETYPEFONT_H__

#include <GELib/GeConfig.h>
#include <GELib/Renderer/CachedTextureFont.h>
#include <vector>

typedef struct FT_FaceRec_*  FT_Face;

namespace GELib
{


class CFreeTypeFont : public CCachedTextureFont
{
public:
    GELIB_API CFreeTypeFont();
    GELIB_API virtual ~CFreeTypeFont();

    GELIB_API bool Load(const char *fileName, int faceIndex, int style, int fontSize, int cacheSize, int textureSize);

	// memory is not copied and can't be deleted
    GELIB_API bool Load(const void *fileInMemory, int fileSize, int faceIndex, int style, int fontSize, int cacheSize, int textureSize);

    GELIB_API virtual void Uninitialize();

protected:

    virtual bool RenderGlyph(int c, CCachedTextureFont::CGlyphBitMap &bmp);
    virtual bool ComputeGlyphAdvance(int c, int &advance);

    FT_Face m_pFace;
};


}//GELib

#endif//__GELIB_RENDERER_FREETYPEFONT_H__
