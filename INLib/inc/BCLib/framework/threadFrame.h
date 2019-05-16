//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/threadFrame.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_THREADFRAME_H__
#define __BCLIB_FRAMEWORK_THREADFRAME_H__

#include <BCLib/utility/hashMap.h>
#include <BCLib/framework/msgExecMgr.h>
#include <BCLib/framework/serviceID.h>
#include <BCLib/framework/service.h>

namespace BCLib
{
namespace Framework
{
struct SThreadInfo;
class CServiceMgr;


struct SRequestServiceInfor
{
    CServiceID m_requestServiceID;
    std::string m_requestServiceName;
    EServiceZone m_serviceZone;

    SRequestServiceInfor()
    :m_requestServiceID(0)
    ,m_serviceZone(ESZONE_THREAD)
    {
        ;
    }
};

class BCLIB_FRAMEWORK_API CThreadFrame : public CMsgExecMgr
{
public:
    CThreadFrame();
    virtual ~CThreadFrame();

    const std::string& getName()
    {
        return m_name;
    }
    void setName(const std::string& name)
    {
        m_name = name;
    }

    bool isTerminate()
    {
        return m_isTerminate;
    }
    bool isValid()
    {
        return m_isValid;
    }

    void terminate();
    void setValid(bool valid);

    bool sendMsgToSelf(const CMessage* msg, uint32 sessionID = 0);
    bool sendMsgToSelf(const SMessage* msg, uint32 msgSize, uint32 sessionID = 0);

    bool sendMsgToSelf(const CMessage* msg, SThdMsgLabel* msgLabel);
    bool sendMsgToSelf(const SMessage* msg, uint32 msgSize, SThdMsgLabel* msgLabel);

    CCtrlID getCtrlID();
    SThreadInfo* getThreadInfor()
    {
        return m_threadInfo;
    }

public:
    virtual bool isMainThread()
    {
        return false;
    }

public:
    static SThreadInfo* getSelfThreadInfo();

protected:
    virtual int _main();
    virtual bool _init();
    virtual int _final();
    virtual bool _callback();
    virtual bool _recvMsg(bool isAsync);
    virtual bool _loadServices();

    bool _addService(CService* service);
    bool _delService(const CServiceID& serviceID);
    void _removeAllServices();

    void _setMsgBuffSize(uint32 msgBuffSize)
    {
        m_msgBuffSize = msgBuffSize;
    }

protected:
    bool _sendMsgToSelf(const CMessage* msg, uint32 sessionID = 0);
    bool _sendMsgToSelf(const SMessage* msg, uint32 msgSize, uint32 sessionID = 0);

    bool _sendMsgToSelf(const CMessage* msg, SThdMsgLabel* msgLabel);
    bool _sendMsgToSelf(const SMessage* msg, uint32 msgSize, SThdMsgLabel* msgLabel);

    bool _sendMsgToALlCtrlInProcess(const CMessage* msg, uint32 sessionID = 0);
    bool _sendMsgToALlCtrlInProcess(const SMessage* msg, uint32 msgSize, uint32 sessionID = 0);

    bool _sendMsgToALlCtrlInMachine(const CMessage* msg, uint32 sessionID = 0);
    bool _sendMsgToALlCtrlInMachine(const SMessage* msg, uint32 msgSize, uint32 sessionID = 0);

    bool _sendMsgToService(const CServiceID& toService, const CMessage* msg, uint32 sessionID = 0);
    bool _sendMsgToService(const CServiceID& toService, const SMessage* msg, uint32 msgSize, uint32 sessionID = 0);

    bool _sendMsgToCtrlNoBuff(const CCtrlID& toCtrl, uint16 type, uint32 param);
    bool _sendMsgToAllCtrlNoBuff(uint16 type, uint32 param);

protected:
    virtual bool _createMsgExecPtr(uint16 type, uint16 id, CMsgExecPtr& msgExecPtr);
    virtual bool _createMsgExecPtr(uint16 type, CMsgExecPtr& msgExecPtr);

protected:
    virtual void _onNtfTerminate(SThdMsgLabel* msgLabel, SMessage* msg);
    virtual void _onNtfServiceEnter(SThdMsgLabel* msgLabel, SMessage* msg);
    virtual void _onReqCallService(SThdMsgLabel* msgLabel, SMessage* msg);
    virtual void _onResCallService(SThdMsgLabel* msgLabel, SMessage* msg);

private:
    bool _requestService(const CServiceID& requestService, const std::string& name, EServiceZone serviceZone);
    void _parseMsgNonBuff(const CServiceID& senderService, uint16 type, uint64 param);
    bool _setRequestServiceInfor(const CServiceID& requestService, const std::string& name, EServiceZone serviceZone);
    void _notifyServiceLeave(const CServiceID& leaveServiceID);

private:
    bool m_isTerminate;
    bool m_isValid;
    std::string m_name;
    uint32 m_msgBuffSize;
    SThreadInfo* m_threadInfo;
    std::vector<SRequestServiceInfor*> m_requestServiceList;

    friend class CServiceMgr;
    friend class CService;
};
}//Framework
}//BCLib

#endif//__BCLIB_FRAMEWORK_THREADFRAME_H__
