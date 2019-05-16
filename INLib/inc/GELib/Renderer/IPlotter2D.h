//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/IPlotter2D.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_IPLOTTER2D_H__
#define __GELIB_RENDERER_IPLOTTER2D_H__

#include <GELib/GeConfig.h>
#include <GELib/Math/GeMath.h>

// undefined Windows.h DrawText
#ifdef DrawText
    #undef DrawText
#endif


namespace GELib
{

class ITextureFont;

// draw flags
const int DRAW_BLEND_NONE = 0;
const int DRAW_BLEND_ALPHA = 1;
const int DRAW_BLEND_ADD = 2;
const int DRAW_BLEND_SUB = 3;
const int DRAW_BLEND_MUL = 4;		// no alpha
const int DRAW_BLEND_MUL_ADD = 5;   // no alpha
const int DRAW_BLEND_REV_SUB = 6;	// no alpha
const int DRAW_BLEND_COMPOSITE = 7;
const int DRAW_BLEND_MASK = 7;

const int DRAW_IMAGE_FLIP_X = 1<<10;
const int DRAW_IMAGE_FLIP_Y = 1<<11;
const int DRAW_IMAGE_FLIP_XY = DRAW_IMAGE_FLIP_X | DRAW_IMAGE_FLIP_Y;
const int DRAW_IMAGE_FLIP_MASK = DRAW_IMAGE_FLIP_XY;

const int DRAW_IMAGE_CHANNEL = 1 << 12;

const int DRAW_TEXT_DEFAULT = 0;
const int DRAW_TEXT_SHADOW = 1<<10;
const int DRAW_TEXT_ITALIC = 1<<11;
const int DRAW_TEXT_UNDERLINE = 1<<12;
const int DRAW_TEXT_OUTLINE = 1<<13;
const int DRAW_TEXT_GRADIENT = 1<<14;
const int DRAW_TEXT_SMOOTH_SHADOW = 1<<15;
const int DRAW_TEXT_ALIGN_LEFT = 0;
const int DRAW_TEXT_ALIGN_RIGHT = 1<<20;
const int DRAW_TEXT_ALIGN_TOP = 0;
const int DRAW_TEXT_ALIGN_BOTTOM = 1<<21;
const int DRAW_TEXT_ALIGN_CENTER_X = 1<<22;
const int DRAW_TEXT_ALIGN_CENTER_Y = 1<<23;
const int DRAW_TEXT_ALIGN_CENTER_XY = DRAW_TEXT_ALIGN_CENTER_X | DRAW_TEXT_ALIGN_CENTER_Y;
const int DRAW_TEXT_MASK = DRAW_TEXT_SHADOW | DRAW_TEXT_ITALIC | DRAW_TEXT_UNDERLINE |
						   DRAW_TEXT_OUTLINE | DRAW_TEXT_GRADIENT | DRAW_TEXT_SMOOTH_SHADOW |
						   DRAW_TEXT_ALIGN_RIGHT | DRAW_TEXT_ALIGN_BOTTOM | DRAW_TEXT_ALIGN_CENTER_XY;

class CClipRectInfo
{
public:
	CClipRectInfo() { enable = false; outScreen = false; }
	bool enable;
	bool outScreen;
	float x, y, w, h;

	bool IsEqual(const CClipRectInfo &r) const
	{
		return enable == r.enable && x == r.x && y == r.y && w == r.w && h == r.h;
	}
};

class CPlotter2DStatistic
{
public:
	CPlotter2DStatistic();
	void Reset();

	int m_iNumDrawCalls;
	int m_iNumTriangles;
	int m_iNumLines;
};

class IPlotter2D
{
public:
    GELIB_API IPlotter2D() {}
    GELIB_API virtual ~IPlotter2D() {}

    GELIB_API virtual void Initialize() = 0;
    GELIB_API virtual void Uninitialize() = 0;

    GELIB_API virtual void SetScreen(float left, float top, float right, float bottom) = 0;
    GELIB_API virtual void GetScreen(float &left, float &top, float &right, float &bottom) = 0;

    GELIB_API virtual void SetColor(float r, float g, float b, float a=1.0f) = 0;
    GELIB_API virtual void SetColor(int r, int g, int b, int a=255) = 0;
	GELIB_API virtual void SetColor(CRGBAColor c) = 0;
    GELIB_API virtual void SetColor(const CVector3 &c) = 0;
    GELIB_API virtual void SetColor(const CVector4 &c) = 0;

