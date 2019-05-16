//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/security/security.h
//  author:     League of Perfect
/// @brief      加密/解密/哈希
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SECURITY_SECURITY_H__
#define __BCLIB_SECURITY_SECURITY_H__

#include <BCLib/security/baseDef.h>

namespace BCLib
{
namespace Security
{
enum ESecurityError
{
    ESE_OK,
    ESE_NOT_INIT,               //未初始化，或者需要密钥的算法未设置密钥
    ESE_INVALID_KEY,            //秘钥错误
    ESE_INVALID_KEYLEN,         //秘钥长度错误
    ESE_INVALID_INPUT,          //非法输入，例如空指针或者需要加密的长度为0
    ESE_INVALID_OUTPUT,         //非法输出
    ESE_NOT_ENOUGH_BUFFER,      //输出缓存太小
    ESE_CANNOT_DECODE,          //非对称算法，无法解密
    ESE_CANNOT_HASH,            //不支持的哈希算法
    ESE_CANNOT_OPEN_FILE,       //无法打开输入文件
    ESE_UNKNOWN_FILE_ERROR,     //未知文件操作失败
};

//////////////////////////////////////////////////////////////////////////
/// @brief 统一接口
//////////////////////////////////////////////////////////////////////////
class BCLIB_SECURITY_API CSecurity
{
public:
    /// @brief 初始化
    /// @return ESecurityError
    virtual ESecurityError init() = 0;

    /// @brief 设置秘钥
    /// @return ESecurityError
    /// @param key 秘钥
    /// @param len 秘钥长度
    virtual ESecurityError setKey(const char* key, uint32 len) = 0;

    /// @brief 加密/哈希
    /// @return ESecurityError
    /// @param src 需要加密的内容
    /// @param srclen 需要加密的长度
    /// @param dst 加密后的内容
    /// @param dstlen 加密后的长度
    virtual ESecurityError encode(const char* src, uint32 srclen, char* dst, uint32& dstlen) = 0;

    /// @brief 解密
    /// @return ESecurityError
    /// @param src 需要解密的内容
    /// @param srclen 需要解密的长度
    /// @param dst 解密后的内容
    /// @param dstlen 解密后的长度
    virtual ESecurityError decode(const char* src, uint32 srclen, char* dst, uint32& dstlen) = 0;

    virtual ~CSecurity()
    {
    }
};

enum ESecurityType
{
    EST_MD5,
    EST_SHA1,
    EST_RC4,
    EST_AES,
    EST_TOTAL
};

/// @brief 创建CSecurity对象
/// @return CSecurity*
/// @param stype CSecurity对象类型
BCLIB_SECURITY_API CSecurity* createSecurity(ESecurityType stype);

/// @brief 销毁CSecurity对象
/// @return void
/// @param stype CSecurity*
BCLIB_SECURITY_API void destroySecurity(CSecurity* pSecurity);

/// @brief 从文件哈希，并将结果保存到内存
/// @return ESecurityError
/// @param stype 哈希算法
/// @param path 需要哈希的文件路径
/// @param dst 保存结果的内存
/// @param dstlen 保存结果的内存的长度
BCLIB_SECURITY_API ESecurityError hashFile(ESecurityType stype, const char* path, char* dst, uint32& dstlen);
}//Security
}//BCLib

#endif//__BCLIB_SECURITY_SECURITY_H__
