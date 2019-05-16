//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/StringUtility.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_STRINGUTILITY_H__
#define __GELIB_UTILITY_STRINGUTILITY_H__

#include <GELib/GeConfig.h>

#include <string>
#include <vector>
#include <stdarg.h>
#include <cstdio>

namespace GELib
{


#ifdef GELIB_WINDOWS_OR_XBOX
    #define SScanf sscanf_s
#else
    #define SScanf sscanf
#endif

GELIB_API int StrCmp(const char *str1, const char *str2);
GELIB_API int StrNCmp(const char *str1, const char *str2, size_t count);
GELIB_API int WcsCmp(const wchar_t *str1, const wchar_t *str2);
GELIB_API int WcsNCmp(const wchar_t *str1, const wchar_t *str2, size_t count);
GELIB_API int StrICmp(const char *str1, const char *str2);
GELIB_API int WcsICmp(const wchar_t *str1, const wchar_t *str2);
GELIB_API size_t StrLen(const char *str);
GELIB_API size_t WcsLen(const wchar_t *str);
GELIB_API const char * StrStr(const char *str, const char *strSearch);
GELIB_API const wchar_t * WcsStr(const wchar_t *str, const wchar_t *strSearch);
GELIB_API const char * StrChr(const char *str, int c);
GELIB_API const wchar_t * WcsChr(const wchar_t *str, wchar_t c);
GELIB_API void StrCpy(char *strDest, size_t sizeInBytes, const char *strSrc);
GELIB_API void WcsCpy(wchar_t *strDest, size_t sizeInWords, const wchar_t *strSrc);
GELIB_API void StrNCpy(char *strDest, size_t sizeInBytes, const char *strSrc, size_t count);
GELIB_API void WcsNCpy(wchar_t *strDest, size_t sizeInWords, const wchar_t *strSrc, size_t count);
GELIB_API void StrCat(char *strDest, size_t sizeInBytes, const char *strSrc);
GELIB_API void WcsCat(wchar_t *strDest, size_t sizeInWords, const wchar_t *strSrc);
GELIB_API void StrLwr(char *str, size_t sizeInBytes);
GELIB_API void WcsLwr(wchar_t *str, size_t sizeInWords);
GELIB_API void StrUpr(char *str, size_t sizeInBytes);
GELIB_API void WcsUpr(wchar_t *str, size_t sizeInWords);
GELIB_API void StrLwr(std::string &str);
GELIB_API void StrLwr(std::wstring &str);
GELIB_API void StrUpr(std::string &str);
GELIB_API void StrUpr(std::wstring &str);
GELIB_API std::string LowerString(const char *s);
GELIB_API std::string UpperString(const char *s);

GELIB_API const char * SetLocale(const char *locale);
GELIB_API size_t WcsToMbs(char *mbstr, size_t sizeInBytes, const wchar_t *wcstr, size_t count=-1);
GELIB_API size_t MbsToWcs(wchar_t *wcstr, size_t sizeInWords, const char *mbstr, size_t count=-1);
GELIB_API std::string WcsToMbs(const wchar_t *wcstr);	// max string length is 255
GELIB_API std::wstring MbsToWcs(const char *mbstr);	// max string length is 255

GELIB_API std::string Utf16ToUtf8(const wchar_t *s);
GELIB_API std::wstring Utf8ToUtf16(const char *s);

GELIB_API char * StrTok(char *strToken, const char *strDelimit, char **context);
GELIB_API wchar_t * WcsTok(wchar_t *strToken, const wchar_t *strDelimit, wchar_t **context);
GELIB_API void TokenizeString(const char *str, const char *delimit, std::vector<std::string> &tokens);
GELIB_API void TokenizeString(const wchar_t *str, const wchar_t *delimit, std::vector<std::wstring> &tokens);

GELIB_API char* StrDup(const char *string);
GELIB_API void StrFree(const char *string);

GELIB_API int SPrintf(char *buffer, size_t sizeInBytes, const char *format, ...);
GELIB_API int SnPrintf(char *buffer, size_t sizeInBytes, size_t count, const char *format, ...);
GELIB_API int SwPrintf(wchar_t *buffer, size_t sizeInWords, const wchar_t *format, ...);
GELIB_API int SnwPrintf(wchar_t *buffer, size_t sizeInWords, size_t count, const wchar_t *format, ...);

GELIB_API int VSPrintf(char *buffer, size_t sizeInBytes, const char *format, va_list args);
GELIB_API int VSnPrintf(char *buffer, size_t sizeInBytes, size_t count, const char *format, va_list args);
GELIB_API int VSwPrintf(wchar_t *buffer, size_t sizeInWords, const wchar_t *format, va_list args);
GELIB_API int VSnwPrintf(wchar_t *buffer, size_t sizeInWords, size_t count, const wchar_t *format, va_list args);

GELIB_API void IToA(int value, char *buffer, size_t sizeInBytes, int radix);
GELIB_API std::string IToA(int value, int radix);

GELIB_API void IToW(int value, wchar_t *buffer, size_t sizeInWords, int radix);
GELIB_API std::wstring IToW(int value, int radix);

GELIB_API double AToF(const char *string);
GELIB_API int AToI(const char *string);


}//GELib

#endif//__GELIB_UTILITY_STRINGUTILITY_H__
