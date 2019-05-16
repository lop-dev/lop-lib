//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SSLib/accountSet/accountServer/accountServer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SSLIB_ACCOUNTSET_ACCOUNTSERVER_ACCOUNTSERVER_H__
#define __SSLIB_ACCOUNTSET_ACCOUNTSERVER_ACCOUNTSERVER_H__

#include <BCLib/utility/pointer.h>
#include <BCLib/network/tcp/tcpServer.h>
#include <BCLib/framework/serviceID.h>
#include <BCLib/database/dbInfo.h>
#include <iostream>
#include <MWLib/misc/stringFilter.h>
#include <BCLib/framework/mainThread.h>
#include <BCLib/utility/timer.h>
#include <BCLib/utility/thread/thread.h>
namespace SSLib
{
namespace AccountServer
{
class CInterfaceInfo
{
public:
    CInterfaceInfo()
	:m_strServerUrl("")
	,m_strLocalIP("")
	,m_appID(0)
	,m_appKey(0)
    {
    }
    virtual ~CInterfaceInfo() {}

    void setServerUrl(std::string strServerUrl)
    {
        m_strServerUrl = strServerUrl;
    }
    std::string getServerUrl()
    {
        return m_strServerUrl;
    }

    void setLocalIP(std::string strLocalIP)
    {
        m_strLocalIP = strLocalIP;
    }
    std::string getLocalIP()
    {
        return m_strLocalIP;
    }

    void setAppID(BCLib::int32 appID)
    {
        m_appID = appID;
    }
    BCLib::int32 getAppID()
    {
        return m_appID;
    }

    void setAppKey(BCLib::int32 appKey)
    {
        m_appKey= appKey;
    }
    BCLib::int32 getAppKey()
    {
        return m_appKey;
    }
private:
    std::string m_strServerUrl;
    std::string m_strLocalIP;
    BCLib::int32 m_appID;
    BCLib::int32 m_appKey;
};
class CAccountServer : public BCLib::Framework::CMainThread, public BCLib::Network::CTcpServer
{
    BCLIB_SINGLETON_DECLARE(CAccountServer);

public:
    CAccountServer();
    virtual ~CAccountServer();
    bool hasUnLawKeyWord(const std::string& strName) const;
    std::string& getDyDBName() {return m_dyDBName;}
    CInterfaceInfo& getDyInterface() {return m_dyInterfaceInfo;}

protected:
    bool _initConfig();

public:
    static int main();

protected:
    virtual bool _init();
    virtual bool _callback();
    virtual int _final();
    virtual BCLib::Network::CTcpStubMgr* _cbGetStubMgr();
    virtual BCLib::Network::CTcpStubSPtr _cbNewStub(const BCLib::Network::CTcpConnectionSPtr& connPtr);
private:
    std::vector<BCLib::Database::CDBInfo*> m_vecDBInfo;
    MWLib::Misc::CStringFilter m_WordFilter;
    std::string m_dyDBName;
    CInterfaceInfo m_dyInterfaceInfo;
};

}//AccountServer
}//SSLib

#endif//__SSLIB_ACCOUNTSET_ACCOUNTSERVER_ACCOUNTSERVER_H__
