//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/CachedTextureFont.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_CACHEDTEXTUREFONT_H__
#define __GELIB_RENDERER_CACHEDTEXTUREFONT_H__

#include <GELib/GeConfig.h>
#include <GELib/Renderer/ITextureFont.h>
#include <vector>


namespace GELib
{


class CCachedTextureFont : public ITextureFont
{
public:
    GELIB_API CCachedTextureFont();
    GELIB_API virtual ~CCachedTextureFont();

    GELIB_API virtual void Uninitialize();

    GELIB_API virtual Glyph* GetGlyph(int c);
    GELIB_API virtual int GetGlyphAdvance(int c);
    GELIB_API virtual int GetSize();
    GELIB_API virtual int GetHeight();

    GELIB_API virtual int GetNumTextures();
    GELIB_API virtual int GetTexture(int index);

protected:

    class CGlyphBitMap
    {
    public:
        short width, height;
        short offsetx, offsety;
        short advancex;
        std::vector<unsigned char> bitmap;
    };
    virtual bool RenderGlyph(int c, CGlyphBitMap &bmp) = 0;
    virtual bool ComputeGlyphAdvance(int c, int &advance) = 0;

    void InitializeCache(int fontSize, int cacheSize, int textureSize);
	unsigned short GetGlyphCacheIndex(int c);
	void SetGlyphCacheIndex(int c, unsigned short index);
    bool AddChar(int c);
    int  AddTexture();

    class CGlyphCache
    {
    public:
        Glyph m_Glyph;
        unsigned short m_iNext;
        unsigned short m_iPrev;
        unsigned short m_iChar;
        unsigned char m_iMaxWidth;
        unsigned char m_iMaxHeight;
    };
    std::vector<CGlyphCache> m_GlyphCache;
    std::vector<int> m_Textures;
	int m_iTextureSize;

    int m_iSize;
    int m_iHeight;
    int m_iMaxChar;
	unsigned short *m_CharToCacheTable[16];

    int m_iMaxCacheSize;
    unsigned short m_iTx, m_iTy;
    unsigned short m_iLineHeight;
    unsigned short m_iCacheTail;
    bool m_bCacheFull;

	class GlyphAdvanceCache
	{
	public:
		short advance;
		wchar_t c;
	};
	GlyphAdvanceCache m_GlyphAdvanceCache[256];
};

inline unsigned short CCachedTextureFont::GetGlyphCacheIndex(int c)
{
	int a = c >> 12;
	if (m_CharToCacheTable[a])
		return m_CharToCacheTable[a][c & 0x0fff];
	else
		return 0xffff;
}


}//GELib

#endif//__GELIB_RENDERER_CACHEDTEXTUREFONT_H__
