//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/security/rc4.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SECURITY_RC4_H__
#define __BCLIB_SECURITY_RC4_H__

#include <BCLib/security/security.h>

namespace BCLib
{
namespace Security
{
class BCLIB_SECURITY_API CRc4 : public CSecurity
{
public:
    CRc4();
    ESecurityError init();
    ESecurityError setKey(const char* key, uint32 len);
    ESecurityError encode(const char* src, uint32 srclen, char* dst, uint32& dstlen);
    ESecurityError decode(const char* src, uint32 srclen, char* dst, uint32& dstlen);

    virtual ~CRc4()
    {
    }

private:
    static const uint32 STATE_LEN = 256;
    static const uint32 KEY_LEN = 256;

    bool initKey();
    unsigned char m_shIndex1;
    unsigned char m_shIndex2;
    unsigned char m_acState[STATE_LEN];
    char m_szKey[KEY_LEN];
    uint32 m_dwKeyLen;
};
}//Security
}//BCLib

#endif//__BCLIB_SECURITY_RC4_H__
