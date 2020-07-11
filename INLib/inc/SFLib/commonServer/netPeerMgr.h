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
class SFLIB_COMMON_API CNetPeerMgr
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

    static CNetPeerMgr* getNetPeerMgr()
    {
        return m_netPeerMgr;
    }

    bool addNetPeer(CNetPeerPtr& netPeer);
    
    bool getNetPeer(PeerID peerID, CNetPeerPtr& netPeer);
    bool getNetPeerByEntityID(EntityID entityID, CNetPeerPtr& netPeer);
    CNetPeerPtrVec getNetPeers(ServerID serverID);

    void delNetPeer(PeerID peerID);

    /// @brief 当一个服务器断线了，清理跟它关联的所有玩家，会触发本进程cbPeerLeave()和cbPeerRemove()
    /// @return void
    /// @param serverType 服务器类型
    /// @param serverID   服务器编号
    void clear(ServerType serverType, ServerID serverID);
    /// @brief 清理所有玩家
    /// @return void
    void clear();

    BCLib::uint32 getNetPeerNum() { return m_mapNetPeerByPeerID.size(); }
    void traversal(CNetPeerMgrCallback& cb);

    bool serializeTo(BCLib::Utility::CStream& stream) const;
    bool serializeFrom(BCLib::Utility::CStream& stream);

protected:
	virtual bool _serializeTo(BCLib::Utility::CStream& stream) const;
	virtual bool _serializeFrom(BCLib::Utility::CStream& stream);

private:
    static CNetPeerMgr* m_netPeerMgr;

protected:
    BCLib::Utility::CHashMap<PeerID, CNetPeerPtr> m_mapNetPeerByPeerID;
    BCLib::Utility::CHashMap<EntityID, CNetPeerPtr> m_mapNetPeerByEntityID;
    BCLib::Utility::CMutex m_mutex;
};
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_NETPEERMGR_H__
