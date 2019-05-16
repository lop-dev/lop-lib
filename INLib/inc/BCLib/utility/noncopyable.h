//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/noncopyable.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_NONCOPYABLE_H__
#define __BCLIB_UTILITY_NONCOPYABLE_H__

#include <BCLib/utility/baseDef.h>

namespace BCLib
{
namespace Utility
{
class  BCLIB_UTILITY_API CNoncopyable
{
private:
    CNoncopyable(const CNoncopyable&);
    const CNoncopyable& operator =(const CNoncopyable&);

protected:
    CNoncopyable()
    {
    }
    virtual ~CNoncopyable()
    {
    }
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_NONCOPYABLE_H__
