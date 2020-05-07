//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/p2p/p2pCommon/baseDef.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_P2P_P2PCOMMON_BASEDEF_H__
#define __BCLIB_P2P_P2PCOMMON_BASEDEF_H__

#include <BCLib/utility/baseDef.h>


//#define BCLIB_P2P_DEBUG         //! 开关P2P调试的宏
//#define BCLIB_USE_MEMPOLL       //! 开关内存池的宏


#if !defined(_WIN32) && !defined(_LINUX)
#error "Unknown platform !!"
#endif

#if defined(_WIN32)
//#include <WinSock2.h>
#elif defined(_LINUX)
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#if defined(BCLIB_P2P_EXPORTS)
#define BCLIB_P2P_API _declspec(dllexport)
#elif defined(BCLIB_P2P_IMPORTS)
#define BCLIB_P2P_API _declspec(dllimport)
#else
#define BCLIB_P2P_API
#endif

namespace BCLib
{
namespace P2p
{
#if defined(_WIN32)
typedef SOCKET SOCK;
const SOCK INVALID_SOCK = INVALID_SOCKET;
const BCLib::int32 SOCK_ERROR = SOCKET_ERROR;
#elif defined(_LINUX)
typedef int SOCK;
const SOCK INVALID_SOCK = -1;
const BCLib::int32 SOCK_ERROR = -1;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
#endif

const BCLib::uint16 INVALID_PORT = BCLib::uint16(-1);

typedef BCLib::uint64 UdpConnectID;
const UdpConnectID INVALID_UDPCONNECTID = 0;

typedef BCLib::uint64 PeerID;
const PeerID INVALID_PEERID = 0;

const BCLib::int32 DEF_MSG_BUF_SIZE = 2*1024;       //! byte
const BCLib::int32 UDP_PACKET_SIZE = 1472;          //! byte
}//P2p
}//BCLib

#endif//__BCLIB_P2P_P2PCOMMON_BASEDEF_H__
