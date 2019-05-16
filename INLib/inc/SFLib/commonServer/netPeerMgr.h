//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/netPeerMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_NETPEERMGR_H__
#define __SFLIB_COMMONSERVER_NETPEERMGR_H__

#include <BCLib/utility/thread/mutex.h>
#include <BCLib/utility/hashMap.h>
#include <SFLib/commonServer/netPeer.h>

namespace SFLib
{
namespace CommonServer
{
class CNetPeerMgr
{
public:
    class CNetPeerMgrCallback
    {
    public:
        virtual ~CNetPeerMgrCallback()
        {
        }
        virtual bool execute(CNetPeerPtr& stub) = 0;
    };
public:
    CNetPeerMgr();
    virtual ~CNetPeerMgr();

    bool addNetPeer(CNetPeerPtr& netPeer);
    void delNetPeer(PeerID peerID);
    bool getNetPeer(PeerID peerID, CNetPeerPtr& netPeer);
    void clear();
    void traversal(CNetPeerMgrCallback& cb);

	BCLib::uint32 getNetPeerNum() { return m_netPeerHasnMap.size(); }

    virtual bool serializeTo(BCLib::Utility::CStream& stream) const;
    virtual bool serializeFrom(BCLib::Utility::CStream& stream);

private:
    BCLib::Utility::CHashMap<PeerID, CNetPeerPtr> m_netPeerHasnMap;
    BCLib::Utility::CMutex m_mutex;
};
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_NETPEERMGR_H__
