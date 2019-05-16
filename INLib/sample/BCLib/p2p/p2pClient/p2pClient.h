//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/p2p/p2pClient/p2pClient.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_P2P_P2PCLIENT_P2PCLIENT_H__
#define __BCLIB_SAMPLE_P2P_P2PCLIENT_P2PCLIENT_H__


#include <BCLib/utility/singleton.h>
#include <BCLib/utility/thread/thread.h>
#include <BCLib/framework/mainThread.h>
#include <BCLib/p2p/p2pClient/p2pClient.h>
#include <BCLib/utility/dateTime.h>
#include "p2pCommand.h"

class CP2pClient : public BCLib::Framework::CMainThread, public BCLib::P2p::P2pClient::CP2pClient
{
    BCLIB_SINGLETON_DECLARE(CP2pClient);
public:
    CP2pClient();
    virtual ~CP2pClient();

    BCLib::ResThread inputCommand(void* param);
    BCLib::int64 getNowTime();

public:
    static int main();

protected:
    virtual bool _init();
    virtual int _final();

    virtual void _cbParseMsgFromServer(const void* msg, BCLib::uint32 msgSize);
    virtual void _cbParseMsgFromPeer(BCLib::P2p::PeerID peerID, const void* msg, BCLib::uint32 msgSize);

    void _parseSampleMsgFromServer(BCLib::Framework::SMessage* msg, BCLib::uint32 msgSize);
    void _parseSampleMsgFromPeer(BCLib::P2p::PeerID peerID, BCLib::Framework::SMessage* msg, BCLib::uint32 msgSize);

private:
    CP2pCommand* m_p2pClientCommand;
    BCLib::Utility::CHndThread m_inputThread;
    BCLib::P2p::PeerID m_peerID;
    BCLib::Utility::CRealTime m_realTime;
};


#endif//__BCLIB_SAMPLE_P2P_P2PCLIENT_P2PCLIENT_H__
