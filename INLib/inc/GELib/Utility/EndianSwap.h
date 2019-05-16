//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/EndianSwap.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_ENDIANSWAP_H__
#define __GELIB_UTILITY_ENDIANSWAP_H__

namespace GELib
{

#if defined(_XBOX) || defined(_PS3)
    #define NATIVE_BIG_ENDIAN
#else
    #define NATIVE_LITTLE_ENDIAN
#endif

inline bool IsNativeLittleEndian()
{
    int i = 1;
    return *((char*)&i) == 1;
}

inline bool IsNativeBigEndian()
{
    return !IsNativeLittleEndian();
}

inline void SwapEndian(unsigned char &b0, unsigned char &b1)
{
    unsigned char tmp = b0;
    b0 = b1;
    b1 = tmp;
};

inline void SwapBytes2(void *value)
{
    unsigned char *b = (unsigned char*)value;
    SwapEndian(b[0], b[1]);
}

inline void SwapBytes4(void *value)
{
    unsigned char *b = (unsigned char*)value;
    SwapEndian(b[0], b[3]);
    SwapEndian(b[1], b[2]);
}

inline void SwapBytes8(void *value)
{
    unsigned char *b = (unsigned char*)value;
    SwapEndian(b[0], b[7]);
    SwapEndian(b[1], b[6]);
    SwapEndian(b[2], b[5]);
    SwapEndian(b[3], b[4]);
}

inline void SwapEndian(short &value)
{
    SwapBytes2(&value);
}

inline void SwapEndian(unsigned short &value)
{
    SwapBytes2(&value);
}

inline void SwapEndian(long &value)
{
    SwapBytes4(&value);
}

inline void SwapEndian(unsigned long &value)
{
    SwapBytes4(&value);
}

inline void SwapEndian(int &value)
{
    SwapBytes4(&value);
}

inline void SwapEndian(unsigned int &value)
{
    SwapBytes4(&value);
}

inline void SwapEndian(float &value)
{
    SwapBytes4(&value);
}

inline void SwapEndian(double &value)
{
    SwapBytes8(&value);
}

inline void SwapEndian(long long &value)
{
    SwapBytes8(&value);
}

#ifdef NATIVE_BIG_ENDIAN
    template <typename T> void LittleToNativeEndian(T &a) { SwapEndian(a); }
    template <typename T> void BigToNativeEndian(T &a) {}
    template <typename T> void NativeToLittleEndian(T &a) { SwapEndian(a); }
    template <typename T> void NativeToBigEndian(T &a) {}
#else
    template <typename T> void LittleToNativeEndian(T &a) {}
    template <typename T> void BigToNativeEndian(T &a) { SwapEndian(a); }
    template <typename T> void NativeToLittleEndian(T &a) {}
    template <typename T> void NativeToBigEndian(T &a) { SwapEndian(a); }
#endif


}//GELib

#endif//__GELIB_UTILITY_ENDIANSWAP_H__
