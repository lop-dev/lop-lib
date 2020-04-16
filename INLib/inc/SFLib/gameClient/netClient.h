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
    CNetClient(ServerType serverType = SFLib::ESERVER_UNKNOW, bool bUseSelfMsgQueue = false);
    virtual ~CNetClient();

    bool useSelfMsgQueue() { return m_bUseSelfMsgQueue; }
    CNetMsgQueueByPeerID& getNetMsgQueue() { return m_netMsgQueue; }

    virtual std::string getGameVersion() = 0;

    bool connectXS(const char* ip, BCLib::uint16 port);
    bool connectGW(const char* ip, BCLib::uint16 port);

    //void reconnectXS(BCLib::uint16 uCount = 3);
    //void reconnectGW(BCLib::uint16 uCount = 3);

    bool isReconnectXS() { return m_bReconnectXS; }
    bool isReconnectGW() { return m_bReconnectGW; }

    bool sendToXS(const SFLib::Message::CNetMessage* msg);
    bool sendToXS(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    bool sendToGW(const SFLib::Message::CNetMessage* msg);
    bool sendToGW(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    void closeXS();
    void closeGW();

    void resetXS();
    void resetGW();

    void final();

    //
    bool isConnectionValidXS() const;
    bool isConnectionValidGW() const;

    std::string getMAC() const;

public:
    PeerID getPeerID() { return m_selfPeerID; }

    void setUsrKey(BCLib::uint64 usrKey);
    BCLib::uint64 getUsrKey() const;

    void setRandKey(BCLib::uint32 val) { m_uRandKey = val; }
    BCLib::uint32 getRandKey() const { return m_uRandKey; }

    bool setSendDelay(EServerType eServerType, BCLib::uint32 delay, BCLib::uint32 delayDiff);
    void setRTT(EServerType eServerType, bool bOpen);
    BCLib::uint32 getRTTInMS(EServerType eServerType) const;

protected:
    CClientMsgExecMgr* _getClientMsgExecMgr(){ return m_msgExecMgr; }

private:
    CTcpClient* _connectXS(const char* ip, BCLib::uint16 port);
    CTcpClient* _connectGW(const char* ip, BCLib::uint16 port);

    //BCLib::ResThread _reconnectXS(void* param);
    //BCLib::ResThread _reconnectGW(void* param);

private:
    ServerType m_serverType;
    bool m_bUseSelfMsgQueue;
    CNetMsgQueueByPeerID m_netMsgQueue;

    std::string m_strIPXS;
    BCLib::uint16 m_uPortXS;
    std::string m_strIPGW;
    BCLib::uint16 m_uPortGW;

    CTcpClient* m_tcpClient2XS;
    CTcpClient* m_tcpClient2GW;

    bool m_bReconnectXS;
    bool m_bReconnectGW;

    BCLib::Utility::CHndThread m_hndReconnectXS;
    BCLib::Utility::CHndThread m_hndReconnectGW;

    CClientMsgExecMgr* m_msgExecMgr;

    PeerID m_selfPeerID;
    BCLib::uint64 m_usrKey;
    BCLib::uint32 m_uRandKey;

    friend class CClientMsgExecMgr;
    friend class CTcpClient;
};
}//Client
}//SFLib

#endif//__SFLIB_GAMECLIENT_NETCLIENT_H__
