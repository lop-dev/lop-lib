//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/errorFlow.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_ERRORFLOW_H__
#define __BCLIB_UTILITY_ERRORFLOW_H__

#include <BCLib/utility/logFile.h>

namespace BCLib
{
typedef int32 EActionCode;
const EActionCode EACTION_UNKNOW = 1; //! 未知错误

typedef int32 EErrorCode;
const EErrorCode EERROR_OK = 0;       //! 成功
const EErrorCode EERROR_UNKNOW = 1;   //! 未知错误

// 定义在 CPP 文件的前面
#define BCLIB_DEF_LOGMODULE(__module__)                                     \
    static BCLib::uint16 __logModule__ = __module__

// 当需要使用 *_GT* 的时候，定义在 函数 的前面
#define BCLIB_DEF_ERRORCODE                                                 \
    BCLib::EErrorCode __errCode__(BCLib::EERROR_OK);						\
	std::string __actPrompt__("EACTION_UNKNOW")

#define BCLIB_DEF_ERRORCODE_WITH_ACTION(__action__)                         \
	BCLib::EErrorCode __errCode__(BCLib::EERROR_OK);						\
	std::string __actPrompt__(#__action__)

#define BCLIB_DEF_ACTION(__action__)									    \
	std::string __actPrompt__(#__action__)

#define BCLIB_DEF_ACTION_FUNCTION											\
	std::string __actPrompt__(__FUNCTION__)

#define BCLIB_DEF_ERRORCODE_WITH_FUNCTION									\
	BCLib::EErrorCode __errCode__(BCLib::EERROR_OK);						\
	std::string __actPrompt__(__FUNCTION__)

#define BCLIB_DEF_ERR_EXIT                                                  \
		return;																\
	ERR_EXIT:

#define BCLIB_SELFDEF_ERR_EXIT                                             \
	ERR_EXIT:

// 获取 ErrorCode，在函数中使用
#define BCLIB_GET_ERRORCODE                                                 \
    __errCode__

// 设置 ErrorCode，在函数中使用
#define BCLIB_SET_GTC(__condition__, __code__)                              \
    BCLIB_LOG_TRACE(__logModule__,											\
        "[%s]BCLIB_SET_GTC(%s) CODE(%d)",									\
        __actPrompt__.c_str(),#__condition__, __code__);					\
	__errCode__ = __code__;													\
    goto ERR_EXIT;

////////////////////////////////////////////////////////////////////////////////////

// 扩展 BCLIB_LOG_XXX 宏定义
#define BCLIB_LOG_DEBUG_EF(format, ...)                                     \
    BCLIB_LOG_DEBUG(__logModule__,											\
		"[%s]"#format,														\
        __actPrompt__.c_str(), ## __VA_ARGS__);

#define BCLIB_LOG_TRACE_EF(format, ...)                                     \
    BCLIB_LOG_TRACE(__logModule__,											\
		"[%s]"#format,														\
        __actPrompt__.c_str(), ## __VA_ARGS__);

#define BCLIB_LOG_WARNING_EF(format, ...)                                   \
    BCLIB_LOG_WARNING(__logModule__,										\
		"[%s]"#format,														\
        __actPrompt__.c_str(), ## __VA_ARGS__);

#define BCLIB_LOG_ERROR_EF(format, ...)                                     \
    BCLIB_LOG_ERROR(__logModule__,											\
		"[%s]"#format,														\
        __actPrompt__.c_str(), ## __VA_ARGS__);

#define BCLIB_LOG_INFOR_EF(format, ...)                                     \
    BCLIB_LOG_INFOR(__logModule__,											\
		"[%s]"#format,														\
        __actPrompt__.c_str(), ## __VA_ARGS__);

#define BCLIB_LOG_SYSTEM_EF(format, ...)                                    \
    BCLIB_LOG_SYSTEM(__logModule__,											\
		"[%s]"#format,														\
        __actPrompt__.c_str(), ## __VA_ARGS__);

////////////////////////////////////////////////////////////////////////////////////

#define BCLIB_CKB(__condition__)                                            \
    do                                                                      \
    {                                                                       \
        if(!(__condition__))                                                \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKB(%s)",                                            \
                __actPrompt__.c_str(),#__condition__);                      \
        }                                                                   \
    }while(false)

#define BCLIB_CKB_L(__condition__, __logModule__)                           \
    do                                                                      \
    {                                                                       \
        if(!(__condition__))                                                \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKB(%s)",                                        \
                __actPrompt__.c_str(),#__condition__);                      \
        }                                                                   \
    }while(false)

#define BCLIB_CKB_RTV(__condition__)                                        \
    do                                                                      \
    {                                                                       \
        if(!(__condition__))                                                \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKB_RTV(%s)",                                    \
                __actPrompt__.c_str(),#__condition__);                      \
            return;                                                         \
        }                                                                   \
    }while(false)

#define BCLIB_CKB_RTV_L(__condition__, __logModule__)                       \
    do                                                                      \
    {                                                                       \
        if(!(__condition__))                                                \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKB_RTV(%s)",                                    \
                __actPrompt__.c_str(),#__condition__);                      \
            return;                                                         \
        }                                                                   \
    }while(false)

#define BCLIB_CKB_RTB(__condition__)                                        \
    do                                                                      \
    {                                                                       \
        if(!(__condition__))                                                \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKB_RTB(%s)",                                    \
                __actPrompt__.c_str(),#__condition__);                      \
            return false;                                                   \
        }                                                                   \
    }while(false)

#define BCLIB_CKB_RTB_L(__condition__, __logModule__)                       \
    do                                                                      \
    {                                                                       \
        if(!(__condition__))                                                \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKB_RTB(%s)",                                    \
                __actPrompt__.c_str(),#__condition__);                      \
            return false;                                                   \
        }                                                                   \
    }while(false)

#define BCLIB_CKB_RTC(__condition__, __code__)                              \
    do                                                                      \
    {                                                                       \
        if(!(__condition__))                                                \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKB_RTC(%s) CODE(%d)",                           \
                __actPrompt__.c_str(),#__condition__, __code__);            \
            return __code__;                                                \
        }                                                                   \
    }while(false)

#define BCLIB_CKB_RTC_L(__condition__, __code__, __logModule__)             \
    do                                                                      \
    {                                                                       \
        if(!(__condition__))                                                \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKB_RTC(%s) CODE(%d)",                           \
                __actPrompt__.c_str(),#__condition__, __code__);            \
            return __code__;                                                \
        }                                                                   \
    }while(false)

#define BCLIB_CKB_GTV(__condition__)                                        \
    do                                                                      \
    {                                                                       \
        if(!(__condition__))                                                \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKB_GTV(%s)",                                    \
                __actPrompt__.c_str(),#__condition__);                      \
            goto ERR_EXIT;                                                  \
        }                                                                   \
    }while(false)

#define BCLIB_CKB_GTV_L(__condition__, __logModule__)                       \
    do                                                                      \
    {                                                                       \
        if(!(__condition__))                                                \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKB_GTV(%s)",                                    \
                __actPrompt__.c_str(),#__condition__);                      \
            goto ERR_EXIT;                                                  \
        }                                                                   \
    }while(false)

#define BCLIB_CKB_GTC(__condition__, __code__)                              \
    do                                                                      \
    {                                                                       \
        if(!(__condition__))                                                \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKB_GTC(%s) CODE(%d)",                           \
                __actPrompt__.c_str(),#__condition__, __code__);            \
            __errCode__ = __code__;                                         \
            goto ERR_EXIT;                                                  \
        }                                                                   \
    }while(false)

#define BCLIB_CKB_GTC_L(__condition__, __code__, __logModule__)             \
    do                                                                      \
    {                                                                       \
        if(!(__condition__))                                                \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKB_GTC(%s) CODE(%d)",                           \
                __actPrompt__.c_str(),#__condition__, __code__);            \
            __errCode__ = __code__;                                         \
            goto ERR_EXIT;                                                  \
        }                                                                   \
    }while(false)

////////////////////////////////////////////////////////////////////////////////////

#define BCLIB_CKC(__condition__)                                            \
    do                                                                      \
    {                                                                       \
        BCLib::int32 __code__ = __condition__;                              \
        if(BCLib::EERROR_OK != __code__)                                    \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKC(%s) CODE(%d)",                               \
                __actPrompt__.c_str(),#__condition__, __code__);            \
        }                                                                   \
    }while(false)

#define BCLIB_CKC_L(__condition__, __logModule__)                           \
    do                                                                      \
    {                                                                       \
        BCLib::int32 __code__ = __condition__;                              \
        if(BCLib::EERROR_OK != __code__)                                    \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKC(%s) CODE(%d)",                               \
                __actPrompt__.c_str(),#__condition__, __code__);            \
        }                                                                   \
    }while(false)

#define BCLIB_CKC_RTV(__condition__)                                        \
    do                                                                      \
    {                                                                       \
        BCLib::int32 __code__ = __condition__;                              \
        if(BCLib::EERROR_OK != __code__)                                    \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKC_RTV(%s) CODE(%d)",                           \
                __actPrompt__.c_str(),#__condition__, __code__);            \
            return;                                                         \
        }                                                                   \
    }while(false)

#define BCLIB_CKC_RTV_L(__condition__, __logModule__)                       \
    do                                                                      \
    {                                                                       \
        BCLib::int32 __code__ = __condition__;                              \
        if(BCLib::EERROR_OK != __code__)                                    \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKC_RTV(%s) CODE(%d)",                           \
                __actPrompt__.c_str(),#__condition__, __code__);            \
            return;                                                         \
        }                                                                   \
    }while(false)

#define BCLIB_CKC_RTB(__condition__)                                        \
    do                                                                      \
    {                                                                       \
        BCLib::int32 __code__ = __condition__;                              \
        if(BCLib::EERROR_OK != __code__)                                    \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKC_RTB(%s) CODE(%d)",                           \
                __actPrompt__.c_str(),#__condition__, __code__);            \
            return false;                                                   \
        }                                                                   \
    }while(false)

#define BCLIB_CKC_RTB_L(__condition__, __logModule__)                       \
    do                                                                      \
    {                                                                       \
        BCLib::int32 __code__ = __condition__;                              \
        if(BCLib::EERROR_OK != __code__)                                    \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKC_RTB(%s) CODE(%d)",                           \
                __actPrompt__.c_str(),#__condition__, __code__);            \
            return false;                                                   \
        }                                                                   \
    }while(false)

#define BCLIB_CKC_RTS(__condition__)                                        \
    do                                                                      \
    {                                                                       \
        BCLib::int32 __code__ = __condition__;                              \
        if(BCLib::EERROR_OK != __code__)                                    \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKC_RTS(%s) CODE(%d)",                           \
                __actPrompt__.c_str(),#__condition__, __code__);            \
            return __code__;                                                \
        }                                                                   \
    }while(false)

#define BCLIB_CKC_RTS_L(__condition__, __logModule__)                       \
    do                                                                      \
    {                                                                       \
        BCLib::int32 __code__ = __condition__;                              \
        if(BCLib::EERROR_OK != __code__)                                    \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKC_RTS(%s) CODE(%d)",                           \
                __actPrompt__.c_str(),#__condition__, __code__);            \
            return __code__;                                                \
        }                                                                   \
    }while(false)

#define BCLIB_CKC_RTC(__condition__, __code__)                              \
    do                                                                      \
    {                                                                       \
        if(BCLib::EERROR_OK != (__condition__))                             \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKC_RTC(%s) CODE(%d)",                           \
                __actPrompt__.c_str(),#__condition__, __code__);            \
            return __code__;                                                \
        }                                                                   \
    }while(false)

#define BCLIB_CKC_RTC_L(__condition__, __code__, __logModule__)             \
    do                                                                      \
    {                                                                       \
        if(BCLib::EERROR_OK != (__condition__))                             \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKC_RTC(%s) CODE(%d)",                           \
                __actPrompt__.c_str(),#__condition__, __code__);            \
            return __code__;                                                \
        }                                                                   \
    }while(false)

#define BCLIB_CKC_GTV(__condition__)                                        \
    do                                                                      \
    {                                                                       \
        BCLib::int32 __code__ = __condition__;                              \
        if(BCLib::EERROR_OK != __code__)                                    \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKC_GTV(%s) CODE(%d)",                           \
                __actPrompt__.c_str(),#__condition__, __code__);            \
            goto ERR_EXIT;                                                  \
        }                                                                   \
    }while(false)

#define BCLIB_CKC_GTV_L(__condition__, __logModule__)                       \
    do                                                                      \
    {                                                                       \
        BCLib::int32 __code__ = __condition__;                              \
        if(BCLib::EERROR_OK != __code__)                                    \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKC_GTV(%s) CODE(%d)",                           \
                __actPrompt__.c_str(),#__condition__, __code__);            \
            goto ERR_EXIT;                                                  \
        }                                                                   \
    }while(false)

#define BCLIB_CKC_GTS(__condition__)                                        \
    do                                                                      \
    {                                                                       \
        __errCode__ = __condition__;                                        \
        if(BCLib::EERROR_OK != __errCode__)                                 \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKC_GTS(%s) CODE(%d)",                           \
                __actPrompt__.c_str(),#__condition__, __errCode__);         \
            goto ERR_EXIT;                                                  \
        }                                                                   \
    }while(false)

#define BCLIB_CKC_GTS_L(__condition__, __logModule__)                       \
    do                                                                      \
    {                                                                       \
        __errCode__ = __condition__;                                        \
        if(BCLib::EERROR_OK != __errCode__)                                 \
        {                                                                   \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKC_GTS(%s) CODE(%d)",                           \
                __actPrompt__.c_str(),#__condition__, __errCode__);         \
            goto ERR_EXIT;                                                  \
        }                                                                   \
    }while(false)

#define BCLIB_CKC_GTC(__condition__, __code__)                              \
    do                                                                      \
    {                                                                       \
        __errCode__ = __condition__;                                        \
        if(BCLib::EERROR_OK != __errCode__)                                 \
        {                                                                   \
            __errCode__ = __code__;                                         \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKC_GTC(%s) CODE(%d)",                           \
                __actPrompt__.c_str(),#__condition__, __code__);            \
            goto ERR_EXIT;                                                  \
        }                                                                   \
    }while(false)

#define BCLIB_CKC_GTC_L(__condition__, __code__, __logModule__)             \
    do                                                                      \
    {                                                                       \
        __errCode__ = __condition__;                                        \
        if(BCLib::EERROR_OK != __errCode__)                                 \
        {                                                                   \
            __errCode__ = __code__;                                         \
            BCLIB_LOG_ERROR(__logModule__,                                  \
                "[%s]BCLIB_CKC_GTC(%s) CODE(%d)",                           \
                __actPrompt__.c_str(),#__condition__, __code__);            \
            goto ERR_EXIT;                                                  \
        }                                                                   \
    }while(false)
}//BCLib

#endif//__BCLIB_UTILITY_ERRORFLOW_H__
