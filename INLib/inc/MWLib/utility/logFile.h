//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/utility/logFile.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_UTILITY_LOGFILE_H__
#define __MWLIB_UTILITY_LOGFILE_H__

#include <BCLib/utility/logFile.h>

namespace MWLib
{
enum ELogModule
{
    ELOGMODULE_MWLIB_DEFAULT = BCLib::ELOGMODULE_BCLIB_RESERVED_FOR_MWLIB + 1,
    ELOGMODULE_MWLIB_KAFKA,
};

}//MWLib

#endif//__MWLIB_UTILITY_LOGFILE_H__
