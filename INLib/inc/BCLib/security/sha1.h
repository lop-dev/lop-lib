//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/security/sha1.h
//  author:     League of Perfect
/// @brief      SHA1 HASH Ëã·¨
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SECURITY_SHA1_H__
#define __BCLIB_SECURITY_SHA1_H__

#include <BCLib/security/security.h>

namespace BCLib
{
namespace Security
{
typedef union
{
    uint8 c[64];
    uint32 l[16];
} SHA1_WORKSPACE_BLOCK;

class BCLIB_SECURITY_API CSha1 : public CSecurity
{
public:
    CSha1();
    ESecurityError init();
    ESecurityError setKey(const char* key, uint32 len);
    ESecurityError encode(const char* src, uint32 srclen, char* dst, uint32& dstlen);
    ESecurityError decode(const char* src, uint32 srclen, char* dst, uint32& dstlen);

    ~CSha1()
    {
    }
private:
    void Reset();

    // Hash in binary data and strings
    void Update(const uint8* pbData, uint32 uLen);

    // Finalize hash, call before using ReportHash(Stl)
    void Final();

    // Get the raw message digest (20 bytes)
    bool GetHash(uint8* pbDest20) const;

    // Private SHA-1 transformation
    void Transform(uint32* pState, const uint8* pBuffer);

    // Member variables
    uint32 m_state[5];
    uint32 m_count[2];
    uint32 m_reserved0[1]; // Memory alignment padding
    uint8 m_buffer[64];
    uint8 m_digest[20];
    uint32 m_reserved1[3]; // Memory alignment padding

    uint8 m_workspace[64];
    SHA1_WORKSPACE_BLOCK* m_block; // SHA1 pointer to the byte array above
};
}//Security
}//BCLib

#endif//__BCLIB_SECURITY_SHA1_H__
