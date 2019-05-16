//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/tcp/tcpStubMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_NETWORK_TCP_TCPSTUBMGR_H__
#define __BCLIB_NETWORK_TCP_TCPSTUBMGR_H__

#include <BCLib/utility/thread/mutex.h>
#include <BCLib/utility/hashMap.h>
#include <BCLib/network/tcp/tcpStub.h>


namespace BCLib
{
namespace Network
{
class BCLIB_NETWORK_API CTcpStubMgr
{
public:
    class CStubMgrCallback
    {
    public:
        virtual ~CStubMgrCallback()
        {
        }
        virtual bool execute(const CTcpStubSPtr& stubPtr) = 0;
    };

public:
    CTcpStubMgr();
    virtual ~CTcpStubMgr();

    bool addStub(const CTcpStubSPtr& stubPtr);
    bool delStub(const CTcpStubSPtr& stubPtr);
    bool delStub(TcpStubID stubID);
    CTcpStubSPtr getStubByStubID(TcpStubID stubID);
    CTcpStubSPtr getStubByConnectID(TcpConnectID connID);
	BCLib::uint32 getStubNum(){ return m_stubHashMap.size(); }
    void traversal(CStubMgrCallback& cb);

    bool sendMsg(TcpStubID stubID, BCLib::Utility::CStream& stream);
    bool sendMsg(TcpStubID stubID, const void* msgBuff, BCLib::uint32 buffSize);

private:
    BCLib::Utility::CMutex m_mutex;
    BCLib::Utility::CHashMap<TcpStubID, CTcpStubSPtr> m_stubHashMap;
    BCLib::Utility::CHashMap<TcpConnectID, CTcpStubSPtr> m_connectHashMap;
};
}//Network
}//BCLib

#endif//__BCLIB_NETWORK_TCP_TCPSTUBMGR_H__
