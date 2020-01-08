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

    void clear();

private:
    void _clear();

    BCLib::Utility::CHashMap<BCLib::uint32, CTcpClientPtr> m_netClientHashMap;
    BCLib::Utility::CMutex m_mutex;
};
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_NETCLIENT_TCPCLIENTMGR_H__
