//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/p2p/p2pCommon/encryptAndCompress.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_P2P_P2PCOMMON_ENCRYPTANDCOMPRESS_H__
#define __BCLIB_P2P_P2PCOMMON_ENCRYPTANDCOMPRESS_H__

#include <BCLib/p2p/p2pCommon/baseDef.h>


namespace BCLib
{
namespace P2p
{
class BCLIB_P2P_API CEncrypt
{
public:
    virtual ~CEncrypt()
    {
    }
    //! 多个连接共用一个对象,需要保证线程安全
    virtual BCLib::int32 encrypt(const void* pInData, BCLib::int32 iInSize, void* pOutBuff, BCLib::int32 iBuffSize, bool isEncrypt) = 0;
};

class BCLIB_P2P_API CCompress
{
public:
    virtual ~CCompress()
    {
    }
    //! 多个连接共用一个对象,需要保证线程安全
    virtual BCLib::int32 compress(const void* pInData, BCLib::int32 iInSize, void* pOutBuff, BCLib::int32 iBuffSize, bool isCompress) = 0;
};
}//P2p
}//BCLib

#endif//__BCLIB_P2P_P2PCOMMON_ENCRYPTANDCOMPRESS_H__
