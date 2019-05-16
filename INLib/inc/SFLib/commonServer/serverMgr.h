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
    virtual bool bstMsgToXSByType(EServerType serverType, const SFLib::Message::CNetMessage* msg);
    virtual bool bstMsgToXSByType(EServerType serverType, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    virtual bool sendMsgToXSByID(ServerID serverID, const SFLib::Message::CNetMessage* msg);
    virtual bool sendMsgToXSByID(ServerID serverID, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

private:
    void _clear();

private:
    struct SServerInfo
    {
        SServerInfo()
        {
            m_createTime = BCLib::Utility::CDateTime::now().getTime();
        }

        int send(const SFLib::Message::CNetMessage* msg);
        int send(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

        ServerID m_serverID;
        EServerType m_serverType;
        BCLib::Network::ENetType m_acceptNetType;
        std::string m_acceptIP;
        BCLib::uint16 m_acceptPort;
        std::string m_outerIP;
        BCLib::uint16 m_outerPort;
        std::string m_innerIP;
        BCLib::Network::CTcpSendSPtr m_netSend;
        time_t m_createTime;

        std::string getDebugPrompt() const
        {
            BCLib::Utility::CStringA strPrompt = "";
            strPrompt.format("ServerType[%s], ServerID[%d]", getServerTypeName(m_serverType).c_str(), m_serverID);
            return strPrompt;
        }
    };

private:
    BCLib::Utility::CHashMap<BCLib::uint32, SServerInfo*> m_serverListByID;
    std::vector<SServerInfo*> m_serverListByType[ESERVER_MAX - ESERVER_GAMECLIENT];
};
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_SERVERMGR_H__
