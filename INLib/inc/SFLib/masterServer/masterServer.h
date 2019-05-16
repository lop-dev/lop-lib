//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/masterServer/masterServer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_MASTERSERVER_MASTERSERVER_H__
#define __SFLIB_MASTERSERVER_MASTERSERVER_H__

#include <SFLib/commonServer/commonServer.h>
#include <SFLib/masterServer/masterStub.h>
#include <SFLib/message/peerMsg.h>

namespace SFLib
{
namespace Master
{
class CMasterServer : public SFLib::CommonServer::CCommonServer
{
public:
    CMasterServer();
    virtual ~CMasterServer();
    virtual GroupID getGroupID();
    virtual EServerType getServerType()
    {
        return ESERVER_MASTER;
    }

    void setRandKey(BCLib::uint32 val);
    BCLib::uint32 getRandKey();
    bool isValidRandKey(BCLib::uint32 val);

protected:
    virtual bool _init();
    virtual int _final();
    virtual int getLinkedServerIDs(PeerID peerID, EServerType serverType, std::vector<ServerID>& serverIDList);

    virtual BCLib::Network::CTcpStubSPtr _cbNewStub(const BCLib::Network::CTcpConnectionSPtr& connPtr);

    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::uint16 id, BCLib::Framework::CMsgExecPtr& msgExecPtr);
    virtual void _onXS2MSReqApplyForPeerID(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onXS2MSResCreatePeerID(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onXS2MSReqLeavePeerID(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onXS2MSAckLeavePeerID(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onXS2MSResEnterServerCreatePeerID(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onXS2MSResLeaveServerRemovePeerID(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

private:
	void _applyForPeerID(SFLib::Message::SMsgMS2XSResApplyForPeerID resApplyForPeerID);

private:
	BCLib::Utility::CHashMap<PeerID, SFLib::Message::SMsgMS2XSResApplyForPeerID> m_DelayApplyForPeerID;

    bool m_bRandKey1;
    BCLib::uint32 m_uRandKey1;
    BCLib::uint32 m_uRandKey2;
};
}//Master
}//SFLib

#endif//__SFLIB_MASTERSERVER_MASTERSERVER_H__
