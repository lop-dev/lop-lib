//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SSLib/accountSet/accountServer/network/netStub.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SSLIB_ACCOUNTSET_ACCOUNTSERVER_NETWORK_NETSTUB_H__
#define __SSLIB_ACCOUNTSET_ACCOUNTSERVER_NETWORK_NETSTUB_H__

#include <BCLib/utility/pointer.h>
#include <BCLib/network/tcp/tcpStub.h>
#include <BCLib/framework/message.h>
#include <BCLib/framework/msgExecMgr.h>
#include <SSLib/accountSet/accountServer/protobuf/accountAuth.pb.h>

namespace SSLib
{
namespace AccountServer
{

class CTcpServer;
class CNetService;

class CTcpStub : public BCLib::Network::CTcpStub, BCLib::Framework::CMsgExecMgr
{
public:
    CTcpStub(const BCLib::Network::CTcpConnectionSPtr& netConn);
    virtual ~CTcpStub();

    bool sendMsg(const BCLib::Framework::CMessage& msg);
    bool sendMsg(const BCLib::Framework::SMessage* msg, const BCLib::uint32 msgSize);

	void setAccountAuth(PTBuf::CAccountAuth& refAccountAuth) {m_AccountAuth = refAccountAuth;}

protected:
    virtual bool _cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize);
    virtual void _cbTerminate();

    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::uint16 id, BCLib::Framework::CMsgExecPtr& msgExecPtr);
    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::Framework::CMsgExecPtr& msgExecPtr);

private:
    void _onC2SReqAccountSetESSOkay(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

    void _onC2SReqLogin(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

    void _onC2SReqInsertAccountAuth(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    void _onC2SReqSelectAccountAuth(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    void _onC2SReqUpdateAccountAuth(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    void _onC2SReqDeleteAccountAuth(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

    void _onEXS2SReqQueryAccount(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    void _onSS2ASReqQueryAccount(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

    void _onC2SReqVerifyNamePwd(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
	void _onC2SReqInsertNamePwd(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

private:
	PTBuf::CAccountAuth m_AccountAuth;
};
typedef BCLib::Utility::CSPointer<CTcpStub> CNetStubPtr;

}//AccountServer
}//SSLib
#endif//__SSLIB_ACCOUNTSET_ACCOUNTSERVER_NETWORK_NETSTUB_H__
