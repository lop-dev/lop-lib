//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/tcp/tcpStub.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_NETWORK_TCP_TCPSTUB_H__
#define __BCLIB_NETWORK_TCP_TCPSTUB_H__

#include <BCLib/network/tcp/tcpSend.h>
#include <BCLib/network/sockAddr.h>
#include <BCLib/utility/memory/memPoolMgr.h>


namespace BCLib
{
namespace Network
{
class CTcpConnection;
typedef BCLib::Utility::CSPointer<CTcpConnection> CTcpConnectionSPtr;

enum EStubState
{
    ESS_NULL    = 0,    //无效状态
    ESS_VERIFY,         //连接验证状态
    ESS_OKAY,           //验证成功，进入正常处理状态
    ESS_RECYCLE,        //连接退出，准备回收
    ESS_MAX,
};

class BCLIB_NETWORK_API CTcpStub : public CTcpSend
{
    BCLIB_MEMPOLL_DEFINE();
    friend class CNetStubCallback;
    friend class CTcpServer;
    template<class> friend class CNetStubThread;
public:
    CTcpStub(const CTcpConnectionSPtr& connPtr);
    virtual ~CTcpStub();

    TcpStubID getStubID() const;
    TcpConnectID getConnectID() const;

    /// @brief 判断是否已有网络连接
    /// @return bool
    bool hasConnection() const;

    /// @brief 判断网络连接是否有效
    /// @return bool
    bool isConnectionValid() const;

    const CSockAddr& getLocalAddr() const;
    const CSockAddr& getPeerAddr() const;
    std::string getMAC() const;
    BCLib::uint32 getUnSendDataSize() const;

    EStubState getState() const;
    bool setState(EStubState state);

    bool setUsrKey(BCLib::uint64 usrKey);
    BCLib::uint64 getUsrKey() const;

    bool setSendDelay(BCLib::uint32 delay, BCLib::uint32 delayDiff);
    void setRTT(bool bOpen);
    bool isOpenRTT() const;
    BCLib::uint32 getRTTInMS() const;
    void openNetTime();
    bool isOpenNetTime() const;
    BCLib::int64 getConnectTimeInSec() const;
    BCLib::uint64 getTotalSendByte() const;
    BCLib::uint64 getTotalRecvByte() const;
    float getSendRateInKBPerSec() const;
    float getRecvRateInKBPerSec() const;

    BCLib::int32 send(BCLib::Utility::CStream& stream);
    BCLib::int32 send(const void* msgBuf, BCLib::uint32 bufSize);

protected:
    void _setStubID(TcpStubID stubID);
    virtual bool _cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize) = 0;
    virtual void _cbTerminate();

private:
    TcpStubID m_stubID;
    EStubState m_stubState;
    CTcpConnectionSPtr m_connPtr;
};
typedef BCLib::Utility::CSPointer<CTcpStub> CTcpStubSPtr;
}//Network
}//BCLib

#endif//__BCLIB_NETWORK_TCP_TCPSTUB_H__
