//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiStyle.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UISTYLE_H__
#define __GELIB_UI_UISTYLE_H__

#include "UiControl.h"

namespace GELib
{

class CUiStyle : public CUiControl
{
public:

	UICLASS_DECLARE(CUiStyle);

	CUiStyle(CUiSystem *system);
	virtual ~CUiStyle();

	void SetPadding(float left, float top, float right, float bottom);
	void GetPadding(float &left, float &top, float &right, float &bottom);

	// component size
	void SetItemWidth(float w) { m_fItemWidth = w; }
	float GetItemWidth() { return m_fItemWidth; }
	void SetItemHeight(float h) { m_fItemHeight = h; }
	float GetItemHeight() { return m_fItemHeight; }
	void SetItemHeight2(float h) { m_fItemHeight2 = h; }
	float GetItemHeight2() { return m_fItemHeight2; }
	void SetOffset(const CVector2 &offset) { m_Offset = offset; }
	const CVector2& GetOffset() { return m_Offset; }

	// scroll bar
	void SetScrollBarSize(float s) { m_fScrollBarSize = s; }
	float GetScrollBarSize() { return m_fScrollBarSize; }
	void SetHScrollBarSide(bool top) { m_bScrollBarSide[0] = top; }	// true: top, false: bottom
	bool GetHScrollBarSide() { return m_bScrollBarSide[0]; }
	void SetVScrollBarSide(bool left) { m_bScrollBarSide[1] = left; } // true: left, false: right
	bool GetVScrollBarSide() { return m_bScrollBarSide[1]; }

	// color
	void SetLineColor(CRGBAColor c) { m_LineColor[0] = c; }
	CRGBAColor GetLineColor() { return m_LineColor[0]; }
	void SetLineColor2(CRGBAColor c) { m_LineColor[1] = c; }
	CRGBAColor GetLineColor2() { return m_LineColor[1]; }

	class ChildStyle
	{
	public:
		std::string m_Type;
		std::string m_Name;
	};

	void AddChildStyle(const std::string &type, const std::string &name);
	void InsertChildStyle(int index, const std::string &type, const std::string &name);
	void RemoveChildStyle(int index);
	void RemoveAllChildStyles();
	int GetNumChildStyles();
	ChildStyle* GetChildStyle(int index);
	const std::string& GetChildStyleName(const std::string &type);

	void Draw(IPlotter2D &canvas, CUiControl *control);
	void Draw(IPlotter2D &canvas, const CVector2 &absPos, const CVector2 &size, CRGBAColor &color);

	CUiControl* GetActiveControl();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

protected:

	std::vector<ChildStyle> m_ChildStyles;
	CUiControl *m_pControl;
	float m_fPaddingLeft, m_fPaddingTop, m_fPaddingRight, m_fPaddingBottom;
	CVector2 m_Offset;
	float m_fItemWidth;
	float m_fItemHeight;
	float m_fItemHeight2;
	float m_fScrollBarSize;
	bool m_bScrollBarSide[2];
	CRGBAColor m_LineColor[2];
};

inline CUiControl* CUiStyle::GetActiveControl()
{
	if (m_pControl)
		return m_pControl;
	return this;
}



}//GELib

#endif//__GELIB_UI_UISTYLE_H__
