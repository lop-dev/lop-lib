//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiRichTextFormat.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIRICHTEXTFORMAT_H__
#define __GELIB_UI_UIRICHTEXTFORMAT_H__

#include <vector>
#include <string>
#include <GELib/Math/GeMath.h>
#include "UiTypes.h"

namespace GELib
{

class CUiObject;
class CUiControl;
class CUiFont;
class CUiTexture;
class CUiSubTexture;
class CUiStyle;
class CUiTextPresenter;
class CRichTextElement;
class CRichTextAttribs;

class CUiRichTextFormat
{
public:

	CUiRichTextFormat();
	virtual ~CUiRichTextFormat();

	void ClearLines();
	bool IsLineEmpty() { return m_Lines.empty(); }

	void ClearAll();	// clear elements and lines
	bool IsEmpty() { return m_pFirstElement == 0; }

	void BuildElements(const std::wstring &html, CUiControl *ctrl, CUiTextPresenter *tp);
	void BuildElements(const std::wstring &text, CUiControl *ctrl, CRichTextAttribs &attribs);
	void BuildAttribsFromElements(CUiControl *ctrl, CRichTextAttribs &attribs);
	void BuildLines(float width, ETextLineMode lineMode, bool ellipsis);

	bool ComputePositionInLine(int p, int &line, int &posInLine);
	float ComputeHeight(int fromLine, int numLines);
	int ComputeLineFromHeight(float y, float lineSpacing);

	class Line
	{
	public:
		int GetFromChar();
		int GetNumChars();
		int GetAdvance();

		CRichTextElement *m_pFirstElement;
		CRichTextElement *m_pLastElement;
		int m_iFirstFromChar;	// char index in first element of line
		int m_iLastNumChars;	// num chars in last element of line
		int m_iLastAdvanceChars;
		float m_fWidth;
		float m_fHeight;
		float m_fFirstWidth;	// width of first element of line
		bool m_bHasEmbededObject;
	};

	int GetNumLines() { return (int)m_Lines.size(); }
	Line& GetLine(int idx) { return m_Lines[idx]; }
	CRichTextElement* GetFirstElement() { return m_pFirstElement; }
	float GetHeight() { return m_fHeight; }
	bool HasEmbededObject() { return m_bHasEmbededObject; }

	std::wstring m_PlainText;

private:

	std::vector<Line> m_Lines;
	CRichTextElement *m_pFirstElement;
	float m_fHeight;
	bool m_bHasEmbededObject;
};

enum ERichTextElementType
{
	RTE_NONE,
	RTE_TEXT,
	RTE_IMAGE,
	RTE_LINE_BREAK,
	RTE_ANCHOR,
	RTE_EMBEDED_OBJECT,
};

class CRichTextElement
{
public:
	CRichTextElement();
	virtual ~CRichTextElement();

	ERichTextElementType m_eType;
	CVector2 m_Size;
	int m_iFromChar;	// char index in plain text
	int m_iNumChars;
	CRichTextElement *m_pNext;
};

class CRichTextRun : public CRichTextElement
{
public:
	CRichTextRun();
	virtual ~CRichTextRun();

	CUiFont *m_pFont;
	CRGBAColor m_Color;
	int m_iTextStyles;
};

class CRichTextImage : public CRichTextElement
{
public:
	CRichTextImage();
	virtual ~CRichTextImage();

	CUiTexture *m_pTexture;
	CUiSubTexture *m_pSubTexture;
	CUiStyle *m_pStyle;
};

class CRichTextLineBreak : public CRichTextElement
{
public:
	CRichTextLineBreak();
	virtual ~CRichTextLineBreak();
};

class CRichTextAnchor : public CRichTextElement
{
public:
	CRichTextAnchor();
	virtual ~CRichTextAnchor();
	std::string m_HRef;
};

class CRichTextEmbededObject : public CRichTextElement
{
public:
	CRichTextEmbededObject();
	virtual ~CRichTextEmbededObject();

	CUiObject *m_pObject;
};

class CRichTextAttrib
{
public:
	CRichTextAttrib()
	{
		m_pElement = 0;
		m_pFont = 0;
		m_iStyles = 0;
		m_Color = CRGBAColor(255, 255, 255);
	}
	CRichTextElement *m_pElement;
	CUiFont *m_pFont;
	int m_iStyles;
	CRGBAColor m_Color;
};

class CRichTextAttribs
{
public:
	CRichTextAttribs();
	~CRichTextAttribs();

	void SetDefault(CUiFont *font, int style, CRGBAColor color);
	const CRichTextAttrib GetDefault();

	void Resize(int size);
	void Insert(int index, int num, const CRichTextAttrib &a);
	void Erase(int index, int num);
	void Apply(int index, int num, const CRichTextAttrib &a);
	void ApplyDefault(int index, int num);
	CRichTextAttrib GetNearest(int index);
	CRichTextAttrib& Get(int index) { return m_Attribs[index]; }

private:

	void DeleteElements(int index, int num);
	std::vector<CRichTextAttrib> m_Attribs;
	CRichTextAttrib m_Default;

};

void ConvertTextToHtmlEscape(const std::wstring &text, std::wstring &out);


}//GELib

#endif//__GELIB_UI_UIRICHTEXTFORMAT_H__
