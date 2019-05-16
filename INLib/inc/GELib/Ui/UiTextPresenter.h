//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiTextPresenter.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UITEXTPRESENTER_H__
#define __GELIB_UI_UITEXTPRESENTER_H__

#include "UiObject.h"

namespace GELib
{


class CUiFont;
class CUiControl;

enum ETextAlignment
{
	TEXT_ALIGNMENT_LEFT,
	TEXT_ALIGNMENT_RIGHT,
	TEXT_ALIGNMENT_TOP,
	TEXT_ALIGNMENT_BOTTOM,
	TEXT_ALIGNMENT_CENTER,
};

enum ETextStyleFlag
{
	TEXTSTYLE_ELLIPSIS = 1 << 0,
	TEXTSTYLE_UNDERLINE = 1 << 1,
	TEXTSTYLE_OUTLINE = 1 << 2,
	TEXTSTYLE_GRADIENT = 1 << 3,
	TEXTSTYLE_SHADOW = 1 << 4
};

class CUiTextPresenter : public CUiObject
{
public:

	UICLASS_DECLARE(CUiTextPresenter);

	CUiTextPresenter(CUiSystem *system);
	virtual ~CUiTextPresenter();

	bool SetFont(int id);
	CUiFont* GetFont() { return m_pFont; }

	void SetHorizontalAlignment(ETextAlignment a);
	ETextAlignment GetHorizontalAlignment() { return m_eHorizontalAlignment; }
	void SetVerticalAlignment(ETextAlignment a);
	ETextAlignment GetVerticalAlignment() { return m_eVerticalAlignment; }

	void UseControlColor(bool use) { m_bUseControlColor = use; }
	bool IsUseControlColor() { return m_bUseControlColor; }

	void SetRichText(bool r) { m_bRichText = r; }
	bool IsRichText() { return m_bRichText; }

	// style flags
	void SetEllipsis(bool s) { SetTextStyle(TEXTSTYLE_ELLIPSIS, s); }
	bool IsEllipsis() { return GetTextStyle(TEXTSTYLE_ELLIPSIS); }
	void SetUnderline(bool s) { SetTextStyle(TEXTSTYLE_UNDERLINE, s); }
	bool IsUnderline() { return GetTextStyle(TEXTSTYLE_UNDERLINE); }
	void SetOutline(bool s) { SetTextStyle(TEXTSTYLE_OUTLINE, s); }
	bool IsOutline() { return GetTextStyle(TEXTSTYLE_OUTLINE); }
	void SetShadow(bool s) { SetTextStyle(TEXTSTYLE_SHADOW, s); }
	bool IsShadow() { return GetTextStyle(TEXTSTYLE_SHADOW); }
	void SetGradient(bool s) { SetTextStyle(TEXTSTYLE_GRADIENT, s); }
	bool IsGradient() { return GetTextStyle(TEXTSTYLE_GRADIENT); }

	void SetOutlineColor(CRGBAColor c) { m_OutlineColor = c; }
	CRGBAColor GetOutlineColor() { return m_OutlineColor; }
	void SetShadowColor(CRGBAColor c) { m_ShadowColor = c; }
	CRGBAColor GetShadowColor() { return m_ShadowColor; }
	void SetGradientColor(CRGBAColor c) { m_GradientColor = c; }
	CRGBAColor GetGradientColor() { return m_GradientColor; }
	void SetShadowOffset(const CVector2 &o) { m_ShadowOffset = o; }
	const CVector2& GetShadowOffset() { return m_ShadowOffset; }
	void SetShadowIntensity(float s) { m_fShadowIntensity = s; }
	float GetShadowIntensity() { return m_fShadowIntensity; }

	void SetLineMode(ETextLineMode m) { m_eLineMode = m; }
	ETextLineMode GetLineMode() { return m_eLineMode; }

	void SetLineSpacing(float s) { m_fLineSpacing = s; }
	float GetLineSpacing() { return m_fLineSpacing; }

	void SetSelectedTextColor(CRGBAColor c) { m_SelectedTextColor = c; }
	CRGBAColor GetSelectedTextColor() { return m_SelectedTextColor; }
	void SetSelectionColor(CRGBAColor c) { m_SelectionColor = c; }
	CRGBAColor GetSelectionColor() { return m_SelectionColor; }

	void SetTextIndex(int idx) { m_iTextIndex = idx; }
	int GetTextIndex() { return m_iTextIndex; }

	CVector2 ComputeTextBoundSize(CUiControl *ctrl);
	void LayoutEmbededObject(CUiControl *ctrl, const CVector2 &offset);

	virtual void ApplyAnimationTrack(CUiAnimationTrack &tk, float t);

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void Draw(IPlotter2D &canvas);

protected:

	void DrawPlainText(IPlotter2D &canvas);
	void DrawRichText(IPlotter2D &canvas);

	void SetTextStyle(ETextStyleFlag f, bool on);
	bool GetTextStyle(ETextStyleFlag f) { return (m_iTextStyles & f) != 0; }

	CUiFont *m_pFont;
	bool m_bUseControlColor;
	bool m_bRichText;
	ETextAlignment m_eHorizontalAlignment;
	ETextAlignment m_eVerticalAlignment;
	ETextLineMode m_eLineMode;
	CRGBAColor m_SelectedTextColor;
	CRGBAColor m_SelectionColor;
	CRGBAColor m_OutlineColor;
	CRGBAColor m_ShadowColor;
	CRGBAColor m_GradientColor;
	CVector2 m_ShadowOffset;
	float m_fShadowIntensity;
	int m_iTextStyles;
	int m_iTextIndex;
	float m_fLineSpacing;
};


}//GELib

#endif//__GELIB_UI_UITEXTPRESENTER_H__
