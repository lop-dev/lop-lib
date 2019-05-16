//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/SystemUtility.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_SYSTEMUTILITY_H__
#define __GELIB_UTILITY_SYSTEMUTILITY_H__

#include <GELib/GeConfig.h>

#include <string>

namespace GELib
{


GELIB_API double GetMilliseconds();
GELIB_API void SleepMilliseconds(double ms);

GELIB_API void DebugPrint(const char *str);
GELIB_API void MessageBoxPrint(const char *str);

GELIB_API void SysBeep(int freq, int duration);

GELIB_API bool SetTextToClipboard(const char *str);
GELIB_API bool SetTextToClipboard(const wchar_t *str);
GELIB_API bool GetTextFromClipboard(std::string &str);
GELIB_API bool GetTextFromClipboard(std::wstring &str);

class ThreadHandle;
class CriticalSectionHandle;
class EventHandle;
class SemaphoreHandle;

GELIB_API CriticalSectionHandle* CreateCriticalSectionHandle();
GELIB_API void DeleteCriticalSectionHandle(CriticalSectionHandle *cs);
GELIB_API void EnterCriticalSectionHandle(CriticalSectionHandle *cs);
GELIB_API bool TryEnterCriticalSectionHandle(CriticalSectionHandle *cs);
GELIB_API void LeaveCriticalSectionHandle(CriticalSectionHandle *cs);

GELIB_API EventHandle* CreateEventHandle();
GELIB_API void DeleteEventHandle(EventHandle *e);
GELIB_API void TriggerEventHandle(EventHandle *e);
GELIB_API void WaitEventHandle(EventHandle *e);

GELIB_API SemaphoreHandle* CreateSemaphoreHandle();
GELIB_API void DeleteSemaphoreHandle(SemaphoreHandle *s);
GELIB_API void IncreaseSemaphoreHandle(SemaphoreHandle *s);
GELIB_API void WaitSemaphoreHandle(SemaphoreHandle *s);

class GELIB_API CCriticalSectionObject
{
public:
    CCriticalSectionObject() { m_hHandle = CreateCriticalSectionHandle(); }
    ~CCriticalSectionObject() { DeleteCriticalSectionHandle(m_hHandle); m_hHandle=0; }
    void Enter() { EnterCriticalSectionHandle(m_hHandle); }
    bool TryEnter() { return TryEnterCriticalSectionHandle(m_hHandle); }
    void Leave() { LeaveCriticalSectionHandle(m_hHandle); }
private:
    CriticalSectionHandle *m_hHandle;
};

class GELIB_API CCriticalSectionLock
{
public:
    CCriticalSectionLock(CCriticalSectionObject *cs) { m_pCS = cs; if (m_pCS) m_pCS->Enter(); }
    ~CCriticalSectionLock() { if (m_pCS) m_pCS->Leave(); m_pCS = 0; }
private:
    CCriticalSectionObject *m_pCS;
};

class GELIB_API CEventObject
{
public:
    CEventObject() { m_hHandle = CreateEventHandle(); }
    ~CEventObject() { DeleteEventHandle(m_hHandle); m_hHandle=0; }
    void Trigger() { TriggerEventHandle(m_hHandle); }
    void Wait() { WaitEventHandle(m_hHandle); }
private:
    EventHandle *m_hHandle;
};

class GELIB_API CSemaphoreObject
{
public:
    CSemaphoreObject() { m_hHandle = CreateSemaphoreHandle(); }
    ~CSemaphoreObject() { DeleteSemaphoreHandle(m_hHandle); m_hHandle=0; }
    void Increase() { IncreaseSemaphoreHandle(m_hHandle); }
    void Wait() { WaitSemaphoreHandle(m_hHandle); }
private:
    SemaphoreHandle *m_hHandle;
};


class GELIB_API CThread
{
public:
    CThread();
    virtual ~CThread();

    bool CreateAndRun(int stackSize=0);

	// can call after created
    void SetProcessor(int p);
    void WaitAndClose();

    virtual void Run();

private:
    ThreadHandle *m_hHandle;
    bool m_bThreadCreated;
};

GELIB_API unsigned int GetCurrentSystemThreadId();


}//GELib

#endif//__GELIB_UTILITY_SYSTEMUTILITY_H__
