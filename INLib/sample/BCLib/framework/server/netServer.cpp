//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/netfw/server/netServer.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/logFile.h>
#include "netServer.h"
#include "netStubMgr.h"
#include "netStub.h"

CTcpServer::CTcpServer()
{
    ;
}

CTcpServer::~CTcpServer()
{
    ;
}

bool CTcpServer::init()
{
    return BCLib::Network::CTcpServer::_init();
}

void CTcpServer::final()
{
    BCLib::Network::CTcpServer::_final();
}

bool CTcpServer::setStubPool(BCLib::uint32 maxStub, BCLib::uint32 minThread, BCLib::uint32 maxStubPerThd)
{
    return BCLib::Network::CTcpServer::_setStubPool(maxStub, minThread, maxStubPerThd);
}

BCLib::Network::CTcpStubMgr* CTcpServer::_cbGetStubMgr()
{
    return &CTcpStubMgr::singleton();
}

BCLib::Network::CTcpStubSPtr CTcpServer::_cbNewStub(const BCLib::Network::CTcpConnectionSPtr& connPtr)
{
    return new CTcpStub(connPtr);
}