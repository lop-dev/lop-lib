//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiControl.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UICONTROL_H__
#define __GELIB_UI_UICONTROL_H__

#include "UiObject.h"
#include "UiAnimation.h"

namespace GELib
{


class CUiStyle;
class CUiHitArea;
class CUiTexture;
class CUiSubTexture;
class CUiTextFormat;
class CUiRichTextFormat;
class CUiTextPresenter;

class CUiControl : public CUiObject
{
public:

	UICLASS_DECLARE(CUiControl);

	CUiControl(CUiSystem *system);
	virtual ~CUiControl();

	// cursor
	void SetCursor(int id);
	int GetCursor();
	int GetInheritedCursor();

	// text
	virtual void SetText(const std::wstring &text);
	const std::wstring& GetText();

	// ascii text
	void SetTextA(const char *text);
	void SetTextA(const std::string &text);
	std::string GetTextA();

	// texture
	bool SetTexture(const char *name);
	CUiTexture* GetTexture();
	void DeleteTexture();

	// subtexture
	bool SetSubTexture(const char *texSetName, const char *subTexName);
	void SetSubTexture(CUiSubTexture *st);
	CUiSubTexture* GetSubTexture();
	void DeleteSubTexture();

	// external texture or render target managed by other system
	void SetExternalTexture(int t) { m_iExternalTextureId = t; }
	int GetExternalTexture() { return  m_iExternalTextureId; }

	// style
	void SetStyle(const char *name);
	CUiStyle* GetStyle() { return m_pStyle; }
	const std::string& GetStyleName() { return m_StyleName; }
	CUiStyle* FindStyle(const char *name);

	// style animation
	bool SetStyleState(const char *name);
	bool SetStyleState(const char *names[], int count);
	void ResetStyleStateTime();
	float GetSpawnTime() { return m_fSpawnTime; }	// used by subtexture animation

	// auto size
	void SetAutoWidth(bool a);
	bool IsAutoWidth() { return m_bAutoWidth; }
	void SetAutoHeight(bool a);
	bool IsAutoHeight() { return m_bAutoHeight; }

	// hit area
	CUiHitArea* CreateHitArea();
	void DeleteHitArea();
	CUiHitArea* GetHitArea();
	virtual bool IsHit(const CVector2 &p, int *hitCode);
	virtual void DrawHitArea(IPlotter2D &canvas);

	// sound
	void SetSound(const char *name);
	const std::string& GetSound();
	void PlaySound();

	// used by CRichTextFormat
	CUiObject* FindEmbedSource(const char *name);

	// used by CUiImagePresenter
	virtual int GetDisplayTexture(int index=0);
	virtual bool GetDisplayTextureRect(int index, int &x, int &y, int &w, int &h);

	// used by CUiTextPresenter
	virtual const std::wstring& GetDisplayText(int index=0);
	virtual CUiTextFormat& GetTextFormat(int index);
	virtual CUiRichTextFormat& GetRichTextFormat(int index);

	virtual void ApplyStyle(CUiStyle *s);
	virtual void ChooseStyleState();
	virtual void DoAutoSize();
	virtual void DoLayout();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);
    virtual bool PostLoad();

	virtual void Update(float t);
	virtual void Draw(IPlotter2D &canvas);

protected:

	virtual void SizeChanged();

	virtual void OnEvent(CUiEvent &e);
	virtual void OnMouseDown(CUiMouseEvent &e);
	virtual void OnMouseUp(CUiMouseEvent &e);
	virtual void OnMouseDoubleClick(CUiMouseEvent &e);
	virtual void OnMouseMove(CUiMouseEvent &e);
	virtual void OnMouseWheel(CUiMouseEvent &e);
	virtual void OnMouseEnter(CUiMouseEvent &e);
	virtual void OnMouseLeave(CUiMouseEvent &e);
	virtual void OnMouseInside(CUiMouseEvent &e);
	virtual void OnMouseOutside(CUiMouseEvent &e);
	virtual void OnKeyDown(CUiKeyEvent &e);
	virtual void OnKeyUp(CUiKeyEvent &e);
	virtual void OnKeyChar(CUiKeyEvent &e);
	virtual void OnRequestDragDrop(CUiDragDropEvent &e);
	virtual void OnFinishDragDrop(CUiDragDropEvent &e);
	virtual void OnDragEnter(CUiDragDropEvent &e);
	virtual void OnDragLeave(CUiDragDropEvent &e);
	virtual void OnDragDrop(CUiDragDropEvent &e);
	virtual void OnGotFocus(CUiEvent &e);
	virtual void OnLostFocus(CUiEvent &e);
	virtual void OnShowTip(CUiEvent &e);
	virtual void OnHideTip(CUiEvent &e);

	CUiStyle* FindChildStyle(CUiStyle *s, const char *type);
	void DrawStyle(IPlotter2D &canvas, CUiStyle *s);
	void DebugDrawControl(IPlotter2D &canvas, bool drawText=true);

	void ClearTextFormat();
	void ClearTextFormatLines();

	int m_iCursorId;
	int m_iExternalTextureId;
	CUiTexture *m_pTexture;
	CUiSubTexture *m_pSubTexture;

	std::wstring m_Text;
	std::wstring m_DisplayText;
	bool m_bDisplayTextDirty;
	bool m_bDisplayTextTranslated;
	bool m_bAutoWidth;
	bool m_bAutoHeight;
	CUiTextFormat *m_pTextFormat;
	CUiRichTextFormat *m_pRichTextFormat;
	CUiTextPresenter *m_pTextPresenter;

	std::string m_StyleName;
	CUiStyle *m_pStyle;
	CUiAnimationPlayState m_StyleState;

	CUiHitArea *m_pHitArea;
	float m_fSpawnTime;

	std::string m_SoundName;
};



}//GELib

#endif//__GELIB_UI_UICONTROL_H__
