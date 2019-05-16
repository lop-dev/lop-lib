//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiTextBox.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UITEXTBOX_H__
#define __GELIB_UI_UITEXTBOX_H__

#include "UiTextBoxBase.h"

namespace GELib
{

class CUiTextBox : public CUiTextBoxBase
{
public:

	UICLASS_DECLARE(CUiTextBox);

	CUiTextBox(CUiSystem *system);
	virtual ~CUiTextBox();

	virtual void SetText(const std::wstring &text);

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void Draw(IPlotter2D &canvas);

protected:

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

	CUiTextFormat& GetUpdatedTextFormat();
};



}//GELib

#endif//__GELIB_UI_UITEXTBOX_H__
