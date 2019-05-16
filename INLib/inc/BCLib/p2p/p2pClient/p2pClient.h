//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/p2p/p2pClient/p2pClient.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_P2P_P2PCLIENT_P2PCLIENT_H__
#define __BCLIB_P2P_P2PCLIENT_P2PCLIENT_H__

#include <BCLib/p2p/p2pCommon/baseDef.h>


namespace BCLib
{
namespace P2p
{
class CEncrypt;
class CCompress;

namespace P2pClient
{
class CP2pClientThread;

class BCLIB_P2P_API CP2pClient
{
    friend class CP2pClientThread;
public:
    CP2pClient();
    virtual ~CP2pClient();

    bool init(CEncrypt* encrypt, CCompress* compress);

    bool connectServer(const char* srvName, BCLib::uint16 srvPort, BCLib::uint32 connMax = 0);
    bool connectPeer(BCLib::P2p::PeerID peerID);

    void closeServer();
    void closePeer(BCLib::P2p::PeerID peerID);

    bool sendMsgToServer(const void* msg, BCLib::uint32 msgSize);
    bool sendMsgToPeer(BCLib::P2p::PeerID peerID, const void* msg, BCLib::uint32 msgSize);

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

    BCLib::P2p::PeerID getP2pPeerID();

protected:
    virtual void _cbParseMsgFromServer(const void* msg, BCLib::uint32 msgSize) = 0;
    virtual void _cbParseMsgFromPeer(BCLib::P2p::PeerID peerID, const void* msg, BCLib::uint32 msgSize) = 0;

private:
    CEncrypt* m_encrypt;
    CCompress* m_compress;
    CP2pClientThread* m_cltThd;
};
}//P2pClient
}//P2p
}//BCLib

#endif//__BCLIB_P2P_P2PCLIENT_P2PCLIENT_H__
