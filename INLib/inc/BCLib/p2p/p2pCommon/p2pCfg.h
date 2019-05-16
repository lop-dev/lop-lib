//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/p2p/p2pCommon/p2pCfg.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_P2P_P2PCOMMON_P2PCFG_H__
#define __BCLIB_P2P_P2PCOMMON_P2PCFG_H__

#include <BCLib/p2p/p2pCommon/baseDef.h>


namespace BCLib
{
namespace P2p
{
class BCLIB_P2P_API CP2pCfg
{
public:
    static void setP2pVersion(BCLib::uint64 version);
    static BCLib::uint64 getP2pVersion();
    static void setP2pStepKey0(BCLib::uint32 key);
    static BCLib::uint32 getP2pStepKey0();
    static void setP2pStepKey1(BCLib::uint32 key);
    static BCLib::uint32 getP2pStepKey1();
    static void setP2pStepKey2(BCLib::uint32 key);
    static BCLib::uint32 getP2pStepKey2();

private:
    static BCLib::uint64 ms_p2pVersion;
    static BCLib::uint32 ms_p2pStepKey0;
    static BCLib::uint32 ms_p2pStepKey1;
    static BCLib::uint32 ms_p2pStepKey2;

public:
    class Clt
    {
    public:
        static void setSendWindowSize(BCLib::uint32 size);
        static BCLib::uint32 getSendWindowSize();

        static void setRecvWindowSize(BCLib::uint32 size);
        static BCLib::uint32 getRecvWindowSize();

        static void setSockIOWaitTimeout(BCLib::uint32 msTime);
        static BCLib::uint32 getSockIOWaitTimeout();

        static void setHandShakeTimeout(BCLib::uint32 msTime);
        static BCLib::uint32 getHandShakeTimeout();

        static void setPingInterval(BCLib::uint32 msTime);
        static BCLib::uint32 getPingInterval();

        static void setPingPrecision(BCLib::uint32 msTime);
        static BCLib::uint32 getPingPrecision();

        static void setConnectionTimeout(BCLib::uint32 msTime);
        static BCLib::uint32 getConnectionTimeout();

        static void setPacketStaleTime(BCLib::uint32 msTime);
        static BCLib::uint32 getPacketStaleTime();

        static void setConnectPeerTimeout(BCLib::uint32 msTime);
        static BCLib::uint32 getConnectPeerTimeout();

        static void setGetP2pAddrTimeout(BCLib::uint32 msTime);
        static BCLib::uint32 getGetP2pAddrTimeout();

    private:
        static BCLib::uint32 ms_sendWindowSize;     //! byte
        static BCLib::uint32 ms_recvWindowSize;     //! byte
        static BCLib::uint32 ms_sockIOWaitTimeout;  //! millisecond
        static BCLib::uint32 ms_handShakeTimeout;   //! millisecond
        static BCLib::uint32 ms_pingInterval;       //! millisecond
        static BCLib::uint32 ms_pingPrecision;      //! millisecond
        static BCLib::uint32 ms_connectionTimeout;  //! millisecond
        static BCLib::uint32 ms_packetStaleTime;    //! millisecond
        static BCLib::uint32 ms_connectPeerTimeout; //! millisecond
        static BCLib::uint32 ms_getP2pAddrTimeout;  //! millisecond
    };

    class Srv
    {
    public:
        static void setSendWindowSize(BCLib::uint32 size);
        static BCLib::uint32 getSendWindowSize();

        static void setRecvWindowSize(BCLib::uint32 size);
        static BCLib::uint32 getRecvWindowSize();

        static void setSockIOWaitTimeout(BCLib::uint32 msTime);
        static BCLib::uint32 getSockIOWaitTimeout();

        static void setHandShakeTimeout(BCLib::uint32 msTime);
        static BCLib::uint32 getHandShakeTimeout();

        static void setVerifyTimeout(BCLib::uint32 msTime);
        static BCLib::uint32 getVerifyTimeout();

        static void setPingInterval(BCLib::uint32 msTime);
        static BCLib::uint32 getPingInterval();

        static void setPingPrecision(BCLib::uint32 msTime);
        static BCLib::uint32 getPingPrecision();

        static void setConnectionTimeout(BCLib::uint32 msTime);
        static BCLib::uint32 getConnectionTimeout();

        static void setPacketStaleTime(BCLib::uint32 msTime);
        static BCLib::uint32 getPacketStaleTime();

    private:
        static BCLib::uint32 ms_sendWindowSize;     //! byte
        static BCLib::uint32 ms_recvWindowSize;     //! byte
        static BCLib::uint32 ms_sockIOWaitTimeout;  //! millisecond
        static BCLib::uint32 ms_handShakeTimeout;   //! millisecond
        static BCLib::uint32 ms_verifyTimeout;      //! millisecond
        static BCLib::uint32 ms_pingInterval;       //! millisecond
        static BCLib::uint32 ms_pingPrecision;      //! millisecond
        static BCLib::uint32 ms_connectionTimeout;  //! millisecond
        static BCLib::uint32 ms_packetStaleTime;    //! millisecond
    };
};
}//P2p
}//BCLib

#endif//__BCLIB_P2P_P2PCOMMON_P2PCFG_H__
