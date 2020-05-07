//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/p2p/p2pServer/p2pServer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_P2P_P2PSERVER_P2PSERVER_H__
#define __BCLIB_P2P_P2PSERVER_P2PSERVER_H__

#include <BCLib/p2p/p2pCommon/baseDef.h>
#include <vector>
#include <BCLib/utility/hashMap.h>
#include <BCLib/utility/thread/mutex.h>
#include <BCLib/utility/uniqueID.h>
#include <BCLib/utility/spointer.h>


namespace BCLib
{
namespace P2p
{
class CEncrypt;
class CCompress;

namespace P2pServer
{
class CP2pServerThread;
class CP2pConnection;
typedef BCLib::Utility::CSPointer<CP2pConnection> CP2pConnectionSPtr;

class BCLIB_P2P_API CP2pServer
{
    friend class CP2pServerThread;
public:
    CP2pServer();
    virtual ~CP2pServer();

    bool init(bool bTransitMsgBySrv, CEncrypt* encrypt, CCompress* compress);
    void final();

    bool addUdpAcceptor(const char* acceptIP, BCLib::uint16 acceptPort);
    void setAcceptorSendDelay(BCLib::uint32 delay, BCLib::uint32 delayDiff);
    void setAcceptorLostPacketRate(float rate);

    bool sendMsgByPeerID(BCLib::P2p::PeerID peerID, const void* msg, BCLib::uint32 msgSize);
    bool sendMsgByConnectID(BCLib::P2p::UdpConnectID connID, const void* msg, BCLib::uint32 msgSize);

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

    bool setRegisterPeer(BCLib::P2p::UdpConnectID connID, BCLib::P2p::PeerID peerID);
    bool allowConnectPeer(BCLib::P2p::PeerID srcPeerID, BCLib::P2p::PeerID dstPeerID);

    bool closePeerID(BCLib::P2p::PeerID peerID);

protected:
    virtual void _cbParseMsgByPeerID(BCLib::P2p::PeerID peerID, const void* msg, BCLib::uint32 msgSize) = 0;
    virtual void _cbParseMsgByConnectID(BCLib::P2p::UdpConnectID connID, const void* msg, BCLib::uint32 msgSize) = 0;
    virtual void _cbRequestConnectPeer(BCLib::P2p::PeerID srcPeerID, BCLib::P2p::PeerID dstPeerID) = 0;
    virtual void _cbConnectionTerminate(BCLib::P2p::UdpConnectID connID, BCLib::P2p::PeerID peerID) = 0;

protected:
    BCLib::Utility::CUniqueID64& _getConnIDMaker();
    bool _isTransitMsgBySrv() const;
    CP2pConnectionSPtr _getConnByPeerID(PeerID peerID) const;
    CP2pConnectionSPtr _getConnByConnID(UdpConnectID connID) const;
    CP2pConnectionSPtr _getConnByAddr(BCLib::uint64 addr) const;
    bool _addConnection(const CP2pConnectionSPtr& conn);
    void _delConnection(const CP2pConnectionSPtr& conn);
    bool _bindPeerID(UdpConnectID connID, PeerID peerID);

private:
    typedef std::vector<CP2pServerThread*> SrvThdVec;
    typedef BCLib::Utility::CHashMap<PeerID, CP2pConnectionSPtr> ConnMapByPeerID;
    typedef BCLib::Utility::CHashMap<UdpConnectID, CP2pConnectionSPtr> ConnMapByConnID;
    typedef BCLib::Utility::CHashMap<BCLib::uint64, CP2pConnectionSPtr> ConnMapByAddr;

    BCLib::Utility::CUniqueID64 m_connIDMaker;
    bool m_bTransitMsgBySrv;
    CEncrypt* m_encrypt;
    CCompress* m_compress;

    SrvThdVec m_srvThdVec;
    BCLib::Utility::CMutex m_connMapMutex;
    ConnMapByPeerID m_connMapByPeerID;
    ConnMapByConnID m_connMapByConnID;
    ConnMapByAddr m_connMapByAddr;
};
}//P2pServer
}//P2p
}//BCLib

#endif//__BCLIB_P2P_P2PSERVER_P2PSERVER_H__
