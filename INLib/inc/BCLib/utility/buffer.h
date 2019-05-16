//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/buffer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_BUFFER_H__
#define __BCLIB_UTILITY_BUFFER_H__

#include <BCLib/utility/baseDef.h>

namespace BCLib
{
namespace Utility
{
//////////////////////////////////////////////////////////////////////////
/// @brief 缓冲区，现在支持 typedef CBuffer< std::vector<char>, char> CBufferInt8;
///  如果要支持其他类型的缓冲区，需要自行实现init、writeReserve
//////////////////////////////////////////////////////////////////////////
template<typename BUFFTYPE, typename ITEMTYPE = unsigned char>
class CBuffer
{
public:
    CBuffer()
    :m_bufferMaxSize(0)
    ,m_readPosition(0)
    ,m_writePosition(0)
    {
        init();
    }

    virtual ~CBuffer()
    {
    }

    /// @brief 初始化
    /// @return void
    inline void init();

    /// @brief 申请空间
    /// @return bool
    /// @param size
    inline bool writeReserve(uint32 size);

    /// @brief 写入数据
    /// @return bool 写入是否成功
    /// @param data 待写的数据
    /// @param size
    bool push(const ITEMTYPE* data, uint32 size)
    {
        if(data == NULL || size == 0)
        {
            return true;
        }
        if(size < 0)
        {
            return false;
        }
        if(!writeReserve(size))
        {
            return false;
        }
        memcpy(&m_buffer[m_writePosition], data, size);
        writeFlip(size);
        return true;
    }

    /// @brief 读出所有的可读的数据
    /// @return BCLib::uint32 实际读取的长度
    /// @param buff 传出参数，保存结果数据
    /// @param buffSize 准备读取的长度
    uint32 pop(ITEMTYPE* buff, uint32 buffSize)
    {
        if(buff == NULL)
        {
            return 0;
        }
        uint32 size = readSize();
        if(size <= 0)
        {
            return 0;
        }
        if(buffSize < size)
        {
            size = buffSize;
        }
        memcpy(buff, &m_buffer[m_readPosition], size);
        readFlip(size);
        return size;
    }

    /// @brief 可写地址
    /// @return ITEMTYPE*
    ITEMTYPE* writePtr()
    {
        uint32 tmp = m_writePosition - m_readPosition;
        if(m_readPosition >= tmp)
        {
            memmove(&m_buffer[0], &m_buffer[m_readPosition], tmp);
            m_readPosition = 0;
            m_writePosition = tmp;
        }

        return &m_buffer[m_writePosition];
    }

    /// @brief 可读地址
    /// @return const ITEMTYPE*
    const ITEMTYPE* readPtr() const
    {
        return &m_buffer[m_readPosition];
    }

    /// @brief 是否可读
    /// @return bool
    bool readReady() const
    {
        return m_writePosition - m_readPosition > 0;
    }

    /// @brief 可读空间大小
    /// @return BCLib::uint32
    uint32 readSize() const
    {
        return m_writePosition - m_readPosition;
    }

    /// @brief 可写空间大小
    /// @return BCLib::uint32
    uint32 writeSize() const
    {
        return m_bufferMaxSize - m_writePosition;
    }

    void reset()
    {
        m_readPosition = 0;
        m_writePosition = 0;
    }

    /// @brief 内部函数,对overlap读进行处理
    /// @return void
    /// @param size
    void readFlip(uint32 size)
    {
        m_readPosition += size;
        if(m_readPosition >= m_writePosition)
        {
            m_readPosition = 0;
            m_writePosition = 0;
        }
    }

    bool writeFlip(uint32 size)
    {
        if(m_writePosition + size > m_bufferMaxSize)
        {
            return false;
        }
        m_writePosition += size;

        uint32 tmp = m_writePosition - m_readPosition;
        if(m_readPosition >= tmp)
        {
            memmove(&m_buffer[0], &m_buffer[m_readPosition], tmp);
            m_readPosition = 0;
            m_writePosition = tmp;
        }

        return true;
    }

    uint32 getBuffMaxSize()
    {
        return m_bufferMaxSize;
    }
    uint32 getReadPosition()
    {
        return m_readPosition;
    }
    uint32 getWritePosition()
    {
        return m_writePosition;
    }

protected:
private:
    //缓冲区当前大小
    uint32 m_bufferMaxSize;
    uint32 m_readPosition;
    uint32 m_writePosition;
    BUFFTYPE m_buffer;
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_BUFFER_H__
