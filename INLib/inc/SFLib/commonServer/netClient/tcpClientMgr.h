//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/netClient/tcpClientMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_NETCLIENT_TCPCLIENTMGR_H__
#define __SFLIB_COMMONSERVER_NETCLIENT_TCPCLIENTMGR_H__

#include <BCLib/utility/singleton.h>
#include <BCLib/utility/hashMap.h>
#include <BCLib/utility/thread/mutex.h>
#include <SFLib/commonServer/netClient/tcpClient.h>

namespace SFLib
{
namespace CommonServer
{
class CNetClientMgr
{
    BCLIB_SINGLETON_DECLARE(CNetClientMgr);

public:
    CNetClientMgr();
    virtual ~CNetClientMgr();

    bool addNetClient(CTcpClientPtr& netClient);
    void delNetClient(ServerID serverID);
    CTcpClientPtr getNetClient(ServerID serverID);

    void checkBrokenServer();

    void clear();

private:
    void _clear();

private:
    BCLib::Utility::CHashMap<ServerID, CTcpClientPtr> m_netClientHashMap;
    BCLib::Utility::CMutex m_mutexNetClientHashMap;

    BCLib::Utility::CHashMap<ServerID, ServerType> m_mapAlreadyBrokenServer;
    BCLib::Utility::CMutex m_mutexAlreadyBrokenServer;
};
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_NETCLIENT_TCPCLIENTMGR_H__
