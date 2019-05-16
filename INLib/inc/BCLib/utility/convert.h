//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/convert.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_CONVERT_H__
#define __BCLIB_UTILITY_CONVERT_H__

#include <BCLib/utility/baseDef.h>

namespace BCLib
{
namespace Utility
{
//////////////////////////////////////////////////////////////////////////
/// @brief 字符串转换
//////////////////////////////////////////////////////////////////////////
class BCLIB_UTILITY_API CConvert
{
public:
    /// @brief 将UTF-8编码转换成UNICODE编码
    /// @return int 写入缓冲区的宽字符个数
    /// @param outBuff 输出UNICODE编码缓冲区
    /// @param buffSize 输出缓冲区大小(单位:sizeof(wchar_t))
    /// @param text 输入UTF-8编码文本
    /// @param textSize 输入编码文本大小
    static int utf8ToUnicode(wchar_t* outBuff, int buffSize, const char* text, int textSize);
    static int utf8ToUnicode(std::wstring& outBuff, const std::string& text);

    /// @brief 将UNICODE编码转换成UTF-8编码
    /// @return int 写入缓冲区的字节数
    /// @param outBuff 输出UTF-8编码缓冲区
    /// @param buffSize 输出缓冲区大小
    /// @param text 输入UNICODE编码文本
    /// @param textSize 输入编码文本大小(单位:sizeof(wchar_t))
    static int unicodeToUTF8(char* outBuff, int buffSize, const wchar_t* text, int textSize);
    static int unicodeToUTF8(std::string& outBuff, const std::wstring& text);

    /// @brief 将UNICODE编码转换成GB2312编码
    /// @return int 写入缓冲区的字节数
    /// @param outBuff 输出GB2312编码缓冲区
    /// @param buffSize 输出缓冲区大小
    /// @param text 输入UNICODE编码文本
    /// @param textSize 输入编码文本大小(单位:sizeof(wchar_t))
    static int unicodeToGB2312(char* outBuff, int buffSize, const wchar_t* text, int textSize);
    static int unicodeToGB2312(std::string& outBuff, const std::wstring& text);

    /// @brief 将GB2312编码转换成UNICODE编码
    /// @return int 写入缓冲区的宽字符个数
    /// @param outBuff 输出UNICODE编码缓冲区
    /// @param buffSize 输出缓冲区大小(单位:sizeof(wchar_t))
    /// @param text 输入GB2312编码文本
    /// @param textSize 输入编码文本大小
    static int gb2312ToUnicode(wchar_t* outBuff, int buffSize, const char* text, int textSize);
    static int gb2312ToUnicode(std::wstring& outBuff, const std::string& text);

    /// @brief 将GB2312编码转换成UTF-8编码
    /// @return int 写入缓冲区的字节数
    /// @param outBuff 输出UTF-8编码缓冲区
    /// @param buffSize 输出缓冲区大小
    /// @param text 输入GB2312编码文本
    /// @param textSize 输入编码文本大小
    static int gb2312ToUTF8(char* outBuff, int buffSize, const char* text, int textSize);
    static int gb2312ToUTF8(std::string& outBuff, const std::string& text);

    /// @brief 将UTF-8编码转换成GB2312编码
    /// @return int 写入缓冲区的字节数
    /// @param outBuff 输出GB2312编码缓冲区
    /// @param buffSize 输出缓冲区大小(字节)
    /// @param text 输入UTF-8编码文本
    /// @param textSize 输入编码文本大小(字节)
    static int utf8ToGB2312(char* outBuff, int buffSize, const char* text, int textSize);
    static int utf8ToGB2312(std::string& outBuff, const std::string& text);

    static std::string toStringA(const int32 n);
    static std::string toStringA(const uint32 n);
    static std::string toStringA(const int64 n);
    static std::string toStringA(const uint64 n);
    static std::string toStringA(const float f);
    static std::string toStringA(const double d);

    static std::wstring toStringW(const int32 n);
    static std::wstring toStringW(const uint32 n);
    static std::wstring toStringW(const int64 n);
    static std::wstring toStringW(const uint64 n);
    static std::wstring toStringW(const float f);
    static std::wstring toStringW(const double d);

    static int8 toInt8(const std::string& str);
    static uint8 toUint8(const std::string& str);
    static int16 toInt16(const std::string& str);
    static uint16 toUint16(const std::string& str);
    static int32 toInt32(const std::string& str);
    static uint32 toUint32(const std::string& str);
    static int64 toInt64(const std::string& str);
    static uint64 toUint64(const std::string& str);
    static float toFloat(const std::string& str);
    static double toDouble(const std::string& str);

    static int8 toInt8(const std::wstring& str);
    static uint8 toUint8(const std::wstring& str);
    static int16 toInt16(const std::wstring& str);
    static uint16 toUint16(const std::wstring& str);
    static int32 toInt32(const std::wstring& str);
    static uint32 toUint32(const std::wstring& str);
    static int64 toInt64(const std::wstring& str);
    static uint64 toUint64(const std::wstring& str);
    static float toFloat(const std::wstring& str);
    static double toDouble(const std::wstring& str);
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_CONVERT_H__
