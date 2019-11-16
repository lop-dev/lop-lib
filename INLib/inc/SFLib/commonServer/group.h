//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/group.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_GROUP_H__
#define __SFLIB_COMMONSERVER_GROUP_H__
#include <BCLib/utility/pointer.h>
#include <BCLib/network/tcp/tcpStub.h>
#include <SFLib/commonDefine/baseDef.h>
#include <SFLib/message/message.h>

namespace SFLib
{
namespace CommonServer
{
class CGroup
{
public:
    CGroup(BCLib::Network::TcpStubID stubID, BCLib::Network::CTcpStubSPtr netStubPtr);
    virtual ~CGroup();

    BCLib::Network::TcpStubID getGroupID()
    {
        return m_stubID;
    }

    bool sendMsg(PeerID peerID, const SFLib::Message::CNetMessage* msg);
    bool sendMsg(PeerID peerID, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    bool sendMsg(const SFLib::Message::CNetMessage* msg);
    bool sendMsg(const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

private:
    BCLib::Network::TcpStubID m_stubID;
    BCLib::Network::CTcpStubSPtr m_netStubPtr;
};
typedef BCLib::Utility::CPointer<CGroup> CGroupPtr;
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_GROUP_H__
