//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/logFile.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_LOGFILE_H__
#define __BCLIB_UTILITY_LOGFILE_H__

#include <BCLib/utility/baseDef.h>
#include <BCLib/utility/thread/mutex.h>
#include <BCLib/utility/singleton.h>

#define BCLIB_LOGDEF_UINT32_BIT_MAX         (sizeof(uint32) * 8)
#define BCLIB_LOGDEF_MODULE_BIT_MAX         (5)
#define BCLIB_LOGDEF_MODULE_MAX             (ELOGMODULE_BCLIB_MAX / BCLIB_LOGDEF_UINT32_BIT_MAX)
#define BCLIB_LOGDEF_MODULE_POS_MASK        (0XFFE0)
#define BCLIB_LOGDEF_MODULE_BIT_MASK        (0X001F)
#define BCLIB_LOGDEF_MODULE_POS(module)     (BCLIB_LO_SHIFT((module - 1) & BCLIB_LOGDEF_MODULE_POS_MASK, BCLIB_LOGDEF_MODULE_BIT_MAX))
#define BCLIB_LOGDEF_MODULE_BIT(module)     ((module - 1) & BCLIB_LOGDEF_MODULE_BIT_MASK)

#define BCLIB_LOGDEF_LEVEL_MAX              (ELOGMODULE_BCLIB_MAX - 1)

namespace BCLib
{
enum ELogModule
{
    //���⹦��
    ELOGMODULE_ALL,
    ELOGMODULE_DEFAULT,

    //INLib ������ĵ� BCLib ����ʹ��
    ELOGMODULE_BCLIB_ASSERT             = 10,
    ELOGMODULE_BCLIB_UTILITY,
    ELOGMODULE_BCLIB_NETWORK,
    ELOGMODULE_BCLIB_FRAMEWORK,
    ELOGMODULE_BCLIB_P2PCLIENT,
    ELOGMODULE_BCLIB_P2PSERVER,
    ELOGMODULE_BCLIB_DATABASE,
    ELOGMODULE_BCLIB_LUASCRIPT,
    ELOGMODULE_BCLIB_SECURITY,

    //INLib ���������� XXLib ����ʹ��
    ELOGMODULE_BCLIB_RESERVED_FOR_AFLIB = 1000,
    ELOGMODULE_BCLIB_RESERVED_FOR_BTLIB = 2000,
    ELOGMODULE_BCLIB_RESERVED_FOR_CLLIB = 3000,
    ELOGMODULE_BCLIB_RESERVED_FOR_GELIB = 4000,
    ELOGMODULE_BCLIB_RESERVED_FOR_MWLIB = 5000,
    ELOGMODULE_BCLIB_RESERVED_FOR_SFLIB = 6000,
    ELOGMODULE_BCLIB_RESERVED_FOR_SSLIB = 7000,
    ELOGMODULE_BCLIB_RESERVED_FOR_TSLIB = 8000,

    //INLib ���嵼���� XXLib ����ʹ��
    ELOGMODULE_BCLIB_RESERVED_FOR_PBLIB = 10000,
    ELOGMODULE_BCLIB_RESERVED_FOR_PELIB = 11000,
    ELOGMODULE_BCLIB_RESERVED_FOR_MSLIB = 12000,
    ELOGMODULE_BCLIB_RESERVED_FOR_DTLIB = 13000,
    ELOGMODULE_BCLIB_RESERVED_FOR_LTLIB = 14000,

    //INLib ����滮�� XXLib ����ʹ��
    ELOGMODULE_BCLIB_RESERVED_FOR_SHLIB = 20000,
    ELOGMODULE_BCLIB_RESERVED_FOR_LSLIB = 21000,
    ELOGMODULE_BCLIB_RESERVED_FOR_ESLIB = 22000,
    ELOGMODULE_BCLIB_RESERVED_FOR_BSLIB = 23000,

    //INLib ���������ʹ��
    ELOGMODULE_BCLIB_EXTEND             = 30000,
    ELOGMODULE_BCLIB_MAX                = BCLIB_UINT16_MAX,
};

namespace Utility
{
extern void OutputDebug(char* a_msg, ...);

/// @brief �����ջ��Ϣ��Debug�汾��Ϣȫ��Release���Ż����ˣ�Linux ��Ҫ�ӱ������ - rdynamic
/// @return void
extern void DumpStack(int stackNum = 6);

class BCLIB_UTILITY_API CLog
{
public:
    enum ELogLevel
    {
        ELOGLEVEL_DEBUG = 0,  // ֻ�� Debug   ����ʾ�ĵ�����Ϣ
        ELOGLEVEL_TRACE,      // ���� Release ����ʾ�ĵ�����Ϣ
        ELOGLEVEL_INFOR,      // ��Ӫ��Ҫ����־��Ϣ

