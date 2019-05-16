//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/Plotter2D.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_PLOTTER2D_H__
#define __GELIB_RENDERER_PLOTTER2D_H__

#include <vector>
#include <GELib/GeConfig.h>
#include <GELib/Math/GeMath.h>
#include "IPlotter2D.h"
#include "RenderStateSet.h"
#include "Shader.h"

namespace GELib
{


class CPlotter2DShader : public CShader
{
public:
	CPlotter2DShader();
	virtual ~CPlotter2DShader();
	virtual bool Initialize();

    CShaderParameter m_ViewProjMatrix;
    CShaderParameter m_MaskPositionSize;
    CShaderParameter m_Channel;
    CShaderParameter m_Offset;
    CShaderParameter m_Color;
    CShaderParameter m_Sampler;
};

class CPlotter2DColorShader : public CPlotter2DShader
{
public:
	CPlotter2DColorShader();
	virtual bool Initialize();
};

class CPlotter2DColorMaskShader : public CPlotter2DShader
{
public:
	CPlotter2DColorMaskShader();
	virtual bool Initialize();
};

class CPlotter2DT1Shader : public CPlotter2DShader
{
public:
	CPlotter2DT1Shader();
	virtual bool Initialize();
};

class CPlotter2DT1MaskShader : public CPlotter2DShader
{
public:
	CPlotter2DT1MaskShader();
	virtual bool Initialize();
};

class CPlotter2DT1AlphaShader : public CPlotter2DShader
{
public:
	CPlotter2DT1AlphaShader();
	virtual bool Initialize();
};

class CPlotter2DT1AlphaMaskShader : public CPlotter2DShader
{
public:
	CPlotter2DT1AlphaMaskShader();
	virtual bool Initialize();
};

class CPlotter2DT1ChannelShader : public CPlotter2DShader
{
public:
	CPlotter2DT1ChannelShader();
	virtual bool Initialize();
};

class CPlotter2DT1ChannelMaskShader : public CPlotter2DShader
{
public:
	CPlotter2DT1ChannelMaskShader();
	virtual bool Initialize();
};

class CPlotter2DT1AlphaOutlineShader : public CPlotter2DShader
{
public:
	CPlotter2DT1AlphaOutlineShader();
	virtual bool Initialize();
};

class CPlotter2DT1AlphaOutlineMaskShader : public CPlotter2DShader
{
public:
	CPlotter2DT1AlphaOutlineMaskShader();
	virtual bool Initialize();
};

class CPlotter2DT1AlphaShadowShader : public CPlotter2DShader
{
public:
	CPlotter2DT1AlphaShadowShader();
	virtual bool Initialize();
};

class CPlotter2DT1AlphaShadowMaskShader : public CPlotter2DShader
{
public:
	CPlotter2DT1AlphaShadowMaskShader();
	virtual bool Initialize();
};

class CPlotter2D : public IPlotter2D
{
public:
    GELIB_API CPlotter2D();
    GELIB_API virtual ~CPlotter2D();

    GELIB_API virtual void Initialize();
    GELIB_API virtual void Uninitialize();

    GELIB_API virtual void SetScreen(float left, float top, float right, float bottom);			// virtual screen size
    GELIB_API virtual void GetScreen(float &left, float &top, float &right, float &bottom);

    GELIB_API virtual void SetColor(float r, float g, float b, float a=1.0f);
    GELIB_API virtual void SetColor(int r, int g, int b, int a=255);
	GELIB_API virtual void SetColor(CRGBAColor c);
    GELIB_API virtual void SetColor(const CVector3 &c);
    GELIB_API virtual void SetColor(const CVector4 &c);

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

    GELIB_API virtual void SetChannelWeight(const CVector4 &w);

    GELIB_API virtual void EnableTextureFilter(bool enable);

    GELIB_API virtual void Flush();

    GELIB_API virtual void DrawRect(float x, float y, float w, float h, int flag=DRAW_BLEND_ALPHA);
    GELIB_API virtual void DrawRectBorder(float x, float y, float w, float h, float bw, float bh, int flag=DRAW_BLEND_ALPHA);
    GELIB_API virtual void DrawGradientRect(float x, float y, float w, float h, const CVector4 c[4], int flag=DRAW_BLEND_ALPHA);
    GELIB_API virtual void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int flag=DRAW_BLEND_ALPHA);
    GELIB_API virtual void DrawImage(float x, float y, float w, float h, int tex, int flag=DRAW_BLEND_ALPHA);
    GELIB_API virtual void DrawImage(float x, float y, float w, float h, float tx, float ty, float tw, float th, int tex, int flag=DRAW_BLEND_ALPHA);
    GELIB_API virtual void DrawTile(float x, float y, float w, float h, float tx, float ty, float tw, float th, int tex, int flag=DRAW_BLEND_ALPHA);
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

	// statistic
	GELIB_API virtual const CPlotter2DStatistic* GetStatistic();
	GELIB_API virtual void ResetStatistic();

private:

