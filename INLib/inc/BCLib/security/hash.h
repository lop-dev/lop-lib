//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/security/hash.h
//  author:     League of Perfect
/// @brief      ¹þÏ£Ëã·¨
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SECURITY_HASH_H__
#define __BCLIB_SECURITY_HASH_H__

#include <BCLib/utility/baseDef.h>
#include <BCLib/security/security.h>

namespace BCLib
{
namespace Security
{
class BCLIB_SECURITY_API CHash
{
protected:
    CHash() {};
    ~CHash(){};

public:
	static BCLib::uint32 getBKDRHash(const char  *str);
    static BCLib::uint32 getAPHash(const char  *str);
    static BCLib::uint32 getDJBHash(const char  *str);

    static BCLib::uint32 getJSHash(const char  *str);
    static BCLib::uint32 getRSHash(const char  *str);
    static BCLib::uint32 getSDBMHash(const char  *str);

    static BCLib::uint32 getPJWHash(const char  *str);
    static BCLib::uint32 getELFHash(const char  *str);
};
}//Security
}//BCLib
#endif//__BCLIB_SECURITY_HASH_H__
