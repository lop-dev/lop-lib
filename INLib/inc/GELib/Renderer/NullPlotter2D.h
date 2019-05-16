//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/NullPlotter2D.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_NULLPLOTTER2D_H__
#define __GELIB_RENDERER_NULLPLOTTER2D_H__

#include "IPlotter2D.h"

namespace GELib
{


class CNullPlotter2D : public IPlotter2D
{
public:
    GELIB_API CNullPlotter2D();
    GELIB_API virtual ~CNullPlotter2D();

    GELIB_API virtual void Initialize();
    GELIB_API virtual void Uninitialize();

    GELIB_API virtual void SetScreen(float left, float top, float right, float bottom);
    GELIB_API virtual void GetScreen(float &left, float &top, float &right, float &bottom);

    GELIB_API virtual void SetColor(const CVector3 &c);
    GELIB_API virtual void SetColor(const CVector4 &c);
    GELIB_API virtual void SetColor(float r, float g, float b, float a=1.0f);
    GELIB_API virtual void SetColor(CRGBAColor c);
    GELIB_API virtual void SetColor(int r, int g, int b, int a=255);

    GELIB_API virtual void SetZ(float z);
	GELIB_API virtual float GetZ();

    GELIB_API virtual void EnableClipRect(bool enable=true);
    GELIB_API virtual bool IsEnableClipRect();
    GELIB_API virtual void SetClipRect(float x, float y, float w, float h);
    GELIB_API virtual void GetClipRect(float &x, float &y, float &w, float &h);
    GELIB_API virtual void SetClipRect(const CClipRectInfo &c);
    GELIB_API virtual void GetClipRect(CClipRectInfo &c);

    GELIB_API virtual void EnableMask(bool enable);
    GELIB_API virtual void SetMask(float x, float y, float w, float h, int maskTex);

    GELIB_API virtual void EnableTextureFilter(bool enable);

    GELIB_API virtual void Flush();

    GELIB_API virtual void DrawRect(float x, float y, float w, float h, int flag=DRAW_BLEND_ALPHA);
    GELIB_API virtual void DrawRectBorder(float x, float y, float w, float h, float bw, float bh, int flag=DRAW_BLEND_ALPHA);
    GELIB_API virtual void DrawGradientRect(float x, float y, float w, float h, const CVector4 c[4], int flag=DRAW_BLEND_ALPHA);
    GELIB_API virtual void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int flag=DRAW_BLEND_ALPHA);
    GELIB_API virtual void DrawImage(float x, float y, float w, float h, int tex, int flag=DRAW_BLEND_ALPHA);
    GELIB_API virtual void DrawImage(float x, float y, float w, float h, float tx, float ty, float tw, float th, int tex, int flag=DRAW_BLEND_ALPHA);
    GELIB_API virtual void DrawTriangleImage(float x1, float y1, float x2, float y2, float x3, float y3,
									float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, int tex, int flag=DRAW_BLEND_ALPHA);
	GELIB_API virtual void DrawNineGrid(float x, float y, float w, float h,
										float texw, float texh, float rx, float ry, float rw, float rh,	// in pixel
										float left, float top, float right, float bottom, bool center,
										int tex, int flag=DRAW_BLEND_ALPHA);
    GELIB_API virtual void DrawLine(float x1, float y1, float x2, float y2, int flag=DRAW_BLEND_ALPHA);

    GELIB_API virtual void SetFont(ITextureFont *font);
    GELIB_API virtual ITextureFont* GetFont();
    GELIB_API virtual void SetFontSize(float size);
    GELIB_API virtual float GetFontSize();

    GELIB_API virtual void SetTextOutlineColor(CRGBAColor c);
    GELIB_API virtual void SetTextShadowColor(CRGBAColor c);
    GELIB_API virtual void SetTextGradientColor(CRGBAColor c);
    GELIB_API virtual void SetTextShadowIntensity(float s);
    GELIB_API virtual void SetTextShadowOffset(const CVector2 &o);

    GELIB_API virtual void DrawText(float x, float y, const char *text, int textLen=-1, int flag=DRAW_TEXT_DEFAULT);
    GELIB_API virtual void DrawText(float x, float y, const wchar_t *text, int textLen=-1, int flag=DRAW_TEXT_DEFAULT);

    GELIB_API virtual float GetTextWidth(const char *text, int textLen=-1);
    GELIB_API virtual float GetTextWidth(const wchar_t *text, int textLen=-1);
    GELIB_API virtual int   GetNumCharsInWidth(const char *text, float width);
    GELIB_API virtual int   GetNumCharsInWidth(const wchar_t *text, float width);

private:

    float m_Screen[4];
	CClipRectInfo m_ClipRect;
	float m_fZ;
};


}//GELib

#endif//__GELIB_RENDERER_NULLPLOTTER2D_H__
