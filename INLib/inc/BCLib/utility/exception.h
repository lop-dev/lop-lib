//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/exception.h
//  author:     League of Perfect
/// @brief      CException 异常抛出基类
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_EXCEPTION_H__
#define __BCLIB_UTILITY_EXCEPTION_H__

#include <BCLib/utility/string.h>

namespace BCLib
{
namespace Utility
{
class BCLIB_UTILITY_API CException
{
public:
    /// @brief    CException 构造函数
    /// @param major 异常主ID
    /// @param minor 异常次ID
    /// @param errorMsg 异常注释
    /// @param osError 通过GetLastError/errno获取的系统异常ID，当 osError = -1 时，自动获取.
    CException(int16 major, int16 minor, const CStringA& errorMsg = "Unknown error", int osError = 0);
    CException(const CException& e);
    virtual ~CException();

    CException& operator =(CException& e);

    const CStringA& getErrorStr() const
    {
        return m_errorMsg;
    }
    uint32 getErrorCode() const
    {
        return BCLIB_HI_SHIFT(m_major, 16) + m_minor;
    }

    /// @brief    设置异常
    /// @param major 异常主ID
    /// @param minor 异常次ID
    /// @param errorMsg 异常注释
    /// @param osError 通过GetLastError/errno获取的系统异常ID，当 osError = -1 时，自动获取.
    void setError(int16 major, int16 minor, const CStringA& errorMsg = "Unknown error", int osError = 0);

public:
    static void throwRruntimeError(const std::string strError);

    /// @brief    设置异常
    /// @param major 异常主ID
    /// @param minor 异常次ID
    /// @param errorMsg 异常注释
    /// @param osError 通过GetLastError/errno获取的系统异常ID，当 osError = -1 时，自动获取.
    static void setException(int16 major, int16 minor, const CStringA& errorMsg = "Unknown error", int osError = 0);
    static void setException(CException* err);
    static CException& getException();
    static void clearException();

private:
    uint16 m_major;
    uint16 m_minor;
    uint32 m_osErrno;
    CStringA m_errorMsg;
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_EXCEPTION_H__
