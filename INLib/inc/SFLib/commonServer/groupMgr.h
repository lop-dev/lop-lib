//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/groupMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_GROUPMGR_H__
#define __SFLIB_COMMONSERVER_GROUPMGR_H__
#include <BCLib/utility/singleton.h>
#include <BCLib/utility/thread/mutex.h>
#include <BCLib/utility/hashMap.h>
#include <SFLib/commonServer/group.h>

namespace SFLib
{
namespace CommonServer
{
class CGroupMgr
{
    BCLIB_SINGLETON_DECLARE(CGroupMgr);

public:
    CGroupMgr();
    ~CGroupMgr();

    bool addGroup(CGroupPtr& group);
    void delGroup(GroupID groupID);

    bool getGroup(GroupID groupID, CGroupPtr& group);
    CGroupPtr getGroup(GroupID groupID);

    void sendMsgToAll(PeerID peerID, const SFLib::Message::CNetMessage* msg);
    void sendMsgToAll(PeerID peerID, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    void sendMsgToAll(const SFLib::Message::CNetMessage* msg);
    void sendMsgToAll(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

private:
    void _clear();

protected:
private:
    BCLib::Utility::CMutex m_mutex;
    BCLib::Utility::CHashMap<SFLib::GroupID, CGroupPtr> m_groupHashMap;
};
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_GROUPMGR_H__
