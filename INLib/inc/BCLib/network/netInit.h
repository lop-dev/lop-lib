//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/netInit.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_NETWORK_NETINIT_H__
#define __BCLIB_NETWORK_NETINIT_H__

#include <BCLib/network/baseDef.h>


namespace BCLib
{
namespace Network
{
class CNetInit
{
public:
    class NetDelay
    {
    public:
        static bool setTimerDuration(BCLib::uint32 msTime);
        static BCLib::uint32 getTimerDuration();
        static bool setTimerPrecision(BCLib::uint32 msTime);
        static BCLib::uint32 getTimerPrecision();

    protected:
        static BCLib::uint32 ms_timerDuration;              //! millisecond
        static BCLib::uint32 ms_timerPrecision;             //! millisecond
    };

    class Tcp
    {
    public:
        class Accept
        {
        public:
            static bool setSendWindowSize(BCLib::uint32 size);
            static BCLib::uint32 getSendWindowSize();
            static bool setRecvWindowSize(BCLib::uint32 size);
            static BCLib::uint32 getRecvWindowSize();
            static bool setEpollWaitTimeout(BCLib::uint32 msTime);
            static BCLib::uint32 getEpollWaitTimeout();

        protected:
            static BCLib::uint32 ms_sendWindowSize;         //! byte
            static BCLib::uint32 ms_recvWindowSize;         //! byte
            static BCLib::uint32 ms_epollWaitTimeout;       //! millisecond
        };

        class Connect
        {
        public:
            static bool setSendWindowSize(BCLib::uint32 size);
            static BCLib::uint32 getSendWindowSize();
            static bool setRecvWindowSize(BCLib::uint32 size);
            static BCLib::uint32 getRecvWindowSize();
            static bool setEpollWaitTimeout(BCLib::uint32 msTime);
            static BCLib::uint32 getEpollWaitTimeout();
            static bool setPingDuration(BCLib::uint32 msTime);
            static BCLib::uint32 getPingDuration();
            static bool setPingPrecision(BCLib::uint32 msTime);
            static BCLib::uint32 getPingPrecision();
            static bool setTimeout(BCLib::uint32 msTime);
            static BCLib::uint32 getTimeout();

        protected:
            static BCLib::uint32 ms_sendWindowSize;         //! byte
            static BCLib::uint32 ms_recvWindowSize;         //! byte
            static BCLib::uint32 ms_epollWaitTimeout;       //! millisecond
            static BCLib::uint32 ms_pingDuration;           //! millisecond
            static BCLib::uint32 ms_pingPrecision;          //! millisecond
            static BCLib::uint32 ms_timeout;                //! millisecond
        };
    };

    class Stub
    {
    public:
        static bool setVerifyTimeout(BCLib::uint32 msTime);
        static BCLib::uint32 getVerifyTimeout();

    protected:
        static BCLib::uint32 ms_verifyTimeout;              //! millisecond
    };

    class Udp
    {
    public:
        class Client
        {
        public:
            static bool setSendWindowSize(BCLib::uint32 size);
            static BCLib::uint32 getSendWindowSize();
            static bool setRecvWindowSize(BCLib::uint32 size);
            static BCLib::uint32 getRecvWindowSize();
            static bool setEpollWaitTimeout(BCLib::uint32 msTime);
            static BCLib::uint32 getEpollWaitTimeout();

        protected:
            static BCLib::uint32 ms_sendWindowSize;         //! byte
            static BCLib::uint32 ms_recvWindowSize;         //! byte
            static BCLib::uint32 ms_epollWaitTimeout;       //! millisecond
        };

        class Server
        {
        public:
            static bool setSendWindowSize(BCLib::uint32 size);
            static BCLib::uint32 getSendWindowSize();
            static bool setRecvWindowSize(BCLib::uint32 size);
            static BCLib::uint32 getRecvWindowSize();
            static bool setEpollWaitTimeout(BCLib::uint32 msTime);
            static BCLib::uint32 getEpollWaitTimeout();

        protected:
            static BCLib::uint32 ms_sendWindowSize;         //! byte
            static BCLib::uint32 ms_recvWindowSize;         //! byte
            static BCLib::uint32 ms_epollWaitTimeout;       //! millisecond
        };
    };

    class Pipe
    {
    public:
        static bool setConnectTimeout(BCLib::uint32 msTime);
        static BCLib::uint32 getConnectTimeout();
        static bool setEpollWaitTimeout(BCLib::uint32 msTime);
        static BCLib::uint32 getEpollWaitTimeout();

    protected:
        static BCLib::uint32 ms_connectTimeout;             //! millisecond
        static BCLib::uint32 ms_epollWaitTimeout;           //! millisecond
    };

};
}//Network
}//BCLib
#endif//__BCLIB_NETWORK_NETINIT_H__