    void  _DrawText(float x, float y, const void *text, int charBytes, int textLen, int flag);
    float _DrawText(float x, float y, const void *text, int charBytes, int textLen, int flag, int color, int color2);
    float _GetTextWidth(const void *text, int charBytes, int textLen);
    int   _GetNumCharsInWidth(const void *text, int charBytes, float width);
	void AlignToPixel(float &x, float &y);

    class Vertex
    {
    public:
        CVector3 position;
        CVector2 texcoord;
        int color;

        void Set(float x, float y, float z, float tx, float ty, int c)
        {
            position.x = x;
            position.y = y;
            position.z = z;
            texcoord.x = tx;
            texcoord.y = ty;
            color = c;
        }
        void Set(float x, float y, float z, int c)
        {
            position.x = x;
            position.y = y;
            position.z = z;
            color = c;
        }
    };

    Vertex* Allocate(int vcount, int icount, GELib::EPrimitiveType primType, int tex, int flag);
    Vertex* AllocateChar();
	void AddIndex(unsigned short i);
    void AddTriIndices();
    void AddQuadIndices();

    std::vector<Vertex> m_VertexBuffer;
    std::vector<unsigned short> m_IndexBuffer;
    unsigned short m_iNumVertices;
    unsigned short m_iNumIndices;
    unsigned short m_iMaxVertices;
    unsigned short m_iMaxIndices;
    unsigned short m_iCurIndex;
    unsigned short m_iCurVertex;

	CPlotter2DColorShader *m_pColorShader;
	CPlotter2DColorMaskShader *m_pColorMaskShader;
	CPlotter2DT1Shader *m_pT1Shader;
	CPlotter2DT1MaskShader *m_pT1MaskShader;
	CPlotter2DT1AlphaShader *m_pT1AlphaShader;
	CPlotter2DT1AlphaMaskShader *m_pT1AlphaMaskShader;
	CPlotter2DT1ChannelShader *m_pT1ChannelShader;
	CPlotter2DT1ChannelMaskShader *m_pT1ChannelMaskShader;
	CPlotter2DT1AlphaOutlineShader *m_pT1AlphaOutlineShader;
	CPlotter2DT1AlphaOutlineMaskShader *m_pT1AlphaOutlineMaskShader;
	CPlotter2DT1AlphaShadowShader *m_pT1AlphaShadowShader;
	CPlotter2DT1AlphaShadowMaskShader *m_pT1AlphaShadowMaskShader;
    std::vector<CShader*> m_NotCachedShaders;

    CMatrix m_ViewProjMatrix;
    CMatrix m_ViewProjTMatrix;
    float m_Screen[4];
	CVector2 m_ScreenResolutionRatio;
    int m_iVertexDecl;
    int m_iColor;
    int m_iGradientColor;
    int m_iShadowColor;
    int m_iWhiteColor;
    int m_iBlackColor;
	CVector4 m_TextOutlineColor;
	CVector2 m_TextShadowOffset;
	float m_fTextShadowIntensity;
    int m_iTexture;
    int m_iBlendFlag;
    int m_iChannelFlag;
	CVector4 m_ChannelWeight;
    EPrimitiveType m_ePrimitiveType;
    ETextureAddressMode m_eTextureAddressMode;
	float m_fZ;
    bool m_bZTest;
	bool m_bDrawOutline;
	bool m_bDrawShadow;
	bool m_bEnableTextureFilter;
	bool m_bScreenMatchResolution;
    CRenderStateSet m_RenderStates;

    int m_iNumBatchedChars;
	CPlotter2DStatistic m_Statistic;
	bool m_bShiftHalfPixel;

    ITextureFont *m_pFont;
    float m_fFontSize;

	CClipRectInfo m_ClipRect;

    bool m_bEnableMask;
    int m_iMaskTexture;
	CVector4 m_MaskPositionSize;
};

inline void CPlotter2D::AddIndex(unsigned short i)
{
    m_IndexBuffer[m_iCurIndex++] = m_iCurVertex + i;
}

inline void CPlotter2D::AddTriIndices()
{
	AddIndex(0);
	AddIndex(1);
	AddIndex(2);
}

inline void CPlotter2D::AddQuadIndices()
{
	AddIndex(0);
	AddIndex(1);
	AddIndex(2);
	AddIndex(0);
	AddIndex(2);
	AddIndex(3);
}

inline CPlotter2D::Vertex* CPlotter2D::AllocateChar()
{
	const int vcount = 4;
	const int icount = 6;
    if (m_iNumIndices+icount>m_iMaxIndices || m_iNumVertices+vcount>m_iMaxVertices)
        Flush();

    m_iCurVertex = m_iNumVertices;
    m_iNumVertices += vcount;
    m_iCurIndex = m_iNumIndices;
    m_iNumIndices += icount;
    return &m_VertexBuffer[m_iCurVertex];
}

inline void CPlotter2D::AlignToPixel(float &x, float &y)
{
	x = Round(x / m_ScreenResolutionRatio.x) * m_ScreenResolutionRatio.x;
	y = Round(y / m_ScreenResolutionRatio.y) * m_ScreenResolutionRatio.y;
}

}//GELib

#endif//__GELIB_RENDERER_PLOTTER2D_H__
