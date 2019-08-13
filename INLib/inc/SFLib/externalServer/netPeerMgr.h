//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/externalServer/netPeerMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_EXTERNALSERVER_NETPEERMGR_H__
#define __SFLIB_EXTERNALSERVER_NETPEERMGR_H__

#include <BCLib/utility/singleton.h>
#include <SFLib/commonServer/netPeerMgr.h>
#include <SFLib/externalServer/netPeer.h>

namespace SFLib
{
namespace External
{
class CNetPeerMgr : public SFLib::CommonServer::CNetPeerMgr
{
    BCLIB_SINGLETON_DECLARE(CNetPeerMgr);

public:
    CNetPeerMgr();
    virtual ~CNetPeerMgr();

    CNetPeerPtr getNetPeer(PeerID peerID);

    virtual bool serializeTo(BCLib::Utility::CStream& stream) const;
    virtual bool serializeFrom(BCLib::Utility::CStream& stream);
};
}//External
}//SFLib

#endif//__SFLIB_EXTERNALSERVER_NETPEERMGR_H__
