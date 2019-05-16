//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiEvent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIEVENT_H__
#define __GELIB_UI_UIEVENT_H__

#include <string>
#include <vector>
#include <GELib/Math/GeMath.h>
#include <GELib/Utility/FastDelegate.h>

namespace GELib
{


enum EMouseButton
{
	MOUSE_BUTTON_LEFT,
	MOUSE_BUTTON_RIGHT,
	MOUSE_BUTTON_MIDDLE,
};

enum EKey
{
	KEY_LBUTTON        = 0x01,
	KEY_RBUTTON        = 0x02,
	KEY_CANCEL         = 0x03,
	KEY_MBUTTON        = 0x04,
	KEY_XBUTTON1       = 0x05,
	KEY_XBUTTON2       = 0x06,
	KEY_BACK           = 0x08,
	KEY_TAB            = 0x09,
	KEY_CLEAR          = 0x0C,
	KEY_ENTER          = 0x0D,
	KEY_SHIFT          = 0x10,
	KEY_CONTROL        = 0x11,
	KEY_ALT            = 0x12,
	KEY_PAUSE          = 0x13,
	KEY_CAPITAL        = 0x14,
	KEY_KANA           = 0x15,
	KEY_JUNJA          = 0x17,
	KEY_FINAL          = 0x18,
	KEY_KANJI          = 0x19,
	KEY_ESCAPE         = 0x1B,
	KEY_CONVERT        = 0x1C,
	KEY_NONCONVERT     = 0x1D,
	KEY_ACCEPT         = 0x1E,
	KEY_MODECHANGE     = 0x1F,
	KEY_SPACE          = 0x20,
	KEY_PAGE_UP         = 0x21,
	KEY_PAGE_DOWN       = 0x22,
	KEY_END            = 0x23,
	KEY_HOME           = 0x24,
	KEY_LEFT           = 0x25,
	KEY_UP             = 0x26,
	KEY_RIGHT          = 0x27,
	KEY_DOWN           = 0x28,
	KEY_SELECT         = 0x29,
	KEY_PRINT          = 0x2A,
	KEY_EXECUTE        = 0x2B,
	KEY_SNAPSHOT       = 0x2C,
	KEY_INSERT         = 0x2D,
	KEY_DELETE         = 0x2E,
	KEY_HELP           = 0x2F,
	KEY_LWIN           = 0x5B,
	KEY_RWIN           = 0x5C,
	KEY_APPS           = 0x5D,
	KEY_SLEEP          = 0x5F,
	KEY_NUMPAD0        = 0x60,
	KEY_NUMPAD1        = 0x61,
	KEY_NUMPAD2        = 0x62,
	KEY_NUMPAD3        = 0x63,
	KEY_NUMPAD4        = 0x64,
	KEY_NUMPAD5        = 0x65,
	KEY_NUMPAD6        = 0x66,
	KEY_NUMPAD7        = 0x67,
	KEY_NUMPAD8        = 0x68,
	KEY_NUMPAD9        = 0x69,
	KEY_MULTIPLY       = 0x6A,
	KEY_ADD            = 0x6B,
	KEY_SEPARATOR      = 0x6C,
	KEY_SUBTRACT       = 0x6D,
	KEY_DECIMAL        = 0x6E,
	KEY_DIVIDE         = 0x6F,
	KEY_F1             = 0x70,
	KEY_F2             = 0x71,
	KEY_F3             = 0x72,
	KEY_F4             = 0x73,
	KEY_F5             = 0x74,
	KEY_F6             = 0x75,
	KEY_F7             = 0x76,
	KEY_F8             = 0x77,
	KEY_F9             = 0x78,
	KEY_F10            = 0x79,
	KEY_F11            = 0x7A,
	KEY_F12            = 0x7B,
	KEY_F13            = 0x7C,
	KEY_F14            = 0x7D,
	KEY_F15            = 0x7E,
	KEY_F16            = 0x7F,
	KEY_F17            = 0x80,
	KEY_F18            = 0x81,
	KEY_F19            = 0x82,
	KEY_F20            = 0x83,
	KEY_F21            = 0x84,
	KEY_F22            = 0x85,
	KEY_F23            = 0x86,
	KEY_F24            = 0x87,
	KEY_NUMLOCK        = 0x90,
	KEY_SCROLL         = 0x91,
	KEY_OEM_FJ_JISHO   = 0x92,
	KEY_OEM_FJ_MASSHOU = 0x93,
	KEY_OEM_FJ_TOUROKU = 0x94,
	KEY_OEM_FJ_LOYA    = 0x95,
	KEY_OEM_FJ_ROYA    = 0x96,
	KEY_LSHIFT         = 0xA0,
	KEY_RSHIFT         = 0xA1,
	KEY_LCONTROL       = 0xA2,
	KEY_RCONTROL       = 0xA3,
	KEY_LMENU          = 0xA4,
	KEY_RMENU          = 0xA5,
	KEY_BROWSER_BACK   = 0xA6,
	KEY_BROWSER_FORWARD = 0xA7,
	KEY_BROWSER_REFRESH = 0xA8,
	KEY_BROWSER_STOP   = 0xA9,
	KEY_BROWSER_SEARCH = 0xAA,
	KEY_BROWSER_FAVORITES = 0xAB,
	KEY_BROWSER_HOME   = 0xAC,
	KEY_VOLUME_MUTE    = 0xAD,
	KEY_VOLUME_DOWN    = 0xAE,
	KEY_VOLUME_UP      = 0xAF,
	KEY_MEDIA_NEXT_TRACK = 0xB0,
	KEY_MEDIA_PREV_TRACK = 0xB1,
	KEY_MEDIA_STOP     = 0xB2,
	KEY_MEDIA_PLAY_PAUSE = 0xB3,
	KEY_LAUNCH_MAIL    = 0xB4,
	KEY_LAUNCH_MEDIA_SELECT = 0xB5,
	KEY_LAUNCH_APP1    = 0xB6,
	KEY_LAUNCH_APP2    = 0xB7,
	KEY_OEM_1          = 0xBA,
	KEY_OEM_PLUS       = 0xBB,
	KEY_OEM_COMMA      = 0xBC,
	KEY_OEM_MINUS      = 0xBD,
	KEY_OEM_PERIOD     = 0xBE,
	KEY_OEM_2          = 0xBF,
	KEY_OEM_3          = 0xC0,
	KEY_OEM_4          = 0xDB,
	KEY_OEM_5          = 0xDC,
	KEY_OEM_6          = 0xDD,
	KEY_OEM_7          = 0xDE,
	KEY_OEM_8          = 0xDF,
	KEY_OEM_AX         = 0xE1,
	KEY_OEM_102        = 0xE2,
	KEY_ICO_HELP       = 0xE3,
	KEY_ICO_00         = 0xE4,
	KEY_PROCESSKEY     = 0xE5,
	KEY_ICO_CLEAR      = 0xE6,
	KEY_PACKET         = 0xE7,
	KEY_OEM_RESET      = 0xE9,
	KEY_OEM_JUMP       = 0xEA,
	KEY_OEM_PA1        = 0xEB,
	KEY_OEM_PA2        = 0xEC,
	KEY_OEM_PA3        = 0xED,
	KEY_OEM_WSCTRL     = 0xEE,
	KEY_OEM_CUSEL      = 0xEF,
	KEY_OEM_ATTN       = 0xF0,
	KEY_OEM_FINISH     = 0xF1,
	KEY_OEM_COPY       = 0xF2,
	KEY_OEM_AUTO       = 0xF3,
	KEY_OEM_ENLW       = 0xF4,
	KEY_OEM_BACKTAB    = 0xF5,
	KEY_ATTN           = 0xF6,
	KEY_CRSEL          = 0xF7,
	KEY_EXSEL          = 0xF8,
	KEY_EREOF          = 0xF9,
	KEY_PLAY           = 0xFA,
	KEY_ZOOM           = 0xFB,
	KEY_NONAME         = 0xFC,
	KEY_PA1            = 0xFD,
	KEY_OEM_CLEAR      = 0xFE,
};

enum EUiEventType
{
	UIEVENT_NONE,

