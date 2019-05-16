//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/logicServer/logicProxy.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_LOGICSERVER_LOGICPROXY_H__
#define __SFLIB_LOGICSERVER_LOGICPROXY_H__

#include <BCLib/network/udp/udpServer.h>
#include <SFLib/logicServer/netPeer.h>

namespace SFLib
{
namespace Logic
{
class CLogicProxy : public BCLib::Network::CUdpProxy, public BCLib::Framework::CMsgExecMgr
{
public:
    CLogicProxy();
    virtual ~CLogicProxy();

    std::string getDebugPrompt();

protected:
    virtual bool _cbParseMsg(const BCLib::Network::CSockAddr& peerAddr, const void* msgBuff, BCLib::uint32 msgSize);

    virtual void _unhandledMsg(SFLib::CommonServer::SUdpMsgLabel& msgLabel, SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);
    virtual bool _transformMsg(PeerID peerID, SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);

    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::uint16 id, BCLib::Framework::CMsgExecPtr& msgExecPtr);
    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::Framework::CMsgExecPtr& msgExecPtr);
    virtual void _onXX2XSNtfServerType(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
};
typedef BCLib::Utility::CSPointer<CLogicProxy> CLoginProxyPtr;
}//Logic
}//SFLib

#endif//__SFLIB_LOGICSERVER_LOGICPROXY_H__