        ELOGLEVEL_WARNING,    // ����������������ֵĴ��󣬼�鵽������ʱ����Ҫ Return ������
        ELOGLEVEL_ERROR,      // �������̱���Ҫ�޸ĵĴ��󣬼�鵽������ʱ����Ҫ Return ������
        ELOGLEVEL_SYSTEM,     // ��Ҫ��ϵͳ��ʾ(������Դ������ɡ�ȫ����ʼ���ɹ���)

        ELOGLEVEL_STACK,      // �����ջ��Ϣ�� DumpStack ����ר��
        ELOGLEVEL_MAX
    };

protected:
    static const char* m_levelName[ELOGLEVEL_MAX];

public:
    CLog();
    virtual ~CLog();

	static const char* getLevelName(ELogLevel level);

    void setModuleLevel(uint16 module, ELogLevel level);
    void setModuleLevel(uint16 module, const char* level);
    ELogLevel getModuleLevel(uint16 module);

    void setModuleON(uint16 module);
    void setModuleOFF(uint16 module);

    void debug(uint16 module, const char* file, int line, const char* format, ...);
    void trace(uint16 module, const char* file, int line, const char* format, ...);
    void infor(uint16 module, const char* file, int line, const char* format, ...);
    void warning(uint16 module, const char* file, int line, const char* format, ...);
    void error(uint16 module, const char* file, int line, const char* format, ...);
    void system(uint16 module, const char* file, int line, const char* format, ...);

public:
    static void setGlobalLog(CLog* log);
    static CLog* getGlobalLog();

    static void globalStack(uint16 module, const char* file, int line, const char* format, ...);

    static void globalDebug(uint16 module, const char* file, int line, const char* format, ...);
    static void globalTrace(uint16 module, const char* file, int line, const char* format, ...);
    static void globalInfor(uint16 module, const char* file, int line, const char* format, ...);
    static void globalWarning(uint16 module, const char* file, int line, const char* format, ...);
    static void globalError(uint16 module, const char* file, int line, const char* format, ...);
    static void globalSystem(uint16 module, const char* file, int line, const char* format, ...);

    static void globalDebug(uint16 module, const char* file, int line, const char* funcname, const char* format, ...);
    static void globalTrace(uint16 module, const char* file, int line, const char* funcname, const char* format, ...);
    static void globalInfor(uint16 module, const char* file, int line, const char* funcname, const char* format, ...);
    static void globalWarning(uint16 module, const char* file, int line, const char* funcname, const char* format, ...);
    static void globalError(uint16 module, const char* file, int line, const char* funcname, const char* format, ...);
    static void globalSystem(uint16 module, const char* file, int line, const char* funcname, const char* format, ...);

public: // ��ʵ�ʿ����Ĺ����У�����Ҫ���ض����󣬽�����ϸ����־����
    static void setFilter(uint64 filter);
    static void filterDebug(uint64 filter, uint16 module, const char* file, int line, const char* funcname, const char* format, ...);
    static void filterTrace(uint64 filter, uint16 module, const char* file, int line, const char* funcname, const char* format, ...);
    static void filterInfor(uint64 filter, uint16 module, const char* file, int line, const char* funcname, const char* format, ...);
    static void filterWarning(uint64 filter, uint16 module, const char* file, int line, const char* funcname, const char* format, ...);
    static void filterError(uint64 filter, uint16 module, const char* file, int line, const char* funcname, const char* format, ...);
    static void filterSystem(uint64 filter, uint16 module, const char* file, int line, const char* funcname, const char* format, ...);

public:
	static void setDebugValue();
	static bool getDebugValue();

protected:
    virtual void _outLog(ELogLevel level, const char* file, int line, const char* format, va_list ap) = 0;
    virtual void _outLog(ELogLevel level, const char* file, int line, const char* funcname, const char* format, va_list ap) = 0;

private:
    bool _checkType(uint16 type)
    {
        uint16 pos = BCLIB_LOGDEF_MODULE_POS(type);
        uint16 bits = BCLIB_LOGDEF_MODULE_BIT(type);

        return (m_moduleFlags[pos] & BCLIB_HI_SHIFT(1, bits)) != 0;
    }

private:
    CLog::ELogLevel m_moduleLevels[ELOGMODULE_BCLIB_MAX];
    uint32 m_moduleFlags[BCLIB_LOGDEF_MODULE_MAX];
};

class CLogOutputThread;
class BCLIB_UTILITY_API CLogFile : public CLog
{
    friend class CLogOutputThread;
    BCLIB_SINGLETON_DECLARE(CLogFile);

protected:
    CLogFile();
    virtual ~CLogFile();

public:
    void setOutConsole(bool outConsole = true);
    // NOTICE: Only support local code page path!!!
    void setOutFile(const char* fileName, bool bDelOriginalFile = false, BCLib::uint16 uCreateIntervalInMin = 60);
    void setCachedLogCount(BCLib::uint16 uCount) { m_uCachedLogCount = uCount; }
    void forceFlush();
    void close();

