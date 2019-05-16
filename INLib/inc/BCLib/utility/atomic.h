//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/atomic.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_ATOMIC_H__
#define __BCLIB_UTILITY_ATOMIC_H__

#include <BCLib/utility/baseDef.h>

namespace BCLib
{
namespace Utility
{
//////////////////////////////////////////////////////////////////////////
/// @brief 对整数的原子操作
//////////////////////////////////////////////////////////////////////////
class BCLIB_UTILITY_API CAtomic
{
public:
    CAtomic(int32 data = 0)
    :m_data(data)
    {
    }

    /// @brief
    /// @return BCLib::int32 递增后的整数
    int32 increment();
    int32 decrement();

    int32 getValue()
    {
        return m_data;
    }

public:
    static int32 increment(volatile int32& data);
    static int32 decrement(volatile int32& data);

private:
    mutable volatile int32 m_data;
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_ATOMIC_H__
