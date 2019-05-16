//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/assert.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_ASSERT_H__
#define __BCLIB_UTILITY_ASSERT_H__

#include <BCLib/utility/logFile.h>

#if defined(_WIN32)
#elif defined(_LINUX)
#include <signal.h>
#endif

namespace BCLib
{
namespace Utility
{
void BCLIB_UTILITY_API abend();
}//Utility
}//BCLib

#ifdef _DEBUG

#define BCLIB_ASSERT(condition)\
    if (!(condition)) {\
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_BCLIB_ASSERT, "BCLIB_ASSERT(%s)", #condition);\
        BCLib::Utility::abend();\
    }

#define BCLIB_ASSERT_RTV(condition)\
    if (!(condition)) {\
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_BCLIB_ASSERT, "BCLIB_ASSERT(%s)", #condition);\
        BCLib::Utility::abend();\
        return ;\
    }

#define BCLIB_ASSERT_RTC(condition, res)\
    if (!(condition)) {\
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_BCLIB_ASSERT, "BCLIB_ASSERT(%s)", #condition);\
        BCLib::Utility::abend();\
        return res;\
    }

#define BCLIB_ASSERT_MSG(condition, format, ...)\
    if (!(condition)) {\
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_BCLIB_ASSERT, format, ## __VA_ARGS__);\
        BCLib::Utility::abend();\
    }

#define BCLIB_ASSERT_RTV_MSG(condition, format, ...)\
    if (!(condition)) {\
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_BCLIB_ASSERT, format, ## __VA_ARGS__);\
        BCLib::Utility::abend();\
        return ;\
    }

#define BCLIB_ASSERT_RTC_MSG(condition, res, format, ...)\
    if (!(condition)) {\
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_BCLIB_ASSERT, format, ## __VA_ARGS__);\
        BCLib::Utility::abend();\
        return res;\
    }

#else

#define BCLIB_ASSERT(condition)\
    if (!(condition)) {\
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_BCLIB_ASSERT, "BCLIB_ASSERT(%s)", #condition);\
    }

#define BCLIB_ASSERT_RTV(condition)\
    if (!(condition)) {\
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_BCLIB_ASSERT, "BCLIB_ASSERT(%s)", #condition);\
        return ;\
    }

#define BCLIB_ASSERT_RTC(condition, res)\
    if (!(condition)) {\
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_BCLIB_ASSERT, "BCLIB_ASSERT(%s)", #condition);\
        return res;\
    }

#define BCLIB_ASSERT_MSG(condition, format, ...)\
    if (!(condition)) {\
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_BCLIB_ASSERT, format, ## __VA_ARGS__);\
        return ;\
    }

#define BCLIB_ASSERT_RTV_MSG(condition, format, ...)\
    if (!(condition)) {\
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_BCLIB_ASSERT, format, ## __VA_ARGS__);\
        return ;\
    }

#define BCLIB_ASSERT_RTC_MSG(condition, res, format, ...)\
    if (!(condition)) {\
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_BCLIB_ASSERT, format, ## __VA_ARGS__);\
        return res;\
    }

#endif


#endif//__BCLIB_UTILITY_ASSERT_H__
