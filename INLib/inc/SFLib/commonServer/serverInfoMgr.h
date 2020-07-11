//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/serverInfoMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_SERVERINFOMGR_H__
#define __SFLIB_COMMONSERVER_SERVERINFOMGR_H__

#include <BCLib/utility/thread/mutex.h>
#include <BCLib/utility/hashMap.h>
#include <BCLib/utility/thread/mutex.h>
#include <SFLib/message/gameFrame.h>

namespace SFLib
{
namespace CommonServer
{
class SFLIB_COMMON_API CServerInfoMgr
{
public:
    CServerInfoMgr();
    virtual ~CServerInfoMgr();

    void addServerInfo(SFLib::Message::SServerInfo& serverInfo);
    void addServerInfo(SFLib::Message::SServerInfo serverInfos[], int count);
    void clear();
    bool getServerInfo(BCLib::uint32 id, SFLib::Message::SServerInfo& serverInfo);
    bool getServerInfoList(EServerType serverType, std::vector<SFLib::Message::SServerInfo>& serverInfoList);
    bool getServerInfoList(std::vector<SFLib::Message::SServerInfo>& serverInfoList);

private:
    BCLib::Utility::CMutex m_mutex;
    BCLib::Utility::CHashMap<BCLib::uint32, SFLib::Message::SServerInfo> m_serverInfos;
};
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_SERVERINFOMGR_H__
