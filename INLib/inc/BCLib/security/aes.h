//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/security/aes.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SECURITY_AES_H__
#define __BCLIB_SECURITY_AES_H__

#include <BCLib/security/security.h>

namespace BCLib
{
namespace Security
{
class BCLIB_SECURITY_API CAes : public CSecurity
{
public:
    CAes();
    ESecurityError init();
    ESecurityError setKey(const char* key, uint32 len);
    ESecurityError encode(const char* src, uint32 srclen, char* dst, uint32& dstlen);
    ESecurityError decode(const char* src, uint32 srclen, char* dst, uint32& dstlen);

    virtual ~CAes()
    {
    }

private:
    bool keyset;
    unsigned char Sbox[256];
    unsigned char InvSbox[256];
    unsigned char w[11][4][4];

    unsigned char* Cipher(unsigned char* input);
    unsigned char* InvCipher(unsigned char* input);

    void KeyExpansion(const unsigned char* key, unsigned char w[][4][4]);
    unsigned char FFmul(unsigned char a, unsigned char b);

    void SubBytes(unsigned char state[][4]);
    void ShiftRows(unsigned char state[][4]);
    void MixColumns(unsigned char state[][4]);
    void AddRoundKey(unsigned char state[][4], unsigned char k[][4]);

    void InvSubBytes(unsigned char state[][4]);
    void InvShiftRows(unsigned char state[][4]);
    void InvMixColumns(unsigned char state[][4]);
};
}//Security
}//BCLib

#endif//__BCLIB_SECURITY_AES_H__
