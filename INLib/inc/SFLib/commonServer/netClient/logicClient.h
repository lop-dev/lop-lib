//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/netClient/logicClient.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_NETCLIENT_LOGICCLIENT_H__
#define __SFLIB_COMMONSERVER_NETCLIENT_LOGICCLIENT_H__

#include <SFLib/commonServer/netClient/commonClient.h>

namespace SFLib
{
namespace CommonServer
{
class SFLIB_COMMON_API CLogicClient : public CCommonClient
{
public:
    CLogicClient();
    virtual ~CLogicClient();

protected:
    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::uint16 id, BCLib::Framework::CMsgExecPtr& msgExecPtr);
    virtual void _onXX2XSNtfServerType(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);
};
}//CommonServer
}//SFLib

#endif //__SFLIB_COMMONSERVER_NETCLIENT_LOGICCLIENT_H__