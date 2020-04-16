//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/netClient/tcpClient.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_NETCLIENT_TCPCLIENT_H__
#define __SFLIB_COMMONSERVER_NETCLIENT_TCPCLIENT_H__

#include <BCLib/network/tcp/tcpClient.h>
#include <BCLib/framework/msgExecMgr.h>
#include <SFLib/message/gameFrame.h>
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

    virtual BCLib::int32 send(const SFLib::Message::CNetMessage* msg);
    virtual BCLib::int32 send(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    /// @brief 参数peerID主要是在消息发送失败的时候传给逻辑层使用
    /// @return 发送字节数或错误值
    virtual int send(PeerID peerID, const SFLib::Message::CNetMessage* msg);

    /// @brief 参数peerID主要是在消息发送失败的时候传给逻辑层使用
    /// @return 发送字节数或错误值
    virtual int send(PeerID peerID, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

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

    /// @brief 网络连接是否已经验证合法性
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
    virtual void _cbTerminate(const BCLib::Network::CTcpConnectionSPtr& connPtr);

    virtual void _unhandledMsg(SSFMsgLabel& msgLabel, SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);
    virtual bool _transformMsg(PeerID peerID, SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);

    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::uint16 id, BCLib::Framework::CMsgExecPtr& msgExecPtr);
    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::Framework::CMsgExecPtr& msgExecPtr);

protected:
    EServerType m_serverType;
    ServerID m_serverID;
    bool m_verified;
};
typedef BCLib::Utility::CSPointer<CTcpClient> CTcpClientPtr;
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_NETCLIENT_TCPCLIENT_H__
