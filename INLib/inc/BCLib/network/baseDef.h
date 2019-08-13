//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/baseDef.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_NETWORK_BASEDEF_H__
#define __BCLIB_NETWORK_BASEDEF_H__

#include <BCLib/utility/baseDef.h>


//#define BCLIB_NETWORK_DEBUG     //! 开关网络调试的宏
//#define BCLIB_USE_MEMPOLL       //! 开关内存池的宏
//#define BCLIB_USE_MEMMALLOC     //! 开关内存分配的宏


#if !defined(_WIN32) && !defined(_LINUX)
#error "Unknown platform !!"
#endif

#if defined(_WIN32)
//#include <WinSock2.h>
#elif defined(_LINUX)
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#if defined(BCLIB_NETWORK_EXPORTS)
#define BCLIB_NETWORK_API _declspec(dllexport)
#elif defined(BCLIB_NETWORK_IMPORTS)
#define BCLIB_NETWORK_API _declspec(dllimport)
#else
#define BCLIB_NETWORK_API
#endif

namespace BCLib
{
namespace Network
{
enum ENetType
{
    NETT_NULL = -1,
    NETT_MIN = NETT_NULL,
    NETT_TCP,
    NETT_UDP,
    NETT_MAX,
};

#if defined(_WIN32)
typedef SOCKET SOCK;
const SOCK INVALID_SOCK = INVALID_SOCKET;
const BCLib::int32 SOCK_ERROR = SOCKET_ERROR;
const BCLib::int32 SOCK_CLOSE = -2;
#elif defined(_LINUX)
typedef int SOCK;
const SOCK INVALID_SOCK = -1;
const BCLib::int32 SOCK_ERROR = -1;
const BCLib::int32 SOCK_CLOSE = -2;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
#endif

const BCLib::uint16 INVALID_PORT = BCLib::uint16(-1);

typedef BCLib::uint16 TcpConnectID;
const TcpConnectID INVALID_TCPCONNECTID = 0;

typedef BCLib::uint16 TcpStubID;
const TcpStubID INVALID_TCPSTUBID = INVALID_TCPCONNECTID;

typedef BCLib::uint32 PipeID;
const PipeID INVALID_PIPEID = 0;

typedef BCLib::uint32 PipeStubID;
const PipeStubID INVALID_PIPESTUBID = INVALID_PIPEID;

const BCLib::uint32 MAX_SOCKEVENT = 16;

const BCLib::uint32 DEF_MSG_BUF_SIZE = 2*1024;      //! byte
const BCLib::uint32 TCP_RCV_BUF_SIZE = 64*1024;     //! byte

const BCLib::uint32 UDP_PACKET_SIZE = 1472;         //! byte
const BCLib::uint32 UDP_PACKET_RESEND_CNT = 10;

const BCLib::uint32 PIPE_FILENAME_LEN = 255;
const BCLib::uint32 PIPE_BUF_SIZE = 1024;           //! byte


class CEncrypt
{
public:
    virtual ~CEncrypt()
    {
    }
    //! 多个连接共用一个对象,需要保证线程安全
    virtual BCLib::int32 encrypt(const void* pInData, BCLib::int32 iInSize, void* pOutBuff, BCLib::int32 iBuffSize, bool isEncrypt) = 0;
};

class CCompress
{
public:
    virtual ~CCompress()
    {
    }
    //! 多个连接共用一个对象,需要保证线程安全
    virtual BCLib::int32 compress(const void* pInData, BCLib::int32 iInSize, void* pOutBuff, BCLib::int32 iBuffSize, bool isCompress) = 0;
};
}//Network
}//BCLib

#endif//__BCLIB_NETWORK_BASEDEF_H__
