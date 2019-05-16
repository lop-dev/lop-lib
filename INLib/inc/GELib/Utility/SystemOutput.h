//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/SystemOutput.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_SYSTEMOUTPUT_H__
#define __GELIB_UTILITY_SYSTEMOUTPUT_H__

#include <GELib/GeConfig.h>


namespace GELib
{


class CSystemOutput
{
public:

    enum EOutputLevel
    {
        SYSOUTPUT_NORMAL,
        SYSOUTPUT_WARNING,
        SYSOUTPUT_ERROR
    };

    GELIB_API CSystemOutput();
    GELIB_API virtual ~CSystemOutput();

    GELIB_API void Printf(const char *format, ...);
    GELIB_API void WarningPrintf(const char *format, ...);
    GELIB_API void ErrorPrintf(const char *format, ...);

    GELIB_API void Printf(const wchar_t *format, ...);
    GELIB_API void WarningPrintf(const wchar_t *format, ...);
    GELIB_API void ErrorPrintf(const wchar_t *format, ...);

    GELIB_API virtual void Print(EOutputLevel level, const char *str) = 0;
    GELIB_API virtual void Print(EOutputLevel level, const wchar_t *str);
};


GELIB_API void SetSystemOutput(CSystemOutput *o);
GELIB_API CSystemOutput* GetSystemOutput();


}//GELib


#endif//__GELIB_UTILITY_SYSTEMOUTPUT_H__
