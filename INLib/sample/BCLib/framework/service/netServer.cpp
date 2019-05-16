//////////////////////////////////////////////////////////////////////
//  created:    2012/08/01
//  filename:   BCLib/framework/netfw/server/netServer.cpp
//  author:     aaron
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/logFile.h>
#include "netServer.h"
#include "netStubMgr.h"
#include "netStub.h"

CNetServer::CNetServer()
{
    ;
}

CNetServer::~CNetServer()
{
    ;
}

bool CNetServer::init()
{
    return BCLib::Network::CNetServer::_init();
}

void CNetServer::final()
{
    BCLib::Network::CNetServer::_final();
}

bool CNetServer::setStubPool(BCLib::Network::ENetType netType, BCLib::uint32 maxStub, BCLib::uint32 minThread, BCLib::uint32 maxStubPerThd)
{
    return BCLib::Network::CNetServer::_setStubPool(netType, maxStub, minThread, maxStubPerThd);
}

BCLib::Network::CNetStubMgr* CNetServer::_cbGetNetStubMgr()
{
    return &CNetStubMgr::singleton();
}

BCLib::Network::CNetStub* CNetServer::_cbNewStub(BCLib::Network::ENetType netType, const BCLib::Network::CNetConnectionSPtr& connPtr)
{
    switch(netType)
    {
    case BCLib::Network::NETT_TCP:
        {
            BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT,
                "Á¬½Ó[%s:%d] <----- [%s:%d]",
                connPtr->getLocalAddr().getIP().c_str(),
                connPtr->getLocalAddr().getPort(),
                connPtr->getPeerAddr().getIP().c_str(),
                connPtr->getPeerAddr().getPort());
            return new CNetStub(connPtr);
        }
        break;
    default:
        return NULL;
        break;
    }
}