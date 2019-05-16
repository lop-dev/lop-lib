//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/bufferImp.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_BUFFERIMP_H__
#define __BCLIB_UTILITY_BUFFERIMP_H__

#include <BCLib/utility/buffer.h>

namespace BCLib
{
namespace Utility
{
template class BCLIB_UTILITY_API CBuffer<std::vector<char>, char>;

template<>
inline void CBuffer<std::vector<char>, char>::init()
{
    m_bufferMaxSize = 8;
    m_buffer.resize(m_bufferMaxSize);
}

template<>
inline bool CBuffer<std::vector<char>, char>::writeReserve(uint32 reserveSize)
{
    if(writeSize() < reserveSize)
    {
        m_bufferMaxSize = (m_bufferMaxSize + reserveSize) * 2;
        m_buffer.resize(m_bufferMaxSize);
    }
    return true;
}

typedef CBuffer<std::vector<char>, char> CBufferInt8;
}//Utility
}//BCLib
#endif//__BCLIB_UTILITY_BUFFERIMP_H__
