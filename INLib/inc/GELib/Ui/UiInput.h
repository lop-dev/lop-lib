//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiInput.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIINPUT_H__
#define __GELIB_UI_UIINPUT_H__

#include <GELib/GeConfig.h>

namespace GELib
{


class CUiInput
{
public:
	CUiInput();
	void Clear();

	bool FromMessage(void *hWnd, unsigned int msg, WParam wParam, LParam lParam);

	// button 0: left, 1: right, 2: middle
	void MouseDown(int button, float x, float y, bool doubleClick);
	void MouseUp(int button, float x, float y);
	void MouseMove(float x, float y);
	void MouseWheel(float w, float x, float y);
	void KeyDown(int key, bool repeat);
	void KeyUp(int key);
	void KeyChar(int key, bool repeat);

	enum EType
	{
		NONE,
		MOUSE_DOWN,
		MOUSE_UP,
		MOUSE_MOVE,
		MOUSE_WHEEL,
		KEY_DOWN,
		KEY_UP,
		KEY_CHAR,
	};

	EType type;
	float x;
	float y;
	float wheel;
	int key;
	bool doubleClick;
	bool keyRepeat;
	char button;
};


}//GELib


#endif//__GELIB_UI_UIINPUT_H__
