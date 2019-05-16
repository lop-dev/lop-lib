//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiSystem.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UISYSTEM_H__
#define __GELIB_UI_UISYSTEM_H__

#include <GELib/Math/GeMath.h>
#include "UiEvent.h"
#include "UiInput.h"

#include <vector>

namespace GELib
{

class IPlotter2D;
class CIOStream;


class CUiObject;
class CUiTexture;
class CUiFont;
class CUiCursor;
class CUiResourceManager;
class CUiSubTexture;
class IUiSystemHelper;
class CIme;

const int UI_DRAWFLAG_HIT_AREA = (1 << 0);
const int UI_DRAWFLAG_DEBUG = (1 << 1);
const int UI_DRAWFLAG_NAME = (1 << 2);

const int UI_TIPFLAG_MOUSE_DOWN_HIDE = (1 << 0);
const int UI_TIPFLAG_MOUSE_WHEEL_HIDE = (1 << 1);

class CUiStatistic
{
public:
    CUiStatistic();
    void Reset();

    int m_iNumDrawCalls;
	int m_iNumTriangles;
};

class CUiSystem
{
public:
	CUiSystem(IUiSystemHelper *helper, CUiResourceManager *resManager);
	virtual ~CUiSystem();

	IUiSystemHelper* GetSystemHelper() { return m_pHelper; }
	CUiResourceManager* GetResourceManager() { return m_pResourceManager; }

	// input method editor
	void SetIme(CIme *ime) { m_pIme = ime; }
	CIme* GetIme() { return m_pIme; }

	void SetScreenSize(float w, float h);
	const CVector2& GetScreenSize();

	// inject message, return true if message is handled by ui
	bool Input(const CUiInput &in);

	// hide pop, remove focus
	void Inactive();

	void Update(float t);
	void Draw(IPlotter2D &canvas);
	void Draw(CUiObject *u, IPlotter2D &canvas);

	// save / load
	bool Save(const char *fname, CUiObject *u);
	bool Save(CIOStream &stream, CUiObject *u);
	CUiObject* Load(const char *fname);
	CUiObject* Load(CIOStream &stream);

	CUiObject* Duplicate(CUiObject *u);

	// root
	CUiObject* GetRoot();
	CUiObject* GetLibRoot();

	// debug, editor draw flag
	void SetDrawFlag(int flag) { m_iDrawFlag = flag; }
	int GetDrawFlag() { return m_iDrawFlag; }

	// objects management by ui system
	void CaptureMouse(CUiObject *u);
	void UncaptureMouse();
	CUiObject* GetCaptureMouse();

	void SetFocus(CUiObject *u);
	CUiObject* GetFocus();

	void SetImeCaret(CUiObject *u);
	CUiObject* GetImeCaret();

	CUiObject* GetMouseEnter();

	// popup
	void AddPopup(CUiObject *u, CUiObject *owner);
	void RemovePopup(CUiObject *u);

	// drag drop
	void SetDragLimit(float xmin, float ymin, float xmax, float ymax);
	void GetDragLimit(float &xmin, float &ymin, float &xmax, float &ymax);
	void StartDragDrop(CUiObject *source, CUiObject *icon, const CVector2 &iconOffset);
	CUiObject* GetDragSource();

	// mouse, key status
	const CVector2& GetMousePosition();
	bool IsMouseDown(EMouseButton button);
	bool IsKeyDown(int key);

	// tip delay
	void SetTipInitialDelay(float t);
	float GetTipInitialDelay();
	void SetTipReshowDelay(float t);
	float GetTipReshowDelay();
	void SetTipFlag(int flag) { m_iTipFlag = flag; }
	int GetTipFlag() { return m_iTipFlag; }

	// cursor
	void ShowCursor(bool show);
	void SetDefaultCursor(int id);
	int GetDefaultCursor();

	// style
	void ReapplyStyle(CUiObject *u=0);

	// frame
	int GetFrameCounter() { return m_iFrameCounter; }
	float GetFrameDeltaTime() { return m_fFrameDeltaTime; }
	float GetTime() { return m_fTime; }