	// CUiMouseEvent
	UIEVENT_MOUSE_DOWN,
	UIEVENT_MOUSE_UP,
	UIEVENT_MOUSE_DOUBLE_CLICK,
	UIEVENT_MOUSE_MOVE,
	UIEVENT_MOUSE_WHEEL,
	UIEVENT_MOUSE_ENTER,
	UIEVENT_MOUSE_LEAVE,
	UIEVENT_MOUSE_INSIDE,
	UIEVENT_MOUSE_OUTSIDE,

	// CUiKeyEvent
	UIEVENT_KEY_DOWN,
	UIEVENT_KEY_UP,
	UIEVENT_KEY_CHAR,

	// CUiDragDropEvent
	UIEVENT_REQUEST_DRAG_DROP,
	UIEVENT_FINISH_DRAG_DROP,
	UIEVENT_DRAG_ENTER,
	UIEVENT_DRAG_LEAVE,
	UIEVENT_DRAG_DROP,

	// CUiEvent
	UIEVENT_GOT_FOCUS,
	UIEVENT_LOST_FOCUS,
	UIEVENT_SHOW_TIP,
	UIEVENT_HIDE_TIP,
	UIEVENT_THUMB_BEGIN_DRAG,
	UIEVENT_THUMB_DRAG,
	UIEVENT_THUMB_END_DRAG,
	UIEVENT_CLICK,
	UIEVENT_VALUE_CHANGED,
	UIEVENT_CHECKED_CHANGED,
	UIEVENT_SELECTION_CHANGED,
	UIEVENT_COMMIT_SELECTION,
	UIEVENT_TEXT_CHANGED,
	UIEVENT_SCROLL_VIEW_CHANGED,
	UIEVENT_EXPAND_TREE,

