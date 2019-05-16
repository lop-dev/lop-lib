//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/service.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_SERVICE_H__
#define __BCLIB_FRAMEWORK_SERVICE_H__

#include <string>
#include <BCLib/utility/noncopyable.h>
#include <BCLib/utility/pointer.h>
#include <BCLib/framework/msgExecMgr.h>
#include <BCLib/framework/serviceID.h>

namespace BCLib
{
namespace Framework
{
class CThreadFrame;
class CServiceMgr;

enum EServiceZone
{
    ESZONE_THREAD,
    ESZONE_PROCESS,
    ESZONE_MACHINE,
    ESZONE_SYSTEM,
    ESZONE_ALL,
};

enum EServiceType
{
    ESTYPE_GENERAL  = 0,
    ESTYPE_TRANSMIT,
    ESTYPE_MAP_SYSTME,
    ESTYPE_MAP_OUTSYS,
};

class BCLIB_FRAMEWORK_API CService : public CMsgExecMgr, public Utility::CNoncopyable
{
public:
    CService(const std::string& name);
    virtual ~CService();

    virtual bool init() = 0;
    virtual bool final() = 0;

    const std::string& getName()
    {
        return m_serviceName;
    }
    CServiceID getServicID()
    {
        return m_serviceID;
    }

    virtual EServiceType getType()
    {
        return ESTYPE_GENERAL;
    }

    bool sendMsgToSelf(const CMessage* msg, uint32 sessionID = 0);
    bool sendMsgToSelf(const SMessage* msg, uint32 msgSize, uint32 sessionID = 0);

protected:
    bool _sendMsgToSelf(const CMessage* msg, SThdMsgLabel* msgLabel);
    bool _sendMsgToSelf(const SMessage* msg, uint32 msgSize, SThdMsgLabel* msgLabel);

    bool _sendMsgToService(const CServiceID& toService, const CMessage* msg, uint32 sessionID = 0);
    bool _sendMsgToService(const CServiceID& toService, const SMessage* msg, uint32 msgSize, uint32 sessionID = 0);

    bool _sendMsgToService(const CServiceID& fromService, const CServiceID& toService, const CMessage* msg, uint32 sessionID = 0);
    bool _sendMsgToService(const CServiceID& fromService, const CServiceID& toService, const SMessage* msg, uint32 msgSize, uint32 sessionID = 0);

    bool _requestService(const std::string& name, EServiceZone serviceZone);

protected:
    virtual void _cbRequestServiceEnter(const CServiceID& targetService, const std::string& name, EServiceZone serviceZone);
    virtual void _cbServiceLeave(const CServiceID& leaveService);

protected:
    virtual bool _createMsgExecPtr(uint16 type, uint16 id, CMsgExecPtr& msgExecPtr);
    virtual bool _createMsgExecPtr(uint16 type, CMsgExecPtr& msgExecPtr);
    void _sedName(const std::string& name)
    {
        m_serviceName = name;
    }

private:
    std::string m_serviceName;
    CServiceID m_serviceID;
    CThreadFrame* m_threadFrame;

    friend class CThreadFrame;
    friend class CServiceMgr;
};

// 动态库接口函数
typedef CService* (*CREACE_SERVICE_FUN)(const char* serviceName);
#define CREATESERVICE "createService"
}//Framework
}//BCLib

#endif//__BCLIB_FRAMEWORK_SERVICE_H__
