//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/strHelper.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_STRHELPER_H__
#define __BCLIB_UTILITY_STRHELPER_H__


#include <BCLib/utility/baseDef.h>
#include <string>
#include <vector>
#include <stdarg.h>
#include <cstdio>

namespace BCLib
{
namespace Utility
{

#ifdef _WIN32
    #define SScanf sscanf_s
#else
    #define SScanf sscanf
#endif

class BCLIB_UTILITY_API CStrHelper
{
public:
    static int strCmp(const char *str1, const char *str2);
    static int wcsCmp(const wchar_t *str1, const wchar_t *str2);
    static int strICmp(const char *str1, const char *str2);
    static int wcsICmp(const wchar_t *str1, const wchar_t *str2);
    static size_t strLen(const char *str);
    static size_t wcsLen(const wchar_t *str);
    static const char * strStr(const char *str, const char *strSearch);
    static const wchar_t * wcsStr(const wchar_t *str, const wchar_t *strSearch);
    static const char * strChr(const char *str, int c);
    static const wchar_t * wcsChr(const wchar_t *str, wchar_t c);
    static void strCpy(char *strDest, size_t sizeInBytes, const char *strSrc);
    static void wcsCpy(wchar_t *strDest, size_t sizeInWords, const wchar_t *strSrc);
    static void strNCpy(char *strDest, size_t sizeInBytes, const char *strSrc, size_t count);
    static void wcsNCpy(wchar_t *strDest, size_t sizeInWords, const wchar_t *strSrc, size_t count);
    static void strCat(char *strDest, size_t sizeInBytes, const char *strSrc);
    static void wcsCat(wchar_t *strDest, size_t sizeInWords, const wchar_t *strSrc);
    static void strLwr(char *str, size_t sizeInBytes);
    static void wcsLwr(wchar_t *str, size_t sizeInWords);
    static void strUpr(char *str, size_t sizeInBytes);
    static void wcsUpr(wchar_t *str, size_t sizeInWords);
    static void strLwr(std::string &str);
    static void strLwr(std::wstring &str);
    static void strUpr(std::string &str);
    static void strUpr(std::wstring &str);
    static std::string strLwr(const char *s);
    static std::string strUpr(const char *s);

    static const char * setLocale(const char *locale);
    static size_t wcsToMbs(char *mbstr, size_t sizeInBytes, const wchar_t *wcstr, size_t count=-1);
    static size_t mbsToWcs(wchar_t *wcstr, size_t sizeInWords, const char *mbstr, size_t count=-1);
    static std::string wcsToMbs(const wchar_t *wcstr);	// max string length is 255
    static std::wstring mbsToWcs(const char *mbstr);	// max string length is 255

    static std::string utf16ToUtf8(const wchar_t *s);
    static std::wstring utf8ToUtf16(const char *s);

    static char * strTok(char *strToken, const char *strDelimit, char **context);
    static wchar_t * wcsTok(wchar_t *strToken, const wchar_t *strDelimit, wchar_t **context);
    static void tokenizeString(const char *str, const char *delimit, std::vector<std::string> &tokens);
    static void tokenizeString(const wchar_t *str, const wchar_t *delimit, std::vector<std::wstring> &tokens);

    static char* strDup(const char *string);
    static void strFree(const char *string);

    static int vsPrintf(char *buffer, size_t sizeInBytes, const char *format, ...);
    static int vsnPrintf(char *buffer, size_t sizeInBytes, size_t count, const char *format, ...);
    static int vswPrintf(wchar_t *buffer, size_t sizeInWords, const wchar_t *format, ...);
    static int vsnwPrintf(wchar_t *buffer, size_t sizeInWords, size_t count, const wchar_t *format, ...);

    static int vsPrintf(char *buffer, size_t sizeInBytes, const char *format, va_list args);
    static int vsnPrintf(char *buffer, size_t sizeInBytes, size_t count, const char *format, va_list args);
    static int vswPrintf(wchar_t *buffer, size_t sizeInWords, const wchar_t *format, va_list args);
    static int vsnwPrintf(wchar_t *buffer, size_t sizeInWords, size_t count, const wchar_t *format, va_list args);

    static void iToA(int value, char *buffer, size_t sizeInBytes, int radix);
    static std::string iToA(int value, int radix);

    static void iToW(int value, wchar_t *buffer, size_t sizeInWords, int radix);
    static std::wstring iToW(int value, int radix);

    static double aToD(const char *string);
    static int aToI(const char *string);

    static bool safeStrCpy(char* dst, BCLib::int32 dstSize, const char* src);
};

}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_STRHELPER_H__
