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
#define BCLIB_LOGDEF_MODULE_MAX             (ELOGMODULE_MAX / BCLIB_LOGDEF_UINT32_BIT_MAX)
#define BCLIB_LOGDEF_MODULE_POS_MASK        (0XFFE0)
#define BCLIB_LOGDEF_MODULE_BIT_MASK        (0X001F)
#define BCLIB_LOGDEF_MODULE_POS(module)     (BCLIB_LO_SHIFT((module - 1) & BCLIB_LOGDEF_MODULE_POS_MASK, BCLIB_LOGDEF_MODULE_BIT_MAX))
#define BCLIB_LOGDEF_MODULE_BIT(module)     ((module - 1) & BCLIB_LOGDEF_MODULE_BIT_MASK)

#define BCLIB_LOGDEF_LEVEL_MAX              (ELOGMODULE_MAX - 1)

namespace BCLib
{
enum ELogModule
{
    //特殊功能
    ELOGMODULE_ALL              = 0,
    ELOGMODULE_DEFAULT,

    //BCLib 使用
    ELOGMODULE_BCLIB_ASSERT,
    ELOGMODULE_BCLIB_UTILITY    = 8,
    ELOGMODULE_BCLIB_NETWORK,
    ELOGMODULE_BCLIB_FRAMEWORK,
    ELOGMODULE_BCLIB_P2PCLIENT,
    ELOGMODULE_BCLIB_P2PSERVER,
    ELOGMODULE_BCLIB_DATABASE,
    ELOGMODULE_BCLIB_LUASCRIPT,
    ELOGMODULE_BCLIB_SECURITY,

    ELOGMODULE_BCLIB_RESERVED_FOR_AFLIB = 1000,
    ELOGMODULE_BCLIB_RESERVED_FOR_CLLIB = 2000,
    ELOGMODULE_BCLIB_RESERVED_FOR_GELIB = 3000,
    ELOGMODULE_BCLIB_RESERVED_FOR_GFLIB = 4000,
    ELOGMODULE_BCLIB_RESERVED_FOR_MWLIB = 5000,
    ELOGMODULE_BCLIB_RESERVED_FOR_SFLIB = 6000,
    ELOGMODULE_BCLIB_RESERVED_FOR_SSLIB = 7000,
    ELOGMODULE_BCLIB_RESERVED_FOR_TSLIB = 8000,

    //BCLib 以外的使用
    ELOGMODULE_BCLIB_EXTEND     = 10000,
    ELOGMODULE_MAX              = BCLIB_UINT16_MAX,
};

namespace Utility
{
extern void OutputDebug(char* a_msg, ...);

/// @brief 输出堆栈信息，Debug版本信息全，Release被优化掉了，Linux 需要加编译参数 - rdynamic
/// @return void
extern void DumpStack(int stackNum = 6);

class BCLIB_UTILITY_API CLog
{
public:
    enum ELogLevel
    {
        ELOGLEVEL_DEBUG = 0,  // 只在 Debug   中显示的调试信息
        ELOGLEVEL_TRACE,      // 能在 Release 中显示的调试信息
        ELOGLEVEL_INFOR,      // 运营需要的日志信息

        ELOGLEVEL_WARNING,    // 正常流程中允许出现的错误，检查到有问题时，不要 Return 出函数
        ELOGLEVEL_ERROR,      // 程序流程必须要修改的错误，检查到有问题时，需要 Return 出函数
        ELOGLEVEL_SYSTEM,     // 重要的系统提示(比如资源加载完成、全部初始化成功等)

        ELOGLEVEL_STACK,      // 输出堆栈信息的 DumpStack 函数专用
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

public: // 在实际开发的过程中，有需要对特定对象，进行详细的日志跟踪
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
    CLog::ELogLevel m_moduleLevels[ELOGMODULE_MAX];
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

 // 在实际开发的过程中，有需要对特定对象，进行详细的日志跟踪
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
