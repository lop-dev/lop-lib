//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/security/base64.h
//  author:     League of Perfect
/// @brief      Base64Ëã·¨
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SECURITY_BASE64_H__
#define __BCLIB_SECURITY_BASE64_H__

#include <BCLib/utility/baseDef.h>
#include <BCLib/security/security.h>

namespace BCLib
{
namespace Security
{
class BCLIB_SECURITY_API CBase64
{
protected:
    CBase64() {};
    ~CBase64(){};

public:
    static std::string Encode(unsigned char const* src, unsigned int srclen);
	static std::string Encode(std::string const& src);
    static std::string Decode(std::string const& src);
};
}//Security
}//BCLib
#endif//__BCLIB_SECURITY_BASE64_H__
