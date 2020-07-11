//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/tcpStubMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_TCPSTUBMGR_H__
#define __SFLIB_COMMONSERVER_TCPSTUBMGR_H__

#include <BCLib/utility/singleton.h>
#include <BCLib/network/tcp/tcpStubMgr.h>
#include <SFLib/commonDefine/baseDef.h>
#include <SFLib/message/message.h>
namespace SFLib
{
namespace CommonServer
{
class SFLIB_COMMON_API CTcpStubMgr : public BCLib::Network::CTcpStubMgr
{
    BCLIB_SINGLETON_DECLARE(CTcpStubMgr);

public:
    CTcpStubMgr();
    virtual ~CTcpStubMgr();

    bool delStub(const BCLib::Network::CTcpStubSPtr& stub);
    bool delStub(BCLib::Network::TcpStubID stubID);
    BCLib::Network::CTcpStubSPtr getStubByServerID(ServerID serverID);
    void addServerID(BCLib::Network::TcpStubID stubID, ServerID serverID);

    std::string getDebugPrompt(const BCLib::Network::TcpStubID stubID);

private:
    BCLib::Utility::CMutex m_mutex;
    BCLib::Utility::CHashMap<ServerID, BCLib::Network::TcpStubID> m_serverHashMap;
};
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_TCPSTUBMGR_H__