    const std::string& getCurrentOutputLogFile();
protected:
    virtual void _outLog(CLog::ELogLevel level, const char* file, int line, const char* format, va_list ap);
    virtual void _outLog(CLog::ELogLevel level, const char* file, int line, const char* funcname, const char* format, va_list ap);

    typedef std::vector<std::string> TCachedLogs;
    void _getCachedLogs(TCachedLogs &cachedLogs);
private:
    bool m_outConsole;
    bool m_needOpenConsole;
    BCLib::uint16 m_uCachedLogCount;
    CMutex m_mutex;
    TCachedLogs m_cachedLogs;
    CLogOutputThread *m_pOutputThread;
};
}//Utility
}//BCLib


#define BCLIB_LOG_STACK(module, format, ...)    BCLib::Utility::CLog::globalStack(module, __FILE__, 0, format, ## __VA_ARGS__);

#define BCLIB_LOG_DEBUG(module, format, ...)    BCLib::Utility::CLog::globalDebug(module, __FILE__, __LINE__, __FUNCTION__, format, ## __VA_ARGS__);
#define BCLIB_LOG_TRACE(module, format, ...)    BCLib::Utility::CLog::globalTrace(module, __FILE__, __LINE__, __FUNCTION__, format, ## __VA_ARGS__);
#define BCLIB_LOG_INFOR(module, format, ...)    BCLib::Utility::CLog::globalInfor(module, __FILE__, __LINE__, __FUNCTION__, format, ## __VA_ARGS__);
#define BCLIB_LOG_WARNING(module, format, ...)  BCLib::Utility::CLog::globalWarning(module, __FILE__, __LINE__, __FUNCTION__, format, ## __VA_ARGS__);
#define BCLIB_LOG_ERROR(module, format, ...)    BCLib::Utility::CLog::globalError(module, __FILE__, __LINE__, __FUNCTION__, format, ## __VA_ARGS__);
#define BCLIB_LOG_SYSTEM(module, format, ...)   BCLib::Utility::CLog::globalSystem(module, __FILE__, __LINE__, __FUNCTION__, format, ## __VA_ARGS__);

 // ��ʵ�ʿ����Ĺ����У�����Ҫ���ض����󣬽�����ϸ����־����
#define BCLIB_FILTER_DEBUG(filter, module, format, ...)    BCLib::Utility::CLog::filterDebug(filter, module, __FILE__, __LINE__, __FUNCTION__, format, ## __VA_ARGS__);
#define BCLIB_FILTER_TRACE(filter, module, format, ...)    BCLib::Utility::CLog::filterTrace(filter, module, __FILE__, __LINE__, __FUNCTION__, format, ## __VA_ARGS__);
#define BCLIB_FILTER_INFOR(filter, module, format, ...)    BCLib::Utility::CLog::filterInfor(filter, module, __FILE__, __LINE__, __FUNCTION__, format, ## __VA_ARGS__);
#define BCLIB_FILTER_WARNING(filter, module, format, ...)  BCLib::Utility::CLog::filterWarning(filter, module, __FILE__, __LINE__, __FUNCTION__, format, ## __VA_ARGS__);
#define BCLIB_FILTER_ERROR(filter, module, format, ...)    BCLib::Utility::CLog::filterError(filter, module, __FILE__, __LINE__, __FUNCTION__, format, ## __VA_ARGS__);
#define BCLIB_FILTER_SYSTEM(filter, module, format, ...)   BCLib::Utility::CLog::filterSystem(filter, module, __FILE__, __LINE__, __FUNCTION__, format, ## __VA_ARGS__);

#define BCLIB_LOG_SET_MODULE_ON(module)\
    do {\
        BCLib::Utility::CLog* log = BCLib::Utility::CLog::getGlobalLog();\
        if (log != NULL) {\
            log->setModuleON(module);\
        }\
    } while(false);

#define BCLIB_LOG_SET_MODULE_OFF(module)\
    do {\
        BCLib::Utility::CLog* log = BCLib::Utility::CLog::getGlobalLog();\
        if (log != NULL) {\
            log->setModuleOFF(module);\
        }\
    } while(false);

#define BCLIB_LOG_SET_MODULE_LEVEL(module, level)\
    do {\
        BCLib::Utility::CLog* log = BCLib::Utility::CLog::getGlobalLog();\
        if (log != NULL) {\
            log->setModuleLevel(module, level);\
        }\
    } while(false);

#define BCLIB_LOG_GET_MODULE_LEVEL(module)\
    (BCLib::Utility::CLog::getGlobalLog() == NULL ? 0 : BCLib::Utility::CLog::getGlobalLog()->getModuleLevel(module))

#endif//__BCLIB_UTILITY_LOGFILE_H__
