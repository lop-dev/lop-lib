//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/udp/udpServer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_NETWORK_UDP_UDPSERVER_H__
#define __BCLIB_NETWORK_UDP_UDPSERVER_H__

#include <BCLib/network/baseDef.h>
#include <BCLib/network/sockAddr.h>
#include <BCLib/utility/stream.h>
#include <BCLib/utility/spointer.h>


namespace BCLib
{
namespace Network
{
class CUdpProxy;
typedef BCLib::Utility::CSPointer<CUdpProxy> CUdpProxySPtr;
class CUdpServerThread;

class BCLIB_NETWORK_API CUdpServer
{
public:
    CUdpServer();
    virtual ~CUdpServer();

    bool init();
    void final();

	bool addAcceptor(const char* acceptIP, BCLib::uint16 acceptPort, const CUdpProxySPtr& udpProxy, BCLib::uint32 maxUnSendMsgCnt = 10240);

    void setSendDelay(BCLib::uint32 delay, BCLib::uint32 delayDiff);
    void setLostPacketRate(float rate);
    void getSendDelay(BCLib::uint32& delay, BCLib::uint32& delayDiff) const;
    float getLostPacketRate() const;
    BCLib::uint64 getTotalSendByte() const;
    BCLib::uint64 getTotalRecvByte() const;
    float getSendRateInKBPerSec() const;
    float getRecvRateInKBPerSec() const;

private:
    typedef std::vector<CUdpServerThread*> SrvThdVec;
    SrvThdVec m_srvThdVec;

    BCLib::uint32 m_sendDelay;
    BCLib::uint32 m_sendDelayDiff;
    float m_lostPacketRate;
};

class BCLIB_NETWORK_API CUdpProxy : public BCLib::Utility::CRefCnt
{
    friend class CUdpServer;
    friend class CUdpServerThread;
public:
    CUdpProxy();
    virtual ~CUdpProxy();

    BCLib::int32 send(const CSockAddr& peerAddr, BCLib::Utility::CStream& stream);
    BCLib::int32 send(const CSockAddr& peerAddr, const void* msgBuf, BCLib::uint32 bufSize);

protected:
    virtual bool _cbParseMsg(const CSockAddr& peerAddr, const void* msgBuff, BCLib::uint32 msgSize) = 0;

private:
    CUdpServerThread* m_srvThd;
};
typedef BCLib::Utility::CSPointer<CUdpProxy> CUdpProxySPtr;
}//Network
}//BCLib

#endif//__BCLIB_NETWORK_UDP_UDPSERVER_H__
