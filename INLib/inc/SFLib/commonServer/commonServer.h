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
#include <SFLib/commonServer/netClient/commonClient.h>
#include <SFLib/commonServer/netClient/externalClient.h>
#include <SFLib/commonServer/netClient/tcpClientMgr.h>
#include <SFLib/commonServer/netPeerMgr.h>

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

    bool isNotifyMaster()
    {
        return m_notifyMaster;
    }

    //
    void setGroupID(GroupID groupID)
    {
        m_groupID = groupID;
    }

    virtual GroupID getGroupID()
    {
        return m_groupID;
    }

    virtual std::string getGroupName()
    {
        return "";
    }

    //
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
    // 向外部服务器发送消息
    bool bstMsgToExternalServer(EServerType serverType, const SFLib::Message::CNetMessage* msg);
    bool bstMsgToExternalServer(EServerType serverType, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

	bool bstMsgToExternalServer(PeerID peerID, EServerType serverType, const SFLib::Message::CNetMessage* msg);
	bool bstMsgToExternalServer(PeerID peerID, EServerType serverType, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    bool sendMsgToExternalServer(ServerID serverID, const SFLib::Message::CNetMessage* msg);
    bool sendMsgToExternalServer(ServerID serverID, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    bool sendMsgToExternalServer(PeerID peerID, ServerID serverID, const SFLib::Message::CNetMessage* msg);
    bool sendMsgToExternalServer(PeerID peerID, ServerID serverID, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    // 向逻辑服务器发送消息
    bool bstMsgToLogicServer(EServerType serverType, const SFLib::Message::CNetMessage* msg);
    bool bstMsgToLogicServer(EServerType serverType, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

	bool bstMsgToLogicServer(PeerID peerID, EServerType serverType, const SFLib::Message::CNetMessage* msg);
	bool bstMsgToLogicServer(PeerID peerID, EServerType serverType, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    bool sendMsgToLogicServer(ServerID serverID, const SFLib::Message::CNetMessage* msg);
    bool sendMsgToLogicServer(ServerID serverID, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    bool sendMsgToLogicServer(PeerID peerID, ServerID serverID, const SFLib::Message::CNetMessage* msg);
    bool sendMsgToLogicServer(PeerID peerID, ServerID serverID, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

protected:
	virtual CCommonClientPtr _createLogicClient(SFLib::Message::SServerInfo& serverInfo);
	virtual CCommonClientPtr _createExternalClient();

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

    /// @brief 其他Server与本地Server连接断开，可能是异常断开
    /// @return void
    /// @param serverID 对方ServerID
    /// @param serverType 对方Server类型
    virtual void cbServerLeave(ServerID serverID, EServerType serverType);

	/// @brief 本地Server需要保存，是MS发出的通知
	/// @return void
	virtual void cbServerSave(GroupID groupID);

	/// @brief 本地Server即将关闭，是MS发出的通知
	/// @return void
	virtual void cbServerClose(GroupID groupID);

    /// @brief 创建Peer，不能保证其它服务器已经创建成功
    /// @return void
    /// @param netPeerPtr
    virtual void cbPeerCreate(CNetPeerPtr& netPeerPtr);

    /// @brief Peer进入本地Server，保证其它服务器已经创建成功
    /// @return void
    /// @param peerID 进入的PeerID
    /// @param nReason 进入的原因
    virtual void cbPeerEnter(CNetPeerPtr& netPeerPtr, EPeerEnterReason nReason);

    /// @brief Peer离开本地Server，保证其它服务器已经还存在Peer
    /// @return void
    /// @param peerID 离开的PeerID
    /// @param nReason 离开的原因
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

    /// @brief 当Server申请CNetPeer::enterXXXServer或CNetPeer::leaveXXXServer操作，改变Peer的Server，本地Server触发该接口
    /// @return void
    /// @param peerID 发生变化的Peer
    /// @param serverType 新进入的服务器类型
    /// @param serverID 新进入的服务器编号，该值如果是 INVALID_SERVER_ID，表示和服务器断开关联
    virtual void cbSetPeerServer(PeerID peerID, EServerType serverType, ServerID serverID);

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
	// 是否通过 Master 转发进行全服处理（如果为 true 时，部分操作只允许在网关上发起请求）
    bool m_notifyMaster;

private:
    static CCommonServer* m_commonServer;

    BCLib::Utility::CDateTime m_dtConfigFileLastWriteTime;
    volatile bool m_reloadflag;

    GroupID m_groupID;
    ServerID m_serverID;
    std::string m_serverName;

    BCLib::Network::ENetType m_acceptType;
    std::string m_acceptIP;
    BCLib::uint16 m_acceptPort;

    CNetPeerMgr* m_netPeerMgr;
    friend class CNetPeerMgr;
	friend class CServerMgr;
};

}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_COMMONSERVER_H__
