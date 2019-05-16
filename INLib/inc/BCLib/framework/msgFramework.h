//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/msgFramework.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_MSGFRAMEWORK_H__
#define __BCLIB_FRAMEWORK_MSGFRAMEWORK_H__

#include <BCLib/framework/msgType.h>
#include <BCLib/framework/message.h>
#include <BCLib/framework/msgDebug.h>
#include <BCLib/framework/serviceID.h>
#include <BCLib/framework/service.h>
#include <BCLib/utility/strHelper.h>

namespace BCLib
{

namespace Message
{

namespace Framework
{

enum EMsgIDThreadFrame
{
    EMID_THDFRM_NTF_TERMINATE = 1,
    EMID_THDFRM_NTF_SERVICE_ENTER,
    EMID_THDFRM_REQ_CALL_SERVICE,
    EMID_THDFRM_RES_CALL_SERVICE,
};

#if defined(_WIN32)

    #pragma pack (push, 1)

#elif defined(_LINUX)

    #pragma pack (1)

#endif

//通知ThreadFrame初始化Service
BCLIB_MSGDEBUG(BCLib::Message::EMT_FRAMEWORK, EMID_THDFRM_NTF_TERMINATE);
struct SMsgThdFrmNtfTerminate: public BCLib::Framework::SMessage
{
    SMsgThdFrmNtfTerminate() : SMessage(BCLib::Message::EMT_FRAMEWORK, EMID_THDFRM_NTF_TERMINATE){}
};

BCLIB_MSGDEBUG(BCLib::Message::EMT_FRAMEWORK, EMID_THDFRM_NTF_SERVICE_ENTER);
struct SMsgThdFrmNtfServiceEnter : public BCLib::Framework::SMessage
{
    BCLib::Framework::EServiceType m_serviceType;
    BCLib::Framework::CServiceID m_serviceID;
    char m_serviceName[BCLIB_FW_SERVICE_NAME_MAX+1];

    SMsgThdFrmNtfServiceEnter() : SMessage(BCLib::Message::EMT_FRAMEWORK, EMID_THDFRM_NTF_SERVICE_ENTER), m_serviceType(BCLib::Framework::ESTYPE_GENERAL), m_serviceID(0)
    {
        memset(m_serviceName, 0, sizeof(m_serviceName));
    }

    void setServiceName(const std::string& serviceName)
    {
        BCLib::Utility::CStrHelper::strNCpy(m_serviceName, sizeof(m_serviceName), serviceName.c_str(), BCLIB_FW_SERVICE_NAME_MAX);
    }
};

BCLIB_MSGDEBUG(BCLib::Message::EMT_FRAMEWORK, EMID_THDFRM_REQ_CALL_SERVICE);
struct SMsgThdFrmReqCallService : public BCLib::Framework::SMessage
{
    BCLib::Framework::CServiceID m_requestServiceID;
    BCLib::Framework::EServiceZone m_serviceZone;
    char m_serviceName[BCLIB_FW_SERVICE_NAME_MAX+1];

    SMsgThdFrmReqCallService() : SMessage(BCLib::Message::EMT_FRAMEWORK, EMID_THDFRM_REQ_CALL_SERVICE)
    {
        memset(m_serviceName, 0, sizeof(m_serviceName));
    }

    void setServiceName(const std::string& serviceName)
    {
        BCLib::Utility::CStrHelper::strNCpy(m_serviceName, sizeof(m_serviceName), serviceName.c_str(), BCLIB_FW_SERVICE_NAME_MAX);
    }
};

BCLIB_MSGDEBUG(BCLib::Message::EMT_FRAMEWORK, EMID_THDFRM_RES_CALL_SERVICE);
struct SMsgThdFrmResCallService : public BCLib::Framework::SMessage
{
    BCLib::Framework::CServiceID m_targetServiceID;
    BCLib::Framework::CServiceID m_requestServiceID;
    char m_serviceName[BCLIB_FW_SERVICE_NAME_MAX+1];

    SMsgThdFrmResCallService() : SMessage(BCLib::Message::EMT_FRAMEWORK, EMID_THDFRM_RES_CALL_SERVICE), m_targetServiceID(0), m_requestServiceID(0)
    {
        memset(m_serviceName, 0, sizeof(m_serviceName));
    }

    void setServiceName(const std::string& serviceName)
    {
        BCLib::Utility::CStrHelper::strNCpy(m_serviceName, sizeof(m_serviceName), serviceName.c_str(), BCLIB_FW_SERVICE_NAME_MAX);
    }
};

#if defined(_WIN32)

    #pragma pack (pop)

#elif defined(_LINUX)

    #pragma pack ()

#endif

}//Framework

}//Message

}//BCLib

#endif//__BCLIB_FRAMEWORK_MSGFRAMEWORK_H__
