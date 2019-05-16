//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiTextFormat.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UITEXTFORMAT_H__
#define __GELIB_UI_UITEXTFORMAT_H__

#include <vector>
#include <string>
#include "UiTypes.h"

namespace GELib
{


class CUiFont;

class CUiTextFormat
{
public:

	CUiTextFormat();
	virtual ~CUiTextFormat();

	void ClearLines();
	bool IsLineEmpty() { return m_Lines.empty(); }

	void BuildLines(const std::wstring &text, CUiFont *font, float width, ETextLineMode lineMode, bool ellipsis);
	bool ComputePositionInLine(int p, int &line, int &posInLine);

	class Line
	{
	public:
		int m_iFromChar;
		int m_iNumChars;
		int m_iAdvanceChars;
		float m_fWidth;
		bool m_bEllipsis;
	};

	int GetNumLines() { return (int)m_Lines.size(); }
	Line& GetLine(int idx) { return m_Lines[idx]; }

private:

	std::vector<Line> m_Lines;
};



}//GELib

#endif//__GELIB_UI_UITEXTFORMAT_H__
