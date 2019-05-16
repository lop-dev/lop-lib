//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/random.h
//  author:     League of Perfect
/// @brief      产生随机数
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_RANDOM_H__
#define __BCLIB_UTILITY_RANDOM_H__

#include <BCLib/utility/baseDef.h>

namespace BCLib
{
namespace Utility
{
//////////////////////////////////////////////////////////////////////////
/// @brief 随机数
//////////////////////////////////////////////////////////////////////////
class BCLIB_UTILITY_API CRandom
{
public:
    /// @brief 随机数构造函数
    /// @return
    /// @param iSeed 随机种子
    CRandom(unsigned int iSeed);
    virtual ~CRandom() ;

    /// @brief随机[0,2^32]
    /// @return unsigned int
    unsigned int rand32Bits();
    /// @brief 产生 [iMin,iMax]区间的随机数
    /// @return unsigned int
    /// @param iMin
    /// @param iMax
    unsigned int randInt(unsigned int iMin, unsigned int iMax);

private:
    unsigned int _rand15Bits();
    unsigned int m_iSeed;
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_RANDOM_H__