	// CUiRequestScrollEvent
	UIEVENT_REQUEST_SCROLL,

	// CUiCommandEvent
	UIEVENT_ANIMATION_COMMAND,

	UIEVENT_BEGIN_TWEEK_VALUE,
	UIEVENT_END_TWEEK_VALUE,

	// user event
	UIEVENT_USER = 100,
};

class CUiObject;

class CUiEvent
{
public:
	CUiEvent();
	CUiEvent(EUiEventType e);
	virtual ~CUiEvent() {}
	EUiEventType m_eType;
	CUiObject *m_pSender;
	CUiObject *m_pSource;
	int m_iParam;
	bool m_bHandled;
};

class CUiMouseEvent : public CUiEvent
{
public:
	CUiMouseEvent();
	CUiMouseEvent(EUiEventType e);
	EMouseButton m_eButton;
	bool m_bDoubleClick;
	CVector2 m_P;
	float m_fWheel;
	int m_iHitCode;
};

class CUiKeyEvent : public CUiEvent
{
public:
	CUiKeyEvent();
	CUiKeyEvent(EUiEventType e);
	int m_iKey;
	bool m_bRepeat;
};

class CUiDragDropEvent : public CUiEvent
{
public:
	CUiDragDropEvent();
	CUiDragDropEvent(EUiEventType e);
	CUiObject *m_pDragSource;
	CUiObject *m_pDropTarget;
	CUiObject *m_pDragIcon;
	CVector2 m_P;
	int m_iHitCode;
};

class CUiCommandEvent : public CUiEvent
{
public:
	CUiCommandEvent();
	CUiCommandEvent(EUiEventType e);
	std::string m_Command;
};

class CUiRequestScrollEvent : public CUiEvent
{
public:
	CUiRequestScrollEvent();
	CUiRequestScrollEvent(EUiEventType e);
	CVector2 m_P;
	CVector2 m_Size;
};

typedef fastdelegate::FastDelegate1<CUiEvent&> CUiEventHandler;

class CUiEventHandlerList
{
public:

	class CHandler
	{
	public:
		CHandler(EUiEventType et, const CUiEventHandler &h, bool filter)
				{ m_eType = et; m_Handler = h; m_bFilter = filter; }
		EUiEventType m_eType;
		CUiEventHandler m_Handler;
		bool m_bFilter;
	};

	void Add(EUiEventType et, const CUiEventHandler &h, bool filter);
	void Remove(EUiEventType et, const CUiEventHandler &h, bool filter);
	void Execute(CUiEvent &e);
	void ExecuteFilter(CUiEvent &e);

private:
	std::vector<CHandler> m_Handlers;
};


}//GELib

#endif//__GELIB_UI_UIEVENT_H__
