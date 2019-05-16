//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/gameClient/udpClient.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_GAMECLIENT_UDPCLIENT_H__
#define __SFLIB_GAMECLIENT_UDPCLIENT_H__

#include <BCLib/network/udp/udpClient.h>
#include <BCLib/framework/msgExecMgr.h>

namespace SFLib
{
namespace Client
{

class CUdpClient : public BCLib::Network::CUdpClient, public BCLib::Framework::CMsgExecMgr
{
public:
    CUdpClient();
    virtual ~CUdpClient();

    std::string getDebugPrompt();

protected:
    virtual bool _cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize);
    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::uint16 id, BCLib::Framework::CMsgExecPtr& msgExecPtr);
    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::Framework::CMsgExecPtr& msgExecPtr);
};

}//Client
}//SFLib

#endif//__SFLIB_GAMECLIENT_UDPCLIENT_H__
