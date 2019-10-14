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
class CLogicClient : public CCommonClient
{
public:
    CLogicClient();
    virtual ~CLogicClient();
};
}//CommonServer
}//SFLib

#endif //__SFLIB_COMMONSERVER_NETCLIENT_LOGICCLIENT_H__