//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/WindowUtility.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_WINDOWUTILITY_H__
#define __GELIB_UTILITY_WINDOWUTILITY_H__

#include <GELib/GeConfig.h>
#include <vector>
#include <string>

namespace GELib
{

GELIB_API void CaptureMouse(void *hwnd);
GELIB_API void UncaptureMouse();

GELIB_API void SetWindowTitle(void *hwnd, const char *text);
GELIB_API void SetWindowTitle(void *hwnd, const wchar_t *text);

GELIB_API void SetWindowPosition(void *hwnd, int x, int y, int w, int h);
GELIB_API void GetWindowPosition(void *hwnd, int &x, int &y, int &w, int &h);
GELIB_API void GetWindowClientSize(void *hwnd, int &w, int &h);
GELIB_API void* GetWindowFromPoint(int x, int y);

GELIB_API bool IsWindowActive(void *hwnd);
GELIB_API bool IsWindowMinimized(void *hwnd);
GELIB_API bool IsWindowMaximized(void *hwnd);
GELIB_API bool IsValidWindow(void *hwnd);
GELIB_API void ShowHideWindow(void *hwnd, bool show);

GELIB_API void CreateTextConsole();
GELIB_API void DeleteTextConsole();
GELIB_API std::string ReadTextConsole();

GELIB_API void WindowAcceptDropFiles(void *hwnd, bool accept=true);
GELIB_API int GetDropFiles(WParam wParam, std::vector<std::string> &files, int &x, int &y);

GELIB_API void GetScreenSize(int &w, int &h);

enum ECursorType
{
    CURSOR_NONE = 0,
    CURSOR_ARROW,
    CURSOR_IBEAM,
    CURSOR_WAIT,
    CURSOR_CROSS,
    CURSOR_UPARROW,
    CURSOR_SIZENWSE,
    CURSOR_SIZENESW,
    CURSOR_SIZEWE,
    CURSOR_SIZENS,
    CURSOR_SIZEALL,
    CURSOR_NO,
    CURSOR_HAND,
    CURSOR_APPSTARTING,
    CURSOR_HELP,
    CURSOR_MAX,
};

GELIB_API void SetCursorPosition(void *hwnd, int x, int y);
GELIB_API void GetCursorPosition(void *hwnd, int &x, int &y);
GELIB_API void SetCursor(ECursorType c);
GELIB_API void ShowCursor(bool show);

class CursorHandle;
GELIB_API CursorHandle* LoadCursorFile(const char *fileName);
GELIB_API void DeleteCursor(CursorHandle *c);
GELIB_API void SetCursor(CursorHandle *c);

GELIB_API bool IsKeyDown(int key);

std::string OpenFileDialog(void *hwndOwner=0, const char *filterName=0, const char *filter=0, const char *initDir=0, const char *initFile=0);
std::vector<std::string> OpenFilesDialog(void *hwndOwner=0, const char *filterName=0, const char *filter=0, const char *initDir=0, const char *initFile=0);
std::string SaveFileDialog(void *hwndOwner=0, const char *filterName=0, const char *filter=0, const char *initDir=0, const char *initFile=0);
std::string SaveFileDialog(void *hwndOwner, const char *filterNames[], const char *filters[], int numFilters, int &activeFilter, const char *initDir=0, const char *initFile=0);
bool ChooseColorDialog(void *hwndOwner, int rgb[3]);

class IFileDialogCallback
{
public:
	IFileDialogCallback() {}
	virtual ~IFileDialogCallback() {}
	virtual void OpenDialog() {}
	virtual void CloseDialog() {}
	virtual void SelectFile(const char *fileName) {}
};

void SetOpenFileDialogCallback(IFileDialogCallback *callback);
IFileDialogCallback* GetOpenFileDialogCallback();
void SetSaveFileDialogCallback(IFileDialogCallback *callback);
IFileDialogCallback* GetSaveFileDialogCallback();

enum EMessageBoxType
{
	MSGBOX_OK,
	MSGBOX_OK_CANCEL,
	MSGBOX_YES_NO,
	MSGBOX_YES_NO_CANCEL,
};

enum EMessageBoxReturn
{
	MSGBOX_RETURN_CANCEL,
	MSGBOX_RETURN_OK,
	MSGBOX_RETURN_YES,
	MSGBOX_RETURN_NO,
};

EMessageBoxReturn ShowMessageBox(void *hwndOwner, EMessageBoxType type, const char *txt, const char *caption="");



}//GELib

#endif//__GELIB_UTILITY_WINDOWUTILITY_H__
