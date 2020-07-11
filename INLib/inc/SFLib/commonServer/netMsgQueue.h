//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/netMsgQueue.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_NETMSGQUEUE_H__
#define __SFLIB_COMMONSERVER_NETMSGQUEUE_H__

#include <BCLib/utility/singleton.h>
#include <SFLib/message/netMsgQueue.hpp>
#include <SFLib/commonServer/msgLabel.h>

namespace SFLib
{
namespace CommonServer
{

typedef SFLib::Message::CNetMsgQueueCallback<SSFMsgLabel> CNetMsgQueueBySFMsgLabelCallback;

class SFLIB_COMMON_API CNetMsgQueueBySFMsgLabel : public SFLib::Message::CNetMsgQueue<SSFMsgLabel>
{
    BCLIB_SINGLETON_DECLARE(CNetMsgQueueBySFMsgLabel);

public:
    CNetMsgQueueBySFMsgLabel();
    virtual ~CNetMsgQueueBySFMsgLabel();
};

//////////////////////////////////////////////////////////////////////////

typedef SFLib::Message::CNetMsgQueueCallback<SUdpMsgLabel> CNetMsgQueueByUdpMsgLabelCallback;

class SFLIB_COMMON_API CNetMsgQueueByUdpMsgLabel : public SFLib::Message::CNetMsgQueue<SUdpMsgLabel>
{
    BCLIB_SINGLETON_DECLARE(CNetMsgQueueByUdpMsgLabel);

public:
    CNetMsgQueueByUdpMsgLabel();
    virtual ~CNetMsgQueueByUdpMsgLabel();
};

//////////////////////////////////////////////////////////////////////////

typedef SFLib::Message::CNetMsgQueueCallback<PeerID> CNetMsgQueueByPeerIDCallback;

class SFLIB_COMMON_API CNetMsgQueueByPeerID : public SFLib::Message::CNetMsgQueue<PeerID>
{
    BCLIB_SINGLETON_DECLARE(CNetMsgQueueByPeerID);

public:
    CNetMsgQueueByPeerID();
    virtual ~CNetMsgQueueByPeerID();
};
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_NETMSGQUEUE_H__
