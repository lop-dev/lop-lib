//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/udp/udpClient.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_NETWORK_UDP_UDPCLIENT_H__
#define __BCLIB_NETWORK_UDP_UDPCLIENT_H__

#include <BCLib/network/baseDef.h>
#include <BCLib/utility/stream.h>


namespace BCLib
{
namespace Network
{
class CUdpClientThread;

class BCLIB_NETWORK_API CUdpClient
{
    friend class CUdpClientThread;
public:
    CUdpClient();
    virtual ~CUdpClient();

    bool start(const char* srvIP, BCLib::uint16 srvPort, BCLib::uint32 maxUnSendMsgCnt = 10240);
    void close();

    BCLib::int32 send(BCLib::Utility::CStream& stream);
    BCLib::int32 send(const void* msgBuf, BCLib::uint32 bufSize);

    void setSendDelay(BCLib::uint32 delay, BCLib::uint32 delayDiff);
    void setLostPacketRate(float rate);
    void getSendDelay(BCLib::uint32& delay, BCLib::uint32& delayDiff) const;
    float getLostPacketRate() const;
    BCLib::uint64 getTotalSendByte() const;
    BCLib::uint64 getTotalRecvByte() const;
    float getSendRateInKBPerSec() const;
    float getRecvRateInKBPerSec() const;

protected:
    virtual bool _cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize) = 0;

private:
    CUdpClientThread* m_cltThd;
};
}//Network
}//BCLib

#endif//__BCLIB_NETWORK_UDP_UDPCLIENT_H__
