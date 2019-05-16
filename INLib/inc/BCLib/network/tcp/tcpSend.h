//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/tcp/tcpSend.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_NETWORK_TCP_TCPSEND_H__
#define __BCLIB_NETWORK_TCP_TCPSEND_H__

#include <BCLib/network/baseDef.h>
#include <BCLib/utility/spointer.h>
#include <BCLib/utility/stream.h>


namespace BCLib
{
namespace Network
{
class BCLIB_NETWORK_API CTcpSend : public BCLib::Utility::CRefCnt
{
public:
    virtual ~CTcpSend()
    {
    }

    virtual BCLib::int32 send(BCLib::Utility::CStream& stream) = 0;
    virtual BCLib::int32 send(const void* msgBuf, BCLib::uint32 bufSize) = 0;
};
typedef BCLib::Utility::CSPointer<CTcpSend> CTcpSendSPtr;
}//Network
}//BCLib

#endif//__BCLIB_NETWORK_TCP_TCPSEND_H__
