//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/externalServer/externalServer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_EXTERNALSERVER_EXTERNALSERVER_H__
#define __SFLIB_EXTERNALSERVER_EXTERNALSERVER_H__

#include <SFLib/commonServer/commonServer.h>
#include <BCLib/network/udp/udpServer.h>

namespace SFLib
{
namespace External
{
class CExternalServer : public SFLib::CommonServer::CCommonServer
{
public:
    CExternalServer(const std::string& name);
    virtual ~CExternalServer();

protected:
    virtual bool _cbSetAcceptInfo() = 0;

protected:
    virtual bool _init();

    virtual BCLib::Network::CTcpStubSPtr _cbNewStub(const BCLib::Network::CTcpConnectionSPtr& connPtr);

    virtual BCLib::Network::CUdpProxy* _createUdpProxy();

private:
    BCLib::Network::CUdpServer m_udpSrv;
};
}//External
}//SFLib

#endif//__SFLIB_EXTERNALSERVER_EXTERNALSERVER_H__
