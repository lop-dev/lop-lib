//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/commonServer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_COMMONSERVER_H__
#define __SFLIB_COMMONSERVER_COMMONSERVER_H__

#include <BCLib/framework/mainThread.h>
#include <BCLib/network/tcp/tcpServer.h>
#include <SFLib/message/gameFrame.h>
#include <SFLib/commonServer/netClient/netClientMgr.h>
#include <SFLib/commonServer/netPeerMgr.h>
#include <SFLib/commonServer/netClient/externalClient.h>

//#define USE_PIPESERVER_TEST_LOGIC

#include <BCLib/network/pipe/pipeServer.h>
#include <BCLib/network/pipe/pipeStubMgr.h>

namespace SFLib
{
namespace CommonServer
{

#ifndef USE_PIPESERVER_TEST_LOGIC
#else
#endif

#ifndef USE_PIPESERVER_TEST_LOGIC
class CCommonServer : public BCLib::Framework::CMainThread, public BCLib::Network::CTcpServer
#else
class CCommonServer : public BCLib::Framework::CMainThread, public BCLib::Network::CPipeServer
#endif
{
public:
    CCommonServer(const std::string& name);
    virtual ~CCommonServer();

    static CCommonServer* getCommonServer()
    {
        return m_commonServer;
    }

    virtual GroupID getGroupID()
    {
        return INVALID_GROUP_ID;
    }

    void setServerID(ServerID serverID)
    {
        m_serverID = serverID;
    }
    ServerID getServerID()
    {
        return m_serverID;
    }

    const std::string& getServerName()
    {
        return m_serverName;
    }

    virtual EServerType getServerType() = 0;
    virtual std::string getServerVersion() = 0;

public:
    /// @brief 加载配置文件
    /// @return bool
    bool loadConfigFile(std::string configFile = "");

    /// @brief 重新加载配置文件中的一个键值
    /// @return bool
    /// @param strKey 要加载的键名
    /// @param bUpdateTime 缓存最后一次打开文件时，对应的文件修改时间
    bool reloadConfigKey(std::string strKey, bool bUpdateTime = true);

    /// @brief 判断文件是否已经被修改过
    /// @return bool
    bool isConfigFileModified();

    bool getReloadFlag()
    {
        return m_reloadflag;
    }
    void setReloadFlag(bool flag)
    {
        m_reloadflag = flag;
    }

public:
    /// @brief 处理宕机时候的回调,上层继承后，需要调用底层的此函数
    /// @return void
    virtual void cbProcessException();

    /// @brief 主动发送消息失败后的回调
    /// @return void
    virtual void _cbSendError(PeerID peerID, const SFLib::Message::CNetMessage* msg);

    /// @brief 底层转发消息失败后的回调
    /// @return void
    virtual void _cbSendError(PeerID peerID, const SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);

public:
    CExternalClientPtr getExternalClient(ServerID serverID);
    bool getExternalClientList(EServerType serverType, std::vector<SFLib::CommonServer::CCommonClientPtr>& externalClientList);

    // 专用于 Logic -> Logic 的消息发送
    bool bstMsgToXSByType(EServerType serverType, const SFLib::Message::CNetMessage* msg);
    bool bstMsgToXSByType(EServerType serverType, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    bool sendMsgToXSByID(ServerID serverID, const SFLib::Message::CNetMessage* msg);
    bool sendMsgToXSByID(ServerID serverID, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    // 专用于 Logic -> External 的消息发送
    bool bstMsgToExternalServer(EServerType serverType, const SFLib::Message::CNetMessage* msg);
    bool bstMsgToExternalServer(EServerType serverType, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    bool sendMsgToExternalServer(ServerID serverID, const SFLib::Message::CNetMessage* msg);
    bool sendMsgToExternalServer(ServerID serverID, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    bool sendMsgToExternalServer(PeerID peerID, ServerID serverID, const SFLib::Message::CNetMessage* msg);
    bool sendMsgToExternalServer(PeerID peerID, ServerID serverID, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    // 专用于 External -> Logic 的消息发送
    bool bstMsgToLogicServer(EServerType serverType, const SFLib::Message::CNetMessage* msg);
    bool bstMsgToLogicServer(EServerType serverType, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    bool sendMsgToLogicServer(ServerID serverID, const SFLib::Message::CNetMessage* msg);
    bool sendMsgToLogicServer(ServerID serverID, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    bool sendMsgToLogicServer(PeerID peerID, ServerID serverID, const SFLib::Message::CNetMessage* msg);
    bool sendMsgToLogicServer(PeerID peerID, ServerID serverID, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

protected:
    bool _connectExternalServer(SFLib::ServerID serverID);
    bool _connectExternalServers(SFLib::ServerType serverType = INVALID_SERVER_TYPE);

public:
    /// @brief 其他Server与本地Server成功建立连接
    /// @return void
    /// @param serverID 对方ServerID
    /// @param serverType 对方Server类型
    virtual void cbServerEnter(ServerID serverID, EServerType serverType);

    /// @brief 其他Server初始化成功
    /// @return void
    /// @param serverID 对方ServerID
    /// @param serverType 对方Server类型
    virtual void cbServerInit(ServerID serverID, EServerType serverType);

    /// @brief 其他Server与本地Server连接断开
    /// @return void
    /// @param serverID 对方ServerID
    /// @param serverType 对方Server类型
    virtual void cbServerLeave(ServerID serverID, EServerType serverType);

    /// @brief 本地Server即将关闭
    /// @return void
    virtual void cbCloseServer();

    /// @brief 创建Peer，不能保证其它服务器已经创建成功
    /// @return void
    /// @param netPeerPtr
    virtual void cbPeerCreate(CNetPeerPtr& netPeerPtr);

    /// @brief Peer进入本地Server，保证其它服务器已经创建成功
    /// @return void
    /// @param peerID 进入的PeerID
    virtual void cbPeerEnter(CNetPeerPtr& netPeerPtr);

    /// @brief Peer断线重连Server，目前在 GW 和 SC 上使用
    /// @return void
    /// @param peerID 进入的PeerID
    virtual void cbPeerReEnter(CNetPeerPtr& netPeerPtr);

    /// @brief Peer离开本地Server，保证其它服务器已经还存在Peer
    /// @return void
    /// @param peerID 离开的PeerID
    virtual void cbPeerLeave(CNetPeerPtr& netPeerPtr, EPeerLeaveReason nReason);

    /// @brief 删除Peer，不能保证其它服务器已经还存在Peer
    /// @return void
    /// @param netPeerPtr
    virtual void cbPeerRemove(CNetPeerPtr& netPeerPtr);

    /// @brief CNetPeer::enterXXXServer的回调函数，通知该操作是否成功
    /// @return void
    /// @param peerID 请求的Peer
    /// @param ServerID 请求进入的Server
    /// @param result 是否成功
    virtual void cbPeerEnterServer(PeerID peerID, ServerID serverID, bool result);

    /// @brief CNetPeer::leaveXXXServer的回调函数，通知该操作是否成功
    /// @return void
    /// @param peerID 请求的Peer
    /// @param ServerID 离开当前的Server
    /// @param result 是否成功
    virtual void cbPeerLeaveServer(PeerID peerID, ServerID serverID, bool result);

    /// @brief 当其它Server申请CNetPeer::enterXXXServer或CNetPeer::leaveXXXServer操作，改变Peer的Server，本地Server触发该接口
    /// @return void
    /// @param peerID 发生变化的Peer
    /// @param serverID 新进入的Server
    virtual void cbSetPeerServer(PeerID peerID, ServerID serverID);

#ifndef USE_PIPESERVER_TEST_LOGIC

public:
    void setAcceptType(BCLib::Network::ENetType acceptType)
    {
        m_acceptType = acceptType;
    }
    BCLib::Network::ENetType getAcceptType()
    {
        return m_acceptType;
    }

    void setAcceptIP(const std::string& acceptIP)
    {
        m_acceptIP = acceptIP;
    }
    const std::string& getAcceptIP()
    {
        return m_acceptIP;
    }

    void setAcceptPort(BCLib::uint16 acceptPort)
    {
        m_acceptPort = acceptPort;
    }
    BCLib::uint16 getAcceptPort()
    {
        return m_acceptPort;
    }

protected:
    virtual bool _init();
    virtual int _final();

    virtual BCLib::Network::CTcpStubMgr* _cbGetStubMgr();
    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::uint16 id, BCLib::Framework::CMsgExecPtr& msgExecPtr);
    virtual void _onLC2LCNtfRemoveClient(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

#else

public:
    void setAcceptType(BCLib::Network::ENetType acceptType)
    {
        m_acceptType = acceptType;
    }
    BCLib::Network::ENetType getAcceptType()
    {
        return m_acceptType;
    }

    void setAcceptIP(const std::string& acceptIP)
    {
        m_acceptIP = acceptIP;
    }
    const std::string& getAcceptIP()
    {
        return m_acceptIP;
    }

    void setAcceptPort(BCLib::uint16 acceptPort)
    {
        m_acceptPort = acceptPort;
    }
    BCLib::uint16 getAcceptPort()
    {
        return m_acceptPort;
    }

protected:
    virtual bool _init();
    virtual int _final();

    virtual BCLib::Network::CPipeStubMgr* _cbGetStubMgr();
    virtual BCLib::Network::CPipeStubSPtr _cbNewStub(const BCLib::Network::CPipeSPtr& pipePtr);
    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::uint16 id, BCLib::Framework::CMsgExecPtr& msgExecPtr);
    virtual void _onLC2LCNtfRemoveClient(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

#endif

public:
    CNetPeerMgr* getPeerMgr()
    {
        return m_netPeerMgr;
    }

private:
    void _setPeerMgr(CNetPeerMgr* peerMgr)
    {
        m_netPeerMgr = peerMgr;
    }

protected:
    BCLib::Utility::CMutex m_mutex;
    BCLib::Utility::CHashMap<ServerID, SFLib::CommonServer::CCommonClientPtr> m_ExternalClientHashMap;

private:
    static CCommonServer* m_commonServer;

    BCLib::Utility::CDateTime m_dtConfigFileLastWriteTime;
    volatile bool m_reloadflag;

    ServerID m_serverID;
    std::string m_serverName;

    BCLib::Network::ENetType m_acceptType;
    std::string m_acceptIP;
    BCLib::uint16 m_acceptPort;

    CNetPeerMgr* m_netPeerMgr;
    friend class CNetPeerMgr;
};

}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_COMMONSERVER_H__
