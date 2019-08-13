//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/gameClient/netClient.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_GAMECLIENT_NETCLIENT_H__
#define __SFLIB_GAMECLIENT_NETCLIENT_H__

#include <SFLib/message/gameFrame.h>
#include <BCLib/utility/thread/thread.h>
#include <SFLib/gameClient/netMsgQueue.h>

namespace SFLib
{
namespace Client
{

class CP2pClient;
class CClientMsgExecMgr;
class CTcpClient;

class CNetClient
{
public:
    CNetClient(bool bUseSelfMsgQueue = false);
    virtual ~CNetClient();

    virtual std::string getGameVersion() = 0;

    PeerID getPeerID()
    {
        return m_selfPeerID;
    }
    bool connectLG(const char* ip, BCLib::uint16 port);
    bool connectGW(const char* ip, BCLib::uint16 port);

    //void reconnectLG(BCLib::uint16 uCount = 3);
    //void reconnectGW(BCLib::uint16 uCount = 3);

    bool isReconnectLG() { return m_bReconnectLG; }
    bool isReconnectGW() { return m_bReconnectGW; }

    bool sendToLG(const SFLib::Message::CNetMessage* msg);
    bool sendToLG(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    bool sendToGW(const SFLib::Message::CNetMessage* msg);
    bool sendToGW(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    void closeLG();
    void closeGW();

    void resetLG();
    void resetGW();

    void final();

    //
    bool isConnectionValidLG() const;
    bool isConnectionValidGW() const;

    std::string getMAC() const;

    void setUsrKey(BCLib::uint64 usrKey);
    BCLib::uint64 getUsrKey() const;

    void setRandKey(BCLib::uint32 val) { m_uRandKey = val; }
    BCLib::uint32 getRandKey() const { return m_uRandKey; }

    bool setSendDelay(EServerType eServerType, BCLib::uint32 delay, BCLib::uint32 delayDiff);
    void setRTT(EServerType eServerType, bool bOpen);
    BCLib::uint32 getRTTInMS(EServerType eServerType) const;

    bool useSelfMsgQueue() { return m_bUseSelfMsgQueue; }
    CNetMsgQueueByPeerID& getNetMsgQueue() { return m_netMsgQueue; }

protected:
    CClientMsgExecMgr* _getClientMsgExecMgr(){ return m_msgExecMgr; }

    CTcpClient* _connectLG(const char* ip, BCLib::uint16 port);
    CTcpClient* _connectGW(const char* ip, BCLib::uint16 port);

    //BCLib::ResThread _reconnectLG(void* param);
    //BCLib::ResThread _reconnectGW(void* param);

private:
    std::string m_strIPLG;
    BCLib::uint16 m_uPortLG;
    std::string m_strIPGW;
    BCLib::uint16 m_uPortGW;

    CTcpClient* m_tcpClient2LG;
    CTcpClient* m_tcpClient2GW;

    bool m_bReconnectLG;
    bool m_bReconnectGW;

    BCLib::Utility::CHndThread m_hndReconnectLG;
    BCLib::Utility::CHndThread m_hndReconnectGW;

    PeerID m_selfPeerID;
    CClientMsgExecMgr* m_msgExecMgr;

    bool m_bUseSelfMsgQueue;
    CNetMsgQueueByPeerID m_netMsgQueue;

    BCLib::uint64 m_usrKey;
    BCLib::uint32 m_uRandKey;

    friend class CClientMsgExecMgr;
    friend class CTcpClient;
};
}//Client
}//SFLib

#endif//__SFLIB_GAMECLIENT_NETCLIENT_H__
