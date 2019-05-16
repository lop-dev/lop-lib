//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/stream.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_STREAM_H__
#define __BCLIB_UTILITY_STREAM_H__

#include <BCLib/utility/string.h>
#include <BCLib/utility/bufferImp.h>

namespace BCLib
{
namespace Utility
{
class BCLIB_UTILITY_API CStream : public CBufferInt8
{
public:
    CStream& read(char* pBuf, uint32 size);
    CStream& write(const char* pBuf, uint32 size);

    template<typename TYPE>
    CStream& operator <<(const TYPE& val)
    {
        return write((char*)&val, sizeof(val));
    }

    /// @brief 把val的内容追加到stream后面，但不改变val的内容，不支持指针
    /// @return CStream&
    /// @param val
    template<typename TYPE>
    CStream& operator <<(const std::vector<TYPE>& val)
    {
        BCLib::uint32 size = val.size();
        operator << (size);
        typename std::vector<TYPE>::const_iterator itrB = val.begin();
        typename std::vector<TYPE>::const_iterator itrE = val.end();
        for(; itrB != itrE; ++itrB)
        {
            operator << (*itrB);
        }
        return *this;
    }

    template<typename TYPE>
    CStream& operator <<(const std::list<TYPE>& val)
    {
        BCLib::uint32 size = val.size();
        operator << (size);
        typename std::vector<TYPE>::const_iterator itrB = val.begin();
        typename std::vector<TYPE>::const_iterator itrE = val.end();
        for(; itrB != itrE; ++itrB)
        {
            operator << (*itrB);
        }
        return *this;
    }

    /// @brief 把val的内容追加到stream后面，但不改变val的内容
    /// @return CStream&
    /// @param val
    CStream& operator <<(const CStream& val);

    CStream& operator <<(const std::string& val)
    {
        return writeString(val);
    }
    CStream& operator <<(const std::wstring& val)
    {
        return writeString(val);
    }
    CStream& operator <<(const CStringA& val)
    {
        return writeString(val);
    }
    CStream& operator <<(const CStringW& val)
    {
        return writeString(val);
    }

    template<typename TYPE>
    CStream& operator >>(TYPE& val)
    {
        return read((char*)&val, sizeof(val));
    }

    /// @brief 把stream的内容读取到val后面，不支持指针
    /// @return CStream&
    /// @param val
    template<typename TYPE>
    CStream& operator >>(std::vector<TYPE>& val)
    {
        BCLib::uint32 size = 0;
        operator >> (size);
        val.reserve(val.size() + size);
        for(BCLib::uint32 i=0; i<size; ++i)
        {
            TYPE node;
            operator >> (node);
            val.push_back(node);
        }
        return *this;
    }

    template<typename TYPE>
    CStream& operator >>(std::list<TYPE>& val)
    {
        BCLib::uint32 size = 0;
        operator >> (size);
        val.reserve(val.size() + size);
        for(BCLib::uint32 i=0; i<size; ++i)
        {
            TYPE node;
            operator >> (node);
            val.push_back(node);
        }
        return *this;
    }

    /// @brief 把stream的内容读取到val后面
    /// @return CStream&
    /// @param val
    CStream& operator >>(std::string& val)
    {
        return readString(val);
    }
    CStream& operator >>(std::wstring& val)
    {
        return readString(val);
    }
    CStream& operator >>(CStringA& val)
    {
        return readString(val);
    }
    CStream& operator >>(CStringW& val)
    {
        return readString(val);
    }

private:
    template<typename T>
    CStream& readString(T& str);

    template<typename T>
    CStream& writeString(const T& str);
};


template<typename T>
CStream& CStream::readString(T& str)
{
    typedef typename T::value_type ValueType;

    unsigned int sizeBytes = 0;
    CBufferInt8::pop((char*)&sizeBytes, sizeof(sizeBytes));
    if(sizeBytes <= 0)
    {
        return *this;
    }
    unsigned int buffSize = CBufferInt8::readSize();
    unsigned int size = sizeBytes / sizeof(ValueType);

    if(sizeBytes <= buffSize)
    {
        ValueType* pData = (ValueType*)CBufferInt8::readPtr();
        str.assign(pData, size);
        CBufferInt8::readFlip(sizeBytes);
    }
    else
    {
        ;
    }
    return *this;
}

template<typename T>
CStream& CStream::writeString(const T& str)
{
    typedef typename T::value_type ValueType;

    unsigned int sizeBytes = (unsigned int)str.size() * sizeof(ValueType);
    CBufferInt8::push((char*)&sizeBytes, sizeof(sizeBytes));
    if(sizeBytes > 0)
    {
        CBufferInt8::push((char*)str.c_str(), sizeBytes);
    }
    return *this;
}
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_STREAM_H__
