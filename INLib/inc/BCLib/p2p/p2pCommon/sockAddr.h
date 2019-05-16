//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/p2p/p2pCommon/sockAddr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_P2P_P2PCOMMON_SOCKADDR_H__
#define __BCLIB_P2P_P2PCOMMON_SOCKADDR_H__

#include <BCLib/p2p/p2pCommon/baseDef.h>
#include <string>
#include <vector>


namespace BCLib
{
namespace P2p
{
class BCLIB_P2P_API CSockAddr
{
protected:
    struct sockaddr_in m_addr;
    mutable std::string m_strIP;

public:
    CSockAddr(const char* szIP = NULL, BCLib::uint16 port = INVALID_PORT);
    CSockAddr(const CSockAddr& rhs);

    void setIP();
    void setIP(const char* szIP);
    void setNetIP(BCLib::uint32 netIP);
    const std::string& getIP() const;
    BCLib::uint32 getNetIP() const;

    void setPort(BCLib::uint16 port);
    void setNetPort(BCLib::uint16 netPort);
    BCLib::uint16 getPort() const;
    BCLib::uint16 getNetPort() const;

    void setNetIPPort(BCLib::uint64 netIPPort);
    BCLib::uint64 getNetIPPort() const;

    bool isValid() const;

    CSockAddr& operator =(const CSockAddr& rhs);
    bool operator ==(const CSockAddr& rhs) const;
    bool operator !=(const CSockAddr& rhs) const;
    operator struct sockaddr *();
    operator const struct sockaddr *() const;
    operator struct sockaddr_in *();
    operator const struct sockaddr_in *() const;

public:
    static BCLib::int32 getLocalIP(std::vector<std::string>& ipList);
    static BCLib::int32 getLocalIP(std::vector<BCLib::uint32>& ipList);
    static BCLib::int32 getIPsByDomain(const char* domain, std::vector<std::string>& ipList);
};
}//P2p
}//BCLib

#endif//__BCLIB_P2P_P2PCOMMON_SOCKADDR_H__
