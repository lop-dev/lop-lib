//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/gameClient/netMsgQueue.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_GAMECLIENT_NETMSGQUEUE_H__
#define __SFLIB_GAMECLIENT_NETMSGQUEUE_H__

#include <BCLib/utility/singleton.h>
#include <SFLib/commonDefine/netMsgQueue.hpp>

namespace SFLib
{
namespace Client
{
typedef SFLib::CommonDefine::CNetMsgQueueCallback<PeerID> CNetMsgQueueByPeerIDCallback;

class CNetMsgQueueByPeerID : public SFLib::CommonDefine::CNetMsgQueue<PeerID>
{
    BCLIB_SINGLETON_DECLARE(CNetMsgQueueByPeerID);

public:
    CNetMsgQueueByPeerID();
    virtual ~CNetMsgQueueByPeerID();

};
}//Client
}//SFLib

#endif//__SFLIB_GAMECLIENT_NETMSGQUEUE_H__
