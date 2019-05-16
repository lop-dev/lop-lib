//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonDefine/logFile.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONDEFINE_LOGFILE_H__
#define __SFLIB_COMMONDEFINE_LOGFILE_H__

#include <BCLib/utility/logFile.h>
#include <BCLib/utility/errorFlow.h>

namespace SFLib
{
enum ELogModule
{
    ELOGMODULE_SFLIB_DEFAULT = BCLib::ELOGMODULE_BCLIB_RESERVED_FOR_SFLIB + 1,
    ELOGMODULE_SFLIB_COMMON,
    ELOGMODULE_SFLIB_SERVER,    // 服务器状态
    ELOGMODULE_SFLIB_MESSAGE,   // 消息流向
    ELOGMODULE_SFLIB_P2SP,      // P2SP相关
};
}//SFLib

#endif//__SFLIB_COMMONDEFINE_LOGFILE_H__
