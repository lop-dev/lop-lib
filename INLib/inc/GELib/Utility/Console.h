//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/Console.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_CONSOLE_H__
#define __GELIB_UTILITY_CONSOLE_H__

#include <string>
#include <vector>

#include "SystemUtility.h"
#include "CircularBuffer.h"

namespace GELib
{

class IPlotter2D;

class IConsoleCallback
{
public:
    IConsoleCallback() {};
    virtual ~IConsoleCallback() {};
    virtual void Command(const char *command) = 0;
    virtual void Print(const char *str) = 0;
};

inline int ConsoleColor(int r, int g, int b)
{
    return (r << 16) | (g << 8) | (b);
}

class CConsole
{
public:
    CConsole();
    virtual ~CConsole();

    void SetCallback(IConsoleCallback *cb);
    void SetTextSize(float size);
    void SetVisibleLines(int lines);
    void SetMaxCharsPerLine(int n);
    void SetPosition(float x, float y);

    void Update(float t);

    // return true if console process message
    bool KeyChar(int key);
    bool KeyDown(int key);

    void Draw(GELib::IPlotter2D &canvas);

    void Printf(const char *format, ...);
    void Print(const char *s);
    void ColorPrintf(int color, const char *format, ...);
    void ColorPrint(int color, const char *s);

    enum EConsoleState
    {
        STATE_HIDE = 0,
        STATE_INPUT,
        STATE_SHOW,
    };

    void SetState(EConsoleState s);
    EConsoleState GetState() const;
	void SetToggleKey(int key, bool control=false, bool shift=false, bool alt=false);

    void AddCommand(const char *command);
    void ExecuteCommand();
    void InsertCommandText(const char *command);
    void ClearCommandText();

private:

	bool IsToggleKey(int key);

    class Line
    {
    public:
        std::string m_Text;
        int m_iColor;
    };
    CCriticalSectionObject m_CS;
    std::vector<std::string> m_CommandBuffer;
    std::string m_CurCommand;
    CircularBuffer<Line> m_Lines;
    CircularBuffer<std::string> m_CommandHistory;
    int m_iCurHistory;
    EConsoleState m_eState;
    int m_iVisibleLines;
	int m_iMaxCharsPerLine;
    int m_iQueueLines;
    int m_iScrollBackLines;
    int m_iCursorPosition;
    float m_fTextSize;
    float m_fCursorTime;
    IConsoleCallback *m_pCallback;
    float m_fX;
    float m_fY;

	int m_iFrameCounter;
	int m_iToggleFrame;
	short m_iToggleKey;
	short m_iToggleKeyFlag;
};



}//GELib

#endif//__GELIB_UTILITY_CONSOLE_H__
