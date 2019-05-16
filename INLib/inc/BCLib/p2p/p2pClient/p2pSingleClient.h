//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/p2p/p2pClient/p2pSingleClient.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_P2P_P2PCLIENT_P2PSINGLECLIENT_H__
#define __BCLIB_P2P_P2PCLIENT_P2PSINGLECLIENT_H__

#include <BCLib/p2p/p2pCommon/baseDef.h>
#include <BCLib/p2p/p2pCommon/sockAddr.h>


namespace BCLib
{
namespace P2p
{
class CEncrypt;
class CCompress;

namespace P2pClient
{
class CP2pSingleClientThread;

class BCLIB_P2P_API CP2pSingleClient
{
    friend class CP2pSingleClientThread;
public:
    CP2pSingleClient();
    virtual ~CP2pSingleClient();

    bool connectAddrServerGetP2pAddr(const char* srvName, BCLib::uint16 srvPort, BCLib::uint32 connMax = 0);
    bool start(BCLib::P2p::PeerID peerID, CEncrypt* encrypt = NULL, CCompress* compress = NULL);
    void stop();

    BCLib::P2p::PeerID getP2pPeerID() const;
    bool getP2pLocalAddrs(std::vector<CSockAddr>& localAddrs);
    bool getP2pNetAddr(CSockAddr& netAddr);

    bool sendMsgToServer(const void* msg, BCLib::uint32 msgSize);

    bool connectPeer(BCLib::P2p::PeerID peerID, const std::vector<CSockAddr>& localAddrs, const CSockAddr& netAddr);
    void closePeer(BCLib::P2p::PeerID peerID);
    bool sendMsgToPeer(BCLib::P2p::PeerID peerID, const void* msg, BCLib::uint32 msgSize);
    bool broadcastMsg(const void* msg, BCLib::uint32 msgSize);
    BCLib::uint32 getConnectPeers(std::vector<BCLib::P2p::PeerID>& peerList);

    void setSystemSendDelay(BCLib::uint32 delay, BCLib::uint32 delayDiff);
    void setSystemLostPacketRate(float rate);

    void setSendDelay(BCLib::P2p::PeerID peerID, BCLib::uint32 delay, BCLib::uint32 delayDiff);
    void setLostPacketRate(BCLib::P2p::PeerID peerID, float rate);
    void setRTT(BCLib::P2p::PeerID peerID, bool bOpen);
    bool isOpenRTT(BCLib::P2p::PeerID peerID) const;
    void getSendDelay(BCLib::P2p::PeerID peerID, BCLib::uint32& delay, BCLib::uint32& delayDiff) const;
    float getLostPacketRate(BCLib::P2p::PeerID peerID) const;
    BCLib::uint32 getRTTInMS(BCLib::P2p::PeerID peerID) const;
    BCLib::int64 getConnectTimeInSec(BCLib::P2p::PeerID peerID) const;
    BCLib::uint64 getTotalSendByte(BCLib::P2p::PeerID peerID) const;
    BCLib::uint64 getTotalRecvByte(BCLib::P2p::PeerID peerID) const;
    float getSendRateInKBPerSec(BCLib::P2p::PeerID peerID) const;
    float getRecvRateInKBPerSec(BCLib::P2p::PeerID peerID) const;

protected:
    virtual void _cbConnectPeer(BCLib::P2p::PeerID peerID, bool bSuccess) = 0;
    virtual void _cbParseMsgFromServer(const void* msg, BCLib::uint32 msgSize) = 0;
    virtual void _cbParseMsgFromPeer(BCLib::P2p::PeerID peerID, const void* msg, BCLib::uint32 msgSize) = 0;

private:
    CP2pSingleClientThread* m_cltThd;
};
}//P2pClient
}//P2p
}//BCLib

#endif//__BCLIB_P2P_P2PCLIENT_P2PSINGLECLIENT_H__
