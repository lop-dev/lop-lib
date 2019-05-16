//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/msgLabel.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_MSGLABEL_H__
#define __SFLIB_COMMONSERVER_MSGLABEL_H__

#include <BCLib/network/tcp/tcpStub.h>
#include <BCLib/network/sockAddr.h>
#include <BCLib/framework/thdMsgLabel.h>
#include <SFLib/commonDefine/baseDef.h>

namespace SFLib
{
namespace CommonServer
{
#ifdef WIN32
#pragma pack (push, 1)
#else
#pragma pack (1)
#endif

struct SSFMsgLabel : public BCLib::Framework::SThdMsgLabel
{
    SSFMsgLabel()
    :BCLib::Framework::SThdMsgLabel(sizeof(SSFMsgLabel))
    {
        m_stubID = 0;
        m_peerID = 0;
        m_serverID = 0;
    }

    BCLib::Network::TcpStubID m_stubID;
    ServerID m_serverID;
    PeerID m_peerID;
};

struct SUdpMsgLabel : public BCLib::Framework::SThdMsgLabel
{
    SUdpMsgLabel()
        :BCLib::Framework::SThdMsgLabel(sizeof(SUdpMsgLabel))
    {
    }

    BCLib::Network::CSockAddr m_peerAddr;
};

#ifdef WIN32
#pragma pack (pop)
#else
#pragma pack ()
#endif
}//CommonServer
}//SFLib


#endif//__SFLIB_COMMONSERVER_MSGLABEL_H__
