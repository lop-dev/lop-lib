//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/p2p/p2pServer/p2pAddrServer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_P2P_P2PSERVER_P2PADDRSERVER_H__
#define __BCLIB_P2P_P2PSERVER_P2PADDRSERVER_H__

#include <BCLib/p2p/p2pCommon/baseDef.h>
#include <BCLib/p2p/p2pCommon/sockAddr.h>
#include <BCLib/utility/spointer.h>


namespace BCLib
{
namespace P2p
{
namespace P2pServer
{
class CP2pSrvProxy;
typedef BCLib::Utility::CSPointer<CP2pSrvProxy> CP2pSrvProxySPtr;
class CP2pAddrServerThread;

class BCLIB_P2P_API CP2pAddrServer
{
public:
    CP2pAddrServer();
    virtual ~CP2pAddrServer();

    bool init();
    void final();

    bool addUdpAcceptor(const char* acceptIP, BCLib::uint16 acceptPort, const CP2pSrvProxySPtr& p2pSrvProxy);
    void setAcceptorSendDelay(BCLib::uint32 delay, BCLib::uint32 delayDiff);
    void setAcceptorLostPacketRate(float rate);

private:
    typedef std::vector<CP2pAddrServerThread*> AddrSrvThdVec;
    AddrSrvThdVec m_addrSrvThdVec;
};

class BCLIB_P2P_API CP2pSrvProxy : public BCLib::Utility::CRefCnt
{
    friend class CP2pAddrServer;
    friend class CP2pAddrServerThread;
public:
    CP2pSrvProxy();
    virtual ~CP2pSrvProxy();

    BCLib::int32 send(const CSockAddr& peerAddr, const void* msg, BCLib::uint32 msgSize);

protected:
    virtual void _cbParseMsg(const CSockAddr& peerAddr, const void* msg, BCLib::uint32 msgSize) = 0;

private:
    CP2pAddrServerThread* m_addrSrvThd;
};
typedef BCLib::Utility::CSPointer<CP2pSrvProxy> CP2pSrvProxySPtr;
}//P2pServer
}//P2p
}//BCLib

#endif//__BCLIB_P2P_P2PSERVER_P2PADDRSERVER_H__
