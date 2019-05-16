//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/utility.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_NETWORK_UTILITY_H__
#define __BCLIB_NETWORK_UTILITY_H__

#include <BCLib/network/baseDef.h>
#include <BCLib/network/sockAddr.h>


namespace BCLib
{
namespace Network
{
class BCLIB_NETWORK_API CIcmp
{
public:
    CIcmp();
    ~CIcmp();

    bool init();
    void final();

    struct SPingRes
    {
        enum EPingErr
        {
            EPE_OK = 0,
            EPE_UNINIT,
            EPE_RESOLVE_HOST_FAIL,
            EPE_SET_TTL_FAIL,
            EPE_ALLOC_MEMORY_FAIL,
            EPE_SEND_FAIL,
            EPE_TIMEOUT,
            EPE_RECV_FAIL,
            EPE_HOST_UNREACH,
            EPE_TTL_EXPIRE,
        };
        CSockAddr m_dstAddr;
        BCLib::uint32 m_rtt;
        BCLib::uint8 m_hops;
        EPingErr m_eErr;
        void clear();
    };
    const SPingRes& ping(const char* hostName, BCLib::uint32 dataLen=32, BCLib::uint8 ttl=30, BCLib::uint32 timeoutInMS=-1);

protected:
    BCLib::uint32 _getProcID();
    void _initPingPk(char* ping, BCLib::uint32 pingSize, BCLib::uint8 seq, BCLib::uint16 id);
    void _decodePingReply(char* pingReply, BCLib::uint32 pingReplySize, BCLib::uint8 seq, BCLib::uint16 id, SPingRes& pingRes);
    BCLib::uint16 _makeIPCheckSum(const void* data, BCLib::uint32 dataLen);

protected:
    SOCK m_sock;
    SPingRes m_pingRes;
};
}//Network
}//BCLib

#endif//__BCLIB_NETWORK_UTILITY_H__
