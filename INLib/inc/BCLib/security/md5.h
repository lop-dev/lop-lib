//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/security/md5.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SECURITY_MD5_H__
#define __BCLIB_SECURITY_MD5_H__

#include <BCLib/security/security.h>

namespace BCLib
{
namespace Security
{
class BCLIB_SECURITY_API CMd5 : public CSecurity
{
public:
    virtual ~CMd5()
    {
    }

    ESecurityError init();
    ESecurityError setKey(const char* key, uint32 len);
    ESecurityError encode(const char* src, uint32 srclen, char* dst, uint32& dstlen);
    ESecurityError decode(const char* src, uint32 srclen, char* dst, uint32& dstlen);

    std::string encode(const char* src, uint32 srclen);
    std::string decode(const char* src, uint32 srclen);
    std::string encode(std::string strVal);
    std::string decode(std::string strVal);
    std::string encodeFile(std::string fileName);
    std::string decodeFile(std::string fileName);
};
}//Security
}//BCLib

#endif//__BCLIB_SECURITY_MD5_H__
