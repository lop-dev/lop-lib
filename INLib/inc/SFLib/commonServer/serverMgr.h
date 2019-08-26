//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/serverMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_SERVERMGR_H__
#define __SFLIB_COMMONSERVER_SERVERMGR_H__

#include <vector>
#include <BCLib/utility/hashMap.h>
#include <BCLib/utility/singleton.h>
#include <SFLib/message/gameFrame.h>
#include <SFLib/message/message.h>
#include <SFLib/commonServer/msgLabel.h>
#include <BCLib/network/tcp/tcpSend.h>
#include <SFLib/commonServer/xmlConfig.h>
#include <SFLib/commonServer/netClient/externalClient.h>

namespace SFLib
{
namespace CommonServer
{
class CServerMgr
{
    BCLIB_SINGLETON_DECLARE(CServerMgr);

public:
    CServerMgr();
    virtual ~CServerMgr();

    void setLogicServerList(const SFLib::Message::SMsgMS2XSNtfLogicServerList* msg, int msgSize);
    void setExternalServerList(const SFLib::Message::SMsgMS2XSNtfExternalServerList* msg, int msgSize);
    std::vector<SFLib::Message::SServerInfo> getExternalServerList();

    void setNetSendByClient(ServerID serverID);
    void setNetSendByStub(ServerID serverID);
    bool isSetNetSend(ServerID serverID);

    bool getServerInfo(ServerID serverID, SFLib::Message::SServerInfo& serverInfo);
    int getActiveServerID(EServerType serverType, std::vector<ServerID>& serverIDList);
    ServerID getActiveServerID(EServerType serverType);

public:
	bool connectExternalServer(SFLib::ServerID serverID);
	bool connectExternalServers(SFLib::ServerType serverType = INVALID_SERVER_TYPE);
	CExternalClientPtr randomExternalClient(ServerType serverType);

public:
    virtual bool bstMsgToXSByType(EServerType serverType, const SFLib::Message::CNetMessage* msg);
    virtual bool bstMsgToXSByType(EServerType serverType, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    virtual bool sendMsgToXSByID(ServerID serverID, const SFLib::Message::CNetMessage* msg);
    virtual bool sendMsgToXSByID(ServerID serverID, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

private:
    void _clear();

private:
    struct SServerInfoExt
    {
		SServerInfoExt()
        {
        }

        int send(const SFLib::Message::CNetMessage* msg);
        int send(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

		SFLib::CommonServer::SServerInfo m_serverInfo;
        BCLib::Network::CTcpSendSPtr m_netSend;
    };

private:
    BCLib::Utility::CHashMap<BCLib::uint32, SServerInfoExt*> m_serverListByID;
    std::vector<SServerInfoExt*> m_serverListByType[ESERVER_MAX - ESERVER_GAMECLIENT];
};
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_SERVERMGR_H__
