//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/WindowsFont.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_WINDOWSFONT_H__
#define __GELIB_RENDERER_WINDOWSFONT_H__

#include <GELib/GeConfig.h>
#include <GELib/Renderer/CachedTextureFont.h>
#include <vector>


namespace GELib
{


class CWindowsFont : public CCachedTextureFont
{
public:
    GELIB_API CWindowsFont();
    GELIB_API virtual ~CWindowsFont();

    GELIB_API bool Load(const wchar_t *faceName, int style, int fontSize, int cacheSize, int textureSize);
    GELIB_API bool Load(const char *fileName, const wchar_t *faceName, int style, int fontSize, int cacheSize, int textureSize);

	// memory is not copied and can be deleted
    GELIB_API bool Load(const void *fileInMemory, int fileSize, const wchar_t *faceName, int style, int fontSize, int cacheSize, int textureSize);

    GELIB_API virtual void Uninitialize();

protected:

    virtual bool RenderGlyph(int c, CCachedTextureFont::CGlyphBitMap &bmp);
    virtual bool ComputeGlyphAdvance(int c, int &advance);
	bool IsGlyphExist(int c);

    void *m_hFont;
    void *m_hBitmap;
	void *m_hBlackBrush;
    void *m_hDC;
    unsigned char *m_pPixels;
};


}//GELib

#endif//__GELIB_RENDERER_WINDOWSFONT_H__