    GELIB_API virtual void SetZ(float z) = 0;
    GELIB_API virtual float GetZ() = 0;

    GELIB_API virtual void EnableClipRect(bool enable=true) = 0;
    GELIB_API virtual bool IsEnableClipRect() = 0;
    GELIB_API virtual void SetClipRect(float x, float y, float w, float h) = 0;
    GELIB_API virtual void GetClipRect(float &x, float &y, float &w, float &h) = 0;
    GELIB_API virtual void SetClipRect(const CClipRectInfo &c) = 0;
    GELIB_API virtual void GetClipRect(CClipRectInfo &c) = 0;

    GELIB_API virtual void EnableMask(bool enable) = 0;
    GELIB_API virtual void SetMask(float x, float y, float w, float h, int maskTex) = 0;

    GELIB_API virtual void SetChannelWeight(const CVector4 &w) = 0;

    GELIB_API virtual void EnableTextureFilter(bool enable) = 0;

    GELIB_API virtual void Flush() = 0;

    GELIB_API virtual void DrawRect(float x, float y, float w, float h, int flag=DRAW_BLEND_ALPHA) = 0;
    GELIB_API virtual void DrawRectBorder(float x, float y, float w, float h, float bw, float bh, int flag=DRAW_BLEND_ALPHA) = 0;
    GELIB_API virtual void DrawGradientRect(float x, float y, float w, float h, const CVector4 c[4], int flag=DRAW_BLEND_ALPHA) = 0;
    GELIB_API virtual void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int flag=DRAW_BLEND_ALPHA) = 0;
    GELIB_API virtual void DrawImage(float x, float y, float w, float h, int tex, int flag=DRAW_BLEND_ALPHA) = 0;
    GELIB_API virtual void DrawImage(float x, float y, float w, float h, float tx, float ty, float tw, float th, int tex, int flag=DRAW_BLEND_ALPHA) = 0;
    GELIB_API virtual void DrawTile(float x, float y, float w, float h, float tx, float ty, float tw, float th, int tex, int flag=DRAW_BLEND_ALPHA) = 0;
    GELIB_API virtual void DrawTriangleImage(float x1, float y1, float x2, float y2, float x3, float y3,
										float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, int tex, int flag=DRAW_BLEND_ALPHA) = 0;
	GELIB_API virtual void DrawNineGrid(float x, float y, float w, float h,
										float texw, float texh, float rx, float ry, float rw, float rh,	// in pixel
										float left, float top, float right, float bottom, bool center,
										int tex, int flag=DRAW_BLEND_ALPHA) = 0;
    GELIB_API virtual void DrawLine(float x1, float y1, float x2, float y2, int flag=DRAW_BLEND_ALPHA) = 0;

    GELIB_API virtual void SetFont(ITextureFont *font) = 0;
    GELIB_API virtual ITextureFont* GetFont() = 0;
    GELIB_API virtual void SetFontSize(float size) = 0;
    GELIB_API virtual float GetFontSize() = 0;

    GELIB_API virtual void SetTextOutlineColor(CRGBAColor c) = 0;
    GELIB_API virtual void SetTextShadowColor(CRGBAColor c) = 0;
    GELIB_API virtual void SetTextGradientColor(CRGBAColor c) = 0;
    GELIB_API virtual void SetTextShadowIntensity(float s) = 0;
    GELIB_API virtual void SetTextShadowOffset(const CVector2 &o) = 0;

    GELIB_API virtual void DrawText(float x, float y, const char *text, int textLen=-1, int flag=DRAW_TEXT_DEFAULT) = 0;
    GELIB_API virtual void DrawText(float x, float y, const wchar_t *text, int textLen=-1, int flag=DRAW_TEXT_DEFAULT) = 0;

    GELIB_API virtual float GetTextWidth(const char *text, int textLen=-1) = 0;
    GELIB_API virtual float GetTextWidth(const wchar_t *text, int textLen=-1) = 0;
    GELIB_API virtual int   GetNumCharsInWidth(const char *text, float width) = 0;
    GELIB_API virtual int   GetNumCharsInWidth(const wchar_t *text, float width) = 0;

	// statistic
	GELIB_API virtual const CPlotter2DStatistic* GetStatistic() { return 0; }
	GELIB_API virtual void ResetStatistic() {}
};



}//GELib

#endif//__GELIB_RENDERER_IPLOTTER2D_H__
