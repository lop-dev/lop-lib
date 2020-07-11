//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/logicServer/netPeerMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_LOGICSERVER_NETPEERMGR_H__
#define __SFLIB_LOGICSERVER_NETPEERMGR_H__

#include <BCLib/utility/singleton.h>
#include <SFLib/commonServer/netPeerMgr.h>
#include <SFLib/logicServer/netPeer.h>

namespace SFLib
{
namespace Logic
{
class SFLIB_LOGIC_API CNetPeerMgr : public SFLib::CommonServer::CNetPeerMgr
{
    BCLIB_SINGLETON_DECLARE(CNetPeerMgr);

public:
    CNetPeerMgr();
    virtual ~CNetPeerMgr();

    CNetPeerPtr getNetPeer(PeerID peerID);
	CNetPeerPtr getNetPeerByEntityID(EntityID entityID);

    bool addEnterOrLeaveServerByServerID(ServerID serverID, SFLib::Message::SReqEnterOrLeaveServer& reqEnterOrLeaveServer);
    bool addEnterOrLeaveServerByServerType(ServerType serverType, SFLib::Message::SReqEnterOrLeaveServer& reqEnterOrLeaveServer);

    void updateEnterOrLeaveServerByServerID();
    void updateEnterOrLeaveServerByServerType();

protected:
	virtual bool _serializeTo(BCLib::Utility::CStream& stream) const;
	virtual bool _serializeFrom(BCLib::Utility::CStream& stream);

private:
    typedef BCLib::Utility::CHashMap<ServerID, SFLib::Message::SReqEnterOrLeaveServerVec> ReqEnterOrLeaveByServerIDMap;
    ReqEnterOrLeaveByServerIDMap m_mapReqEnterOrLeaveByServerID;
    BCLib::Utility::CMutex  m_mutexReqEnterOrLeaveByServerID;

    typedef BCLib::Utility::CHashMap<ServerType, SFLib::Message::SReqEnterOrLeaveServerVec> ReqEnterOrLeaveByServerTypeMap;
    ReqEnterOrLeaveByServerTypeMap m_mapReqEnterOrLeaveByServerType;
    BCLib::Utility::CMutex  m_mutexReqEnterOrLeaveByServerType;
};
}//Logic
}//SFLib

#endif//__SFLIB_LOGICSERVER_NETPEERMGR_H__
