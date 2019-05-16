//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/masterServer/masterConfig.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_MASTERSERVER_MASTERCONFIG_H__
#define __SFLIB_MASTERSERVER_MASTERCONFIG_H__

#include <BCLib/utility/singleton.h>
#include <BCLib/utility/thread/mutex.h>
#include <BCLib/utility/baseDec.h>
#include <BCLib/utility/io/xmlFile.h>
#include <BCLib/network/tcp/tcpStubMgr.h>
#include <SFLib/message/gameFrame.h>
#include <SFLib/masterServer/masterStub.h>

namespace SFLib
{
namespace Master
{

struct SLogicServerConnectionItem
{
    std::vector<EServerType> m_vecConnectionServerType;
};

struct SLogicServerListItem
{
    SLogicServerListItem()
    :m_serverID(INVALID_SERVER_ID)
    ,m_serverType(ESERVER_UNKNOW)
    ,m_acceptNetType(BCLib::Network::NETT_NULL)
    ,m_acceptIP("")
    ,m_acceptPort(0)
    ,m_outerIP("")
    ,m_outerPort(0)
    ,m_innerIP("")
    ,m_necessary(false)
    ,m_isFetched(false)
    ,m_isActive(false)
    ,m_isInited(false)
    ,m_bClearPeers(false)
    ,m_bCloseServer(false)
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

    ServerID m_serverID;
    EServerType m_serverType;
    BCLib::Network::ENetType m_acceptNetType;
    std::string m_acceptIP;
    BCLib::int16 m_acceptPort;
    std::string m_outerIP;
    BCLib::int16 m_outerPort;
    std::string m_innerIP;
    bool m_necessary;

    bool m_isFetched;
    bool m_isActive;
    bool m_isInited;

    std::vector<ServerID> m_vecAlreadyNotifyActive;
    std::vector<ServerID> m_vecAlreadyShakeHands;
    std::vector<ServerID> m_vecAlreadyNotifyInited;

    bool m_bClearPeers;
    bool m_bCloseServer;
};
typedef std::vector<SLogicServerListItem> SLogicServerListItemVec;

struct SExternalServerListItem
{
	SExternalServerListItem()
		:m_serverID(INVALID_SERVER_ID)
		,m_serverType(ESERVER_UNKNOW)
		,m_outerIP("")
		,m_outerPort(0)
		,m_isFetched(false)
	{
	}

    std::string getDebugPrompt() const
    {
        BCLib::Utility::CStringA strPrompt = "";
        strPrompt.format("ServerType[%s] ServerID[%d] OuterIP[%s] OuterPort[%d]", getServerTypeName(m_serverType).c_str(), m_serverID, m_outerIP.c_str(), m_outerPort);
        return strPrompt;
    }

    ServerID m_serverID;
    EServerType m_serverType;
    std::string m_outerIP;
    BCLib::uint16 m_outerPort;
	bool m_isFetched;
};
typedef std::vector<SExternalServerListItem> SExternalServerListItemVec;

struct SActiveServerList
{
    std::vector<ServerID> m_serverIDList;
};
typedef std::map<EServerType, SActiveServerList> CActiveServerMap;

class CMasterConfig
{
    BCLIB_SINGLETON_DECLARE(CMasterConfig);

    friend class CMasterServer;
    friend class CMasterStub;
	friend class CExternalClient;

public:
    CMasterConfig();
    virtual ~CMasterConfig();

    void sendMsgToAllStub(const SFLib::Message::SNetMessage* msg, unsigned int msgSize);
    void sendMsgToAllStubExceptID(const SFLib::Message::SNetMessage* msg, unsigned int msgSize, ServerID serverID);
	void sendMsgToAllStubExceptType(const SFLib::Message::SNetMessage* msg, unsigned int msgSize, BCLib::int32 serverType);
	void sendMsgToAllStubAssignType(const SFLib::Message::SNetMessage* msg, unsigned int msgSize, BCLib::int32 serverType);

	void sendMsgToAllStub(const SFLib::Message::CNetMessage* msg);
	void sendMsgToAllStubExceptID(const SFLib::Message::CNetMessage* msg, ServerID serverID);
	void sendMsgToAllStubExceptType(const SFLib::Message::CNetMessage* msg, BCLib::int32 serverType);
	void sendMsgToAllStubAssignType(const SFLib::Message::CNetMessage* msg, BCLib::int32 serverType);

    void notifyCloseServer();

    bool loadConfig(const std::string& strFile = "");
    bool reloadConfig();

protected:
    void setServerMgrServerList();

    void setFetched(ServerID serverID, bool isFetched);
    void setActive(ServerID serverID, bool isActive);
    void setShakeHands(ServerID srcServerID, ServerID dstServerID);
    void setInited(ServerID serverID, bool isInited);

    ServerID fetchServerID(EServerType serverType, BCLib::Network::ENetType netType, const char* peerIP);
    EServerType fetchServerType(ServerID serverID, BCLib::Network::ENetType netType, const char* peerIP);

	ServerID fetchExternalServerID(EServerType serverType);
    bool getAcceptInfo(ServerID serverID, SFLib::Message::SServerAcceptInfo& acceptInfo);

    void sendLogicServerListToStub(CMasterStub* masterStub);
    void sendExternalServerListToStub(CMasterStub* masterStub);

    void notifyAlreadyActiveStubsAll();
    void notifyAlreadyInitedStubsAll();

    bool getMasterServerInfo(SLogicServerListItem& itemInfo);

    void setClearPeers(ServerID serverID, bool bClear);
    bool isAllServerClearPeers();
    void setCloseServer(ServerID serverID, bool bClose);
    bool isAllServerCloseServer();

    int getActiveServerMap(CActiveServerMap& serverMap);

    SExternalServerListItemVec& getExternalServerList();

private:
	bool _isNecessaryServer(ServerID serverID);
	bool _isNecessaryServerActives();

	bool _loadLogicServerType(BCLib::Utility::CXmlFile& xmlFile, BCLib::Utility::HXmlNode logicServerTypeNode);
    bool _loadLogicServerConnection(BCLib::Utility::CXmlFile& xmlFile, BCLib::Utility::HXmlNode connectionNode);
    bool _loadLogicServerList(BCLib::Utility::CXmlFile& xmlFile, BCLib::Utility::HXmlNode serverListNode);

    bool _loadExternalServerType(BCLib::Utility::CXmlFile& xmlFile, BCLib::Utility::HXmlNode externalServerTypeNode);
    bool _loadExternalServerList(BCLib::Utility::CXmlFile& xmlFile, BCLib::Utility::HXmlNode serverListNode);

    bool _addLogicServerListItem(SLogicServerListItem& item);
    bool _addExternalServerListItem(SExternalServerListItem& item);

    void _setServerMgrLogicServerList();
    void _setServerMgrExternalServerList();

    EServerType _getLogicServerTypeByString(const std::string& str);
    EServerType _getExternalServerTypeByString(const std::string& str);

    bool _shouldConnectTo(EServerType startServer, EServerType endServer);

private:
    BCLib::Utility::CMutex m_mutex;
    BCLib::Utility::CMutex m_mutexEx;

    std::string m_strFile;

    std::map<std::string, EServerType> m_logicServerType;
    std::map<EServerType, SLogicServerConnectionItem> m_logicServerConnection;
    SLogicServerListItemVec m_logicServerList;

    std::map<std::string, EServerType> m_externalServerType;
    SExternalServerListItemVec m_externalServerList;
};
}//Master
}//SFLib

#endif//__SFLIB_MASTERSERVER_MASTERCONFIG_H__
