//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/p2p/p2pServer/p2pServer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_P2P_P2PSERVER_P2PSERVER_H__
#define __BCLIB_SAMPLE_P2P_P2PSERVER_P2PSERVER_H__

#include <BCLib/utility/uniqueID.h>
#include <BCLib/utility/singleton.h>
#include <BCLib/framework/mainThread.h>
#include <BCLib/p2p/p2pServer/p2pServer.h>

class CP2pServer : public BCLib::Framework::CMainThread, public BCLib::P2p::P2pServer::CP2pServer
{
    BCLIB_SINGLETON_DECLARE(CP2pServer);
public:
    CP2pServer();
    virtual ~CP2pServer();

public:
    static int main();

protected:
    virtual bool _init();
    virtual int _final();

    virtual void _cbParseMsgByPeerID(BCLib::P2p::PeerID peerID, const void* msg, BCLib::uint32 msgSize);
    virtual void _cbParseMsgByConnectID(BCLib::P2p::ConnectID connID, const void* msg, BCLib::uint32 msgSize);
    virtual void _cbRequestConnectPeer(BCLib::P2p::PeerID srcPeerID, BCLib::P2p::PeerID dstPeerID);
    virtual void _cbConnectionTerminate(BCLib::P2p::ConnectID connID, BCLib::P2p::PeerID peerID);

    void _parseSampleMsgConnID(BCLib::P2p::ConnectID connID, BCLib::Framework::SMessage* msg, BCLib::uint32 msgSize);
    void _parseSampleMsgPeerID(BCLib::P2p::PeerID peerID, BCLib::Framework::SMessage* msg, BCLib::uint32 msgSize);

private:
    BCLib::Utility::CUniqueID32 m_allocPeerID;
};

#endif//__BCLIB_SAMPLE_P2P_P2PSERVER_P2PSERVER_H__
