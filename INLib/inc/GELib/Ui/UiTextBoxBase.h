//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiTextBoxBase.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UITEXTBOXBASE_H__
#define __GELIB_UI_UITEXTBOXBASE_H__

#include "UiControl.h"

namespace GELib
{


class CUiCaret;

enum ECharCase
{
	CASE_NONE,
	CASE_LOWER,
	CASE_UPPER,
};

enum EAcceptableChar
{
	ACCEPT_ALL = 0,
	ACCEPT_ASCII = 1 << 0,		// 32-126
	ACCEPT_A_TO_Z = 1 << 1,		// A-Z, a-z
	ACCEPT_NUMBER = 1 << 2,		// 0-9
	ACCEPT_FLOAT = 1 << 3,		// 0-9, +-.Ee
};

class CUiTextBoxBase : public CUiControl
{
public:

	UICLASS_DECLARE(CUiTextBoxBase);

	CUiTextBoxBase(CUiSystem *system);
	virtual ~CUiTextBoxBase();

	virtual const std::wstring& GetDisplayText(int index=0);

	void SelectText(int start, int length);
	int GetSelectionStart() const;
	int GetSelectionLength() const;
	void SelectAll();
	void SelectNone();
	bool IsTextSelected() const;
	std::wstring GetSelectedText();

	virtual void EraseSelectedText();
	virtual void InsertText(const std::wstring &text);

	void SetCaretPosition(int p);
	int GetCaretPosition() const;

	// clipboard
	virtual void CutToClipboard();
	virtual void CopyToClipboard();
	virtual void PasteFromClipboard();

	void SetLineMode(ETextLineMode m);
	ETextLineMode GetLineMode() { return m_eLineMode; }

	void SetReadOnly(bool r);
	bool IsReadOnly() const { return m_bReadOnly; }

	void SetMaxTextLength(int len);
	int GetMaxTextLength() const { return m_iMaxTextLength; }

	void SetCharCase(ECharCase c);
	ECharCase GetCharCase() const { return m_eCharCase; }

	void SetAcceptableChars(int c);
	int GetAcceptableChars() const { return m_iAcceptableChars; }

	void SetPasswordChar(int c);
	int GetPasswordChar() const { return m_iPasswordChar; }

	void SetAutoSelectOnFocus(bool s) { m_bAutoSelectOnFocus = s; }
	bool IsAutoSelectOnFocus() const { return m_bAutoSelectOnFocus; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void ApplyStyle(CUiStyle *s);
	virtual void ChooseStyleState();

	virtual void DoLayout();
	virtual void DoAutoSize();

	virtual void DebugDraw(IPlotter2D &canvas);

protected:

	virtual void ParentSizeChanged();
	virtual void EnabledChanged(bool enable);

	virtual void OnEvent(CUiEvent &e);
	virtual void OnMouseDown(CUiMouseEvent &e);
	virtual void OnMouseUp(CUiMouseEvent &e);
	virtual void OnMouseMove(CUiMouseEvent &e);
	virtual void OnKeyDown(CUiKeyEvent &e);
	virtual void OnKeyChar(CUiKeyEvent &e);
	virtual void OnGotFocus(CUiEvent &e);
	virtual void OnLostFocus(CUiEvent &e);
	virtual void OnTextChanged(CUiEvent &e);
	virtual void OnRequestScroll(CUiRequestScrollEvent &e);

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

	void ShowHideCaret();
	void CheckCaretAndSelectionRange();
	void MoveCaret(int p, bool select);
	void EraseTextScroll(int start, int len);
	void ScrollAndUpdateCaret(const CVector2 &c, float lineHeight);
	void TextModified();

	float ComputeHorizontalOffset(int line);
	float ComputeVerticalOffset();

	void SearchWorld(int c, int &begin, int &end);
	static bool IsDelimiter(wchar_t c);
	void ForwardSkipZeroWidthChars(int &c);
	void BackwardSkipZeroWidthChars(int &c);

	wchar_t FilterChar(wchar_t c);
	void FilterText(std::wstring &text);
	void FilterControlChar(const std::wstring &text, std::wstring &out);

	CUiCaret *m_pCaret;

	float m_fPaddingLeft;
	float m_fPaddingTop;
	float m_fPaddingRight;
	float m_fPaddingBottom;

	bool m_bReadOnly;
	bool m_bAutoSelectOnFocus;
	ETextLineMode m_eLineMode;
	ECharCase m_eCharCase;
	int m_iMaxTextLength;
	int m_iAcceptableChars;
	int m_iPasswordChar;

	bool m_bMouseDown;
	bool m_bSelectWord;
	int m_iMouseDownCaretPosition;
	int m_iFocusFrameCounter;

	int m_iCaretPosition;
	int m_iSelectionStart;
	int m_iSelectionLength;
	float m_fSelectionDrawStart;
	float m_fSelectionDrawEnd;

	int m_iCaretLine;
	int m_iCaretPositionInLine;
	float m_fMoveCaretX;
	CVector2 m_Scroll;

	enum EKeyAction
	{
		ACT_NONE,
		ACT_MOVE,
		ACT_TYPE,
		ACT_DELETE,
		ACT_CLIPBOARD,
	};

	void PrepareUndo(EKeyAction a);
	EKeyAction m_ePrevAction;
	std::wstring m_UndoText;
};



}//GELib

#endif//__GELIB_UI_UITEXTBOXBASE_H__
