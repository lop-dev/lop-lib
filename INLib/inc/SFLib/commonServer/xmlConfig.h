//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/xmlConfig.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_XMLCONFIG_H__
#define __SFLIB_COMMONSERVER_XMLCONFIG_H__

#include <BCLib/utility/singleton.h>
#include <BCLib/utility/thread/mutex.h>
#include <BCLib/utility/baseDec.h>
#include <BCLib/utility/io/xmlFile.h>
#include <BCLib/network/tcp/tcpStubMgr.h>
#include <SFLib/message/gameFrame.h>
#include <SFLib/commonServer/tcpStub.h>

namespace SFLib
{
namespace CommonServer
{

struct SServerInfo
{
	SServerInfo()
	: m_groupID(0)
    , m_serverID(INVALID_SERVER_ID)
	, m_serverType(ESERVER_UNKNOW)
	, m_acceptNetType(BCLib::Network::NETT_NULL)
	, m_acceptIP("")
	, m_acceptPort(0)
	, m_outerIP("")
	, m_outerPort(0)
	, m_innerIP("")
	, m_necessary(false)
	{
	}

	std::string getDebugPrompt() const;

	void setServerInfo(SFLib::Message::SServerInfo serverInfo);
	void getServerInfo(SFLib::Message::SServerInfo& serverInfo);

    void setServerInfo(SFLib::CommonServer::SServerInfo serverInfo);
    void getServerInfo(SFLib::CommonServer::SServerInfo& serverInfo);

    void setServerInfoForReload(SFLib::CommonServer::SServerInfo serverInfo);

    GroupID m_groupID;
	ServerID m_serverID;
	EServerType m_serverType;
	BCLib::Network::ENetType m_acceptNetType;
	std::string m_acceptIP;
	BCLib::uint16 m_acceptPort;
	std::string m_outerIP;
	BCLib::uint16 m_outerPort;
	std::string m_innerIP;
	bool m_necessary;
};

//////////////////////////////////////////////////////////////////////////

struct SLogicServerConnectionItem
{
    std::vector<EServerType> m_vecConnectionServerType;
};

struct SLogicServerListItem
{
    SLogicServerListItem()
    :m_isFetched(false)
    ,m_isActive(false)
    ,m_isInited(false)
    ,m_bClearPeers(false)
    ,m_bServerSave(false)
    {
    }

    bool isNotifyActive(ServerID serverID);
    void addNotifyActive(ServerID serverID);
    void delNotifyActive(ServerID serverID);

    bool isShakeHands(ServerID serverID);
    void addShakeHands(ServerID serverID);
    void delShakeHands(ServerID serverID);

    bool isNotifyInited(ServerID serverID);
    void addNotifyInited(ServerID serverID);
    void delNotifyInited(ServerID serverID);

	SServerInfo m_serverInfo;

    bool m_isFetched;
    bool m_isActive;
    bool m_isInited;

    std::vector<ServerID> m_vecAlreadyNotifyActive;
    std::vector<ServerID> m_vecAlreadyShakeHands;
    std::vector<ServerID> m_vecAlreadyNotifyInited;

    bool m_bClearPeers;
    bool m_bServerSave;
};
typedef std::vector<SLogicServerListItem> SLogicServerListItemVec;

struct SExternalServerListItem
{
	SExternalServerListItem()
	:m_isFetched(false)
    ,m_isActive(false)
    ,m_isInited(false)
	{
	}

    bool isNotifyActive(ServerID serverID);
    void addNotifyActive(ServerID serverID);
    void delNotifyActive(ServerID serverID);

	SServerInfo m_serverInfo;

	bool m_isFetched;
    bool m_isActive;
    bool m_isInited;

    std::vector<ServerID> m_vecAlreadyNotifyActive;
};
typedef std::vector<SExternalServerListItem> SExternalServerListItemVec;

struct SActiveServerList
{
    std::vector<ServerID> m_serverIDList;
};
typedef std::map<EServerType, SActiveServerList> SActiveServerListMap;

//////////////////////////////////////////////////////////////////////////

class CXmlConfig
{
    BCLIB_SINGLETON_DECLARE(CXmlConfig);

    friend class CCommonServer;
    friend class CMasterStub;
	friend class CExternalClient;

public:
    CXmlConfig();
    virtual ~CXmlConfig();

    void sendMsgToAllStub(const SFLib::Message::SNetMessage* msg, unsigned int msgSize);
    void sendMsgToAllStubExceptID(const SFLib::Message::SNetMessage* msg, unsigned int msgSize, ServerID serverID);
	void sendMsgToAllStubExceptType(const SFLib::Message::SNetMessage* msg, unsigned int msgSize, BCLib::int32 serverType);
	void sendMsgToAllStubAssignType(const SFLib::Message::SNetMessage* msg, unsigned int msgSize, BCLib::int32 serverType);

