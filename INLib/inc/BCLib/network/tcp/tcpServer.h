//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/tcp/tcpServer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_NETWORK_TCP_TCPSERVER_H__
#define __BCLIB_NETWORK_TCP_TCPSERVER_H__

#include <BCLib/network/sockAddr.h>
#include <BCLib/utility/uniqueID.h>
#include <BCLib/utility/spointer.h>


namespace BCLib
{
namespace Network
{
struct SConnCreateData;
class CTcpStub;
typedef BCLib::Utility::CSPointer<CTcpStub> CTcpStubSPtr;
class CTcpStubMgr;
class CNetAcceptorMgr;
class CTcpConnectionMgr;
class CNetStubPoolBase;
class CTcpConnection;
typedef BCLib::Utility::CSPointer<CTcpConnection> CTcpConnectionSPtr;


class BCLIB_NETWORK_API CTcpServer
{
    friend class CAcceptorCb;
    template<class> friend class CNetStubThread;
public:
    CTcpServer();
    virtual ~CTcpServer();

    CTcpStubMgr* getStubMgr() const;
    bool addAcceptor(const char* acceptorIP, BCLib::uint16 acceptPort, CEncrypt* encrypt = NULL, CCompress* compress = NULL);
    //void stopAllAcceptor();

protected:
    bool _setStubPool(BCLib::uint32 maxStub, BCLib::uint32 minThread, BCLib::uint32 maxStubPerThd);

    bool _init();
    BCLib::int32 _final();
    virtual CTcpStubMgr* _cbGetStubMgr() = 0;
    virtual CTcpStubSPtr _cbNewStub(const BCLib::Network::CTcpConnectionSPtr& connPtr) = 0;

private:
    bool _createTcpConnect(SOCK sock, const BCLib::Network::CSockAddr& addrAcceptor, const BCLib::Network::CSockAddr& addrPeer, SConnCreateData* connData);
    void _recycleStub(const CTcpStubSPtr& stubPtr);

private:
    BCLib::uint32 m_maxStub[1];
    BCLib::uint32 m_minThreadStub[1];
    BCLib::uint32 m_maxStubPerThd[1];

    CTcpStubMgr* m_netStubMgr;
    CNetAcceptorMgr* m_netAcceptorMgr;
    CNetStubPoolBase* m_stubPool[1];
    BCLib::Utility::CUniqueID64 m_uniqueID;
};
}//Network
}//BCLib

#endif//__BCLIB_NETWORK_TCP_TCPSERVER_H__
