//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/commonServer2Master.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_COMMONSERVER2MASTER_H__
#define __SFLIB_COMMONSERVER_COMMONSERVER2MASTER_H__

#include <SFLib/commonServer/commonServer.h>
#include <SFLib/commonServer/netClient/masterClient.h>

namespace SFLib
{
namespace CommonServer
{
#ifndef USE_PIPESERVER_TEST_LOGIC

class CCommonServer2Master : public CCommonServer
{
public:
    CCommonServer2Master(const std::string& name);
    virtual ~CCommonServer2Master();

    static CCommonServer2Master* getCommonServer2Master()
    {
        return (CCommonServer2Master *)CCommonServer::getCommonServer();
    }

    virtual GroupID getGroupID();

    CMasterClientPtr getMasterClient()
    {
        return (CMasterClientPtr)m_masterClient;
    }

    void ntfInitSuccess();

	bool sendMsgToMS(PeerID peerID, const SFLib::Message::CNetMessage* msg);
	bool sendMsgToMS(PeerID peerID, const SFLib::Message::SNetMessage* msg, const BCLib::uint32 msgSize);

    bool checkMasterServer();

protected:
    virtual bool _init();
    virtual int _final();

    virtual CTcpClientPtr _createMasterClient();

    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::uint16 id, BCLib::Framework::CMsgExecPtr& msgExecPtr);
    virtual void _onMS2XSNtfServerActive(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onMS2XSNtfServerInited(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

private:
    bool _connectMasterServer(BCLib::uint16 uCount = BCLIB_UINT16_MAX);
    bool _verifyMasterClient();

protected:
    CTcpClientPtr m_masterClient;
};

#else

class CCommonServer2Master : public CCommonServer
{
public:
    CCommonServer2Master(const std::string& name);
    virtual ~CCommonServer2Master();

    static CCommonServer2Master* getCommonServer2Master()
    {
        return (CCommonServer2Master *)CCommonServer::getCommonServer();
    }

    virtual GroupID getGroupID();

    CMasterClientPtr getMasterClient()
    {
        return (CMasterClientPtr)m_masterClient;
    }

    void ntfInitSuccess();

protected:
    virtual bool _init();
    virtual int _final();

    virtual CTcpClientPtr _createMasterClient();

    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::uint16 id, BCLib::Framework::CMsgExecPtr& msgExecPtr);
    virtual void _onMS2XSNtfServerActive(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
    virtual void _onMS2XSNtfServerInited(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

protected:
    CTcpClientPtr m_masterClient;
};
#endif
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_COMMONSERVER2MASTER_H__
