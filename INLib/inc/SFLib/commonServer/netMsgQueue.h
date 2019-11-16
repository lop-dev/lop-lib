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
#include <SFLib/commonDefine/netMsgQueue.hpp>
#include <SFLib/commonServer/msgLabel.h>

namespace SFLib
{
namespace CommonServer
{

typedef SFLib::CommonDefine::CNetMsgQueueCallback<SSFMsgLabel> CNetMsgQueueBySFMsgLabelCallback;

class CNetMsgQueueBySFMsgLabel : public SFLib::CommonDefine::CNetMsgQueue<SSFMsgLabel>
{
    BCLIB_SINGLETON_DECLARE(CNetMsgQueueBySFMsgLabel);

public:
    CNetMsgQueueBySFMsgLabel();
    virtual ~CNetMsgQueueBySFMsgLabel();
};

//////////////////////////////////////////////////////////////////////////

typedef SFLib::CommonDefine::CNetMsgQueueCallback<SUdpMsgLabel> CNetMsgQueueByUdpMsgLabelCallback;

class CNetMsgQueueByUdpMsgLabel : public SFLib::CommonDefine::CNetMsgQueue<SUdpMsgLabel>
{
    BCLIB_SINGLETON_DECLARE(CNetMsgQueueByUdpMsgLabel);

public:
    CNetMsgQueueByUdpMsgLabel();
    virtual ~CNetMsgQueueByUdpMsgLabel();
};

//////////////////////////////////////////////////////////////////////////

typedef SFLib::CommonDefine::CNetMsgQueueCallback<PeerID> CNetMsgQueueByPeerIDCallback;

class CNetMsgQueueByPeerID : public SFLib::CommonDefine::CNetMsgQueue<PeerID>
{
    BCLIB_SINGLETON_DECLARE(CNetMsgQueueByPeerID);

public:
    CNetMsgQueueByPeerID();
    virtual ~CNetMsgQueueByPeerID();
};

//////////////////////////////////////////////////////////////////////////

struct SGroupKey
{
    BCLib::Network::TcpStubID m_stubID;
    PeerID m_peerID;
};

typedef SFLib::CommonDefine::CNetMsgQueueCallback<SGroupKey> CNetMsgQueueByGroupKeyCallback;

class CNetMsgQueueByGroupKey : public SFLib::CommonDefine::CNetMsgQueue<SGroupKey>
{
    BCLIB_SINGLETON_DECLARE(CNetMsgQueueByGroupKey);

public:
    CNetMsgQueueByGroupKey();
    virtual ~CNetMsgQueueByGroupKey();
};
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_NETMSGQUEUE_H__
