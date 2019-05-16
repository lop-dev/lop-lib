//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiRichTextBox.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIRICHTEXTBOX_H__
#define __GELIB_UI_UIRICHTEXTBOX_H__

#include "UiTextBoxBase.h"
#include "UiRichTextFormat.h"

namespace GELib
{


class CUiFont;
class CRichTextElement;

class CUiRichTextBox : public CUiTextBoxBase
{
public:

	UICLASS_DECLARE(CUiRichTextBox);

	CUiRichTextBox(CUiSystem *system);
	virtual ~CUiRichTextBox();

	virtual void SetText(const std::wstring &text);

	void SetHtml(const wchar_t *html);

	void InsertImage(const char *fname, int width=0, int height=0);
	void InsertObject(CUiObject *u);	// a copy of CUiObject is stored

	void SetAttribute(const CRichTextAttrib &a);	// set selected text attrib
	const CRichTextAttrib& GetAttribute(int charIndex);

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void ApplyStyle(CUiStyle *s);

	virtual void Draw(IPlotter2D &canvas);

protected:

	virtual void OnKeyDown(CUiKeyEvent &e);

	virtual void DoEraseText(int index, int len);
	virtual void DoInsertText(int index, const wchar_t *txt);

	virtual void UpdateCaretPosition();
	virtual int GetCaretPositionFromMousePosition(const CVector2 &p);

	virtual int GetNumLines();
	virtual int GetLineFromChar(int line);
	virtual int GetLineNumChars(int line);
	virtual float GetLineWidth(int line);
	virtual float GetLineHeight(int line);
	virtual float ComputeAllLinesHeight();
	virtual float ComputeDisplayTextWidth(int start, int len);

	CUiRichTextFormat& GetUpdatedTextFormat();
	void InsertElement(int index, CRichTextElement *e);
	void SetDefaultAttribs();

	CRichTextAttribs m_Attribs;
};


}//GELib

#endif//__GELIB_UI_UIRICHTEXTBOX_H__
