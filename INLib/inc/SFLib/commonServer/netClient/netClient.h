//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/netClient/netClient.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_NETCLIENT_NETCLIENT_H__
#define __SFLIB_COMMONSERVER_NETCLIENT_NETCLIENT_H__

#include <BCLib/utility/pointer.h>
#include <BCLib/utility/thread/thread.h>
#include <BCLib/utility/dateTime.h>
#include <BCLib/network/tcp/tcpClient.h>
#include <BCLib/framework/msgExecMgr.h>
#include <SFLib/message/gameFrame.h>
#include <SFLib/commonServer/netPeer.h>
#include <SFLib/commonServer/msgLabel.h>

namespace SFLib
{
namespace CommonServer
{
class CTcpClient : public BCLib::Framework::CMsgExecMgr, public BCLib::Network::CTcpClient
{
public:
    CTcpClient();
    virtual ~CTcpClient();

    /// @brief 参数peerID主要是在消息发送失败的时候传给逻辑层使用
    /// @return 发送字节数或错误值
    int send(PeerID peerID, const SFLib::Message::CNetMessage* msg);
    /// @brief 参数peerID主要是在消息发送失败的时候传给逻辑层使用
    /// @return 发送字节数或错误值
    int send(PeerID peerID, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    void setServerType(EServerType serverType)
    {
        m_serverType = serverType;
    }
    EServerType getServerType()  const
    {
        return m_serverType;
    }

    void setServerID(ServerID serverID)
    {
        m_serverID = serverID;
    }
    ServerID getServerID() const
    {
        return m_serverID;
    }

    /// @brief 网络连接是否已经验证合法性，只有在收到 SMsgMS2XSNtfVerifySuccess 消息后，才会最终验证通过
    /// @return bool
    bool isVerified() const
    {
        return m_verified && m_serverType != ESERVER_UNKNOW && m_serverID != 0;
    }

    std::string getDebugPrompt();

protected:
    void _setVerified(bool verify)
    {
        m_verified = verify;
    }

    virtual bool _cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize);
    virtual void _unhandledMsg(SSFMsgLabel& msgLabel, SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);
    virtual bool _transformMsg(PeerID peerID, SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);

    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::uint16 id, BCLib::Framework::CMsgExecPtr& msgExecPtr);
    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::Framework::CMsgExecPtr& msgExecPtr);
    virtual void _onXX2XSNtfServerType(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

private:
    EServerType m_serverType;
    ServerID m_serverID;
    bool m_verified;
};
typedef BCLib::Utility::CSPointer<CTcpClient> CTcpClientPtr;

//////////////////////////////////////////////////////////////////////////

class CCommonClient : public CTcpClient
{
public:
    CCommonClient();
    virtual ~CCommonClient();

    virtual bool start();
    virtual void terminate();
    virtual bool isRunning();

	//! need reconnect when connection failed
    bool getNeedReconnect()
    {
        return m_needReconnect;
    }
    void setNeedReconnect(bool needReconnect)
    {
        m_needReconnect = needReconnect;
    }

    void setServerIP(const std::string& outerip, const std::string& innerip)
    {
        m_serverOuterIP = outerip; m_serverInnerIP = innerip;
    }
    std::string& getServerIP()
    {
        return m_serverIP;
    }

    void setServerPort(const BCLib::uint16 port)
    {
        m_serverPort = port;
    }
    BCLib::uint16 getServerPort()
    {
        return m_serverPort;
    }

protected:
    bool _connectToSrv();

    virtual bool _enterPoll();
    virtual void _leavePoll();
    virtual void _cbTerminate(const BCLib::Network::CTcpConnectionSPtr& connPtr);

private:
    bool m_needReconnect;
    std::string m_serverOuterIP;
    std::string m_serverInnerIP;
    std::string m_serverIP;
    BCLib::uint16 m_serverPort;
    bool m_bReConnect;
};
typedef BCLib::Utility::CSPointer<CCommonClient> CCommonClientPtr;
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_NETCLIENT_NETCLIENT_H__