	// resource management
	CUiTexture* CreateTexture(const char *name);
	void DeleteTexture(CUiTexture *tex);
	CUiSubTexture* GetSubTexture(const char *textureSetName, const char *subTextureName);
	CUiFont* GetFont(int id);
	bool TranslateString(CUiObject *u, const std::wstring &key, std::wstring &value);
	void PlaySound(const char *name);

	// ui object destroy event
	void Destroyed(CUiObject *u);
	void Deleted(CUiObject *u);

	// statistic
	const CUiStatistic* GetStatistic();
	void ResetStatistic();

private:

	bool CheckMousePosition(const CVector2 &p);
	CUiObject* MouseTarget(const CVector2 &p, int &hitCode);

	bool MouseDown(EMouseButton button, float x, float y, bool doubleClick);
	bool MouseUp(EMouseButton button, float x, float y);
	bool MouseMove(float x, float y);
	bool MouseWheel(float w, float x, float y);
	bool KeyDown(int key, bool repeat);
	bool KeyUp(int key);
	bool KeyChar(int c, bool repeat);

	void MouseEnter(CUiObject *u, const CVector2 &p, int hitCode);
	void MouseLeave(const CVector2 &p, int hitCode);
	void MouseInside(CUiObject *u, const CVector2 &p, int hitCode);
	void DragEnter(CUiObject *u, const CVector2 &p, int hitCode);
	void DragLeave(const CVector2 &p, int hitCode);
	void BeginDragDrop(CUiObject *u, const CVector2 &p, int hitCode);
	void EndDragDrop(const CVector2 &p, CUiObject *dropTarget, int hitCode);
	void CheckTip(const CVector2 &p);
	void ShowTip();
	void HideTip(bool reshow=false);
	void SetCursor(CUiObject *u);

	CUiObject* GetPopup();
	void ClosePopup();
	bool ErasePopup(CUiObject *u);

	void DeleteDestroyedUi();
	void ClearReference(CUiObject *u);

	bool IsHiddenOrDisabled(CUiObject *u, bool checkHidden, bool checkDisable);

	bool SaveTree(CIOStream &stream, CUiObject *u);
	CUiObject* LoadTree(short version, CIOStream &stream, std::vector<CUiObject*> &internalChildren);

	IUiSystemHelper *m_pHelper;
	CUiResourceManager *m_pResourceManager;
	CIme *m_pIme;

	CVector2 m_ScreenSize;
	int m_iDrawFlag;

	int m_iFrameCounter;
	float m_fFrameDeltaTime;
	float m_fTime;

	CUiObject *m_pRootUi;
	CUiObject *m_pLibRootUi;
	CUiObject *m_pMouseEnterUi;
	CUiObject *m_pMouseInsideUi;
	CUiObject *m_pCaptureUi;
	CUiObject *m_pFocusUi;
	CUiObject *m_pImeCaretUi;
	CUiObject *m_pDragSourceUi;
	CUiObject *m_pCandicateDragSourceUi;
	CUiObject *m_pDragIconUi;
	CUiObject *m_pTipSourceUi;
	CUiObject *m_pCandicateTipSourceUi;

	class CPopup
	{
	public:
		CPopup() { m_pUi = m_pOwner = 0; }
		CUiObject *m_pUi;
		CUiObject *m_pOwner;
	};
	std::vector<CPopup> m_PopupStack;

	CUiCursor *m_pCursor;
	float m_fCursorTime;
	int m_iDefaultCursor;

	CVector2 m_DragMouseDownPosition;
	CVector2 m_DragIconOffset;
	float m_fDragXMin, m_fDragYMin, m_fDragXMax, m_fDragYMax;
	bool m_bDragIconCanHit;

	CVector2 m_MousePosition;
	bool m_MouseButtons[3];
	char m_Keys[256];

	float m_fTipInitialDelay;
	float m_fTipReshowDelay;
	float m_fShowTipTimer;
	float m_fReshowTipTimer;
	int m_iTipFlag;

	std::vector<CUiObject*> m_DestroyedUi;
	CUiStatistic m_Statistic;
};



}//GELib

#endif//__GELIB_UI_UISYSTEM_H__