	void sendMsgToAllStub(const SFLib::Message::CNetMessage* msg);
	void sendMsgToAllStubExceptID(const SFLib::Message::CNetMessage* msg, ServerID serverID);
	void sendMsgToAllStubExceptType(const SFLib::Message::CNetMessage* msg, BCLib::int32 serverType);
	void sendMsgToAllStubAssignType(const SFLib::Message::CNetMessage* msg, BCLib::int32 serverType);

    void notifyCloseServer(GroupID groupID);

	bool loadConfig(const std::string& strFile = "");
	bool reloadConfig();

// 以下函数开放出来主要是给 CMasterServer 和 CMasterStub 用的
public:
	/// @brief 模拟收到网络消息 SMsgMS2XSNtfLogicServerList 后，直接对 CServerMgr 进行设置
	/// @return void
	void setServerMgrLogicServerList();
	/// @brief 模拟收到网络消息 SMsgMS2XSNtfExternalServerList 后，直接对 CServerMgr 进行设置
	/// @return void
	void setServerMgrExternalServerList();

    void setLogicServerFetched(ServerID serverID, bool isFetched);
    void setLogicServerActive(ServerID serverID, bool isActive);
	void setLogicServerInited(ServerID serverID, bool isInited);
    void setLogicServerShakeHands(ServerID srcServerID, ServerID dstServerID);

    void setExternalServerFetched(ServerID serverID, bool isFetched);
    void setExternalServerActive(ServerID serverID, bool isActive);
    void setExternalServerInited(ServerID serverID, bool isInited);

    ServerID fetchLogicServerID(SFLib::EServerType serverType, BCLib::Network::ENetType netType, const char* peerIP);
    EServerType fetchLogicServerType(ServerID serverID, BCLib::Network::ENetType netType, const char* peerIP);
    SServerInfo* getLogicServerInfo(ServerID serverID);

	ServerID fetchExternalServerID(SFLib::EServerType serverType, BCLib::Network::ENetType netType, const char* peerIP);
	EServerType fetchExternalServerType(ServerID serverID, BCLib::Network::ENetType netType, const char* peerIP);
    SServerInfo* getExternalServerInfo(ServerID serverID);

    bool getLogicServerAcceptInfo(ServerID serverID, SFLib::Message::SServerAcceptInfo& acceptInfo);
	bool getExternalServerAcceptInfo(ServerID serverID, SFLib::Message::SServerAcceptInfo& acceptInfo);

    void sendLogicServerListToStub(CTcpStub* tcpStub, GroupID groupID = 0);
    void sendExternalServerListToStub(CTcpStub* tcpStub);

    void notifyAlreadyActiveStubsAll();
    void notifyAlreadyInitedStubsAll();

    void setClearPeers(ServerID serverID, bool bClear);
    bool isAllServerClearPeers();
    void setServerSave(ServerID serverID, bool bSave);
    bool isAllServerServerSave();

    int getLogicActiveServerListMap(SActiveServerListMap& serverMap);

    SExternalServerListItemVec& getExternalServerList();

private:
	bool _isNecessaryLogicServer(ServerID serverID);
	bool _isNecessaryLogicServerActives();

    bool _isNecessaryExternalServer(ServerID serverID);
    bool _isNecessaryExternalServerActives();

	bool _loadLogicServerType(BCLib::Utility::CXmlFile& xmlFile, BCLib::Utility::HXmlNode logicServerTypeNode);
    bool _loadExternalServerType(BCLib::Utility::CXmlFile& xmlFile, BCLib::Utility::HXmlNode externalServerTypeNode);

    bool _loadLogicServerConnection(BCLib::Utility::CXmlFile& xmlFile, BCLib::Utility::HXmlNode connectionNode);

    bool _loadLogicServerList(BCLib::Utility::CXmlFile& xmlFile, BCLib::Utility::HXmlNode serverListNode);
    bool _loadGatewayServerList(BCLib::Utility::CXmlFile& xmlFile, BCLib::Utility::HXmlNode serverListNode);
    bool _loadExternalServerList(BCLib::Utility::CXmlFile& xmlFile, BCLib::Utility::HXmlNode serverListNode);

    bool _addLogicServerListItem(SLogicServerListItem& item);
    bool _addExternalServerListItem(SExternalServerListItem& item);

    EServerType _getLogicServerTypeByString(const std::string& str);
    EServerType _getExternalServerTypeByString(const std::string& str);

    bool _shouldConnectTo(EServerType startServer, EServerType endServer);

private:
    std::string m_strFile;

    std::map<std::string, EServerType> m_logicServerType;
    std::map<EServerType, SLogicServerConnectionItem> m_logicServerConnection;
    SLogicServerListItemVec m_logicServerList;
	BCLib::Utility::CMutex m_mutexLogicServerList;

    std::map<std::string, EServerType> m_externalServerType;
    SExternalServerListItemVec m_externalServerList;
	BCLib::Utility::CMutex m_mutexExternalServerList;
};
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_XMLCONFIG_H__
