//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/keyExecMgr/keyExecMgr.cpp
//  author:     jeff
/// @brief      key -> func
///
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <vector>
#include <BCLib/utility/keyExecMgr.h>

enum EEventId
{
    EEVENT_UNDEFINED,
    EEVENT_1,
    EEVENT_2,
    EEVENT_3
};

class CEventParams
{
public:
    virtual ~CEventParams()
    {
    }

    virtual EEventId GetEventType()
    {
        return EEVENT_UNDEFINED;
    }
};

class CEvent1Params : public CEventParams
{
public:
    virtual ~CEvent1Params()
    {
    }

    virtual EEventId GetEventType()
    {
        return EEVENT_1;
    }

    int m_iParam;
};

class CEvent2Params : public CEventParams
{
public:
    virtual ~CEvent2Params()
    {
    }

    virtual EEventId GetEventType()
    {
        return EEVENT_2;
    }

    int m_iParam;
    std::string m_value;
};

class CEvent3Params : public CEventParams
{
public:
    virtual ~CEvent3Params()
    {
    }

    virtual EEventId GetEventType()
    {
        return EEVENT_3;
    }

    bool m_bParam;
    int m_iParam;
    std::string m_value;
};

// Process Event1
void processEvent1(CEventParams& param)
{
    if(param.GetEventType() != EEVENT_1)
    {
        std::cout << "[processEvent1] WRONG Event Type!" << std::endl;
        return;
    }

    CEvent1Params& event1 = (CEvent1Params&)param;
    std::cout << "C-Style Func Process Event 1: " << event1.m_iParam << std::endl;
}

// Process Event2
class CEvent2Handler
{
public:
    void processEvent2(CEventParams& param)
    {
        if(param.GetEventType() != EEVENT_2)
        {
            std::cout << "[processEvent2] WRONG Event Type!" << std::endl;
            return;
        }

        CEvent2Params& event2 = (CEvent2Params&)param;
        std::cout << "Class Member Func Process Event 2: " << event2.m_iParam << ", " << event2.m_value << std::endl;
    }
};

// Process Event3
void processEvent3(CEventParams& param)
{
    if(param.GetEventType() != EEVENT_3)
    {
        std::cout << "[processEvent3] WRONG Event Type!" << std::endl;
        return;
    }

    CEvent3Params& event3 = (CEvent3Params&)param;
    std::cout << "C-Sytle Func Process Event 3: " << event3.m_bParam << "," << event3.m_iParam << "," << event3.m_value << std::endl;
}

class CEvent3Handler
{
public:
    void processEvent3(CEventParams& param)
    {
        if(param.GetEventType() != EEVENT_3)
        {
            std::cout << "[processEvent3] WRONG Event Type!" << std::endl;
            return;
        }

        CEvent3Params& event3 = (CEvent3Params&)param;
        std::cout << "Class Member Func Process Event 3: " << event3.m_bParam << "," << event3.m_iParam << "," << event3.m_value << std::endl;
    }
};

class CEvent3StaticHandler
{
public:
    static void processEvent3(CEventParams& param)
    {
        if(param.GetEventType() != EEVENT_3)
        {
            std::cout << "[processEvent3] WRONG Event Type!" << std::endl;
            return;
        }

        CEvent3Params& event3 = (CEvent3Params&)param;
        std::cout << "Class Static Member Func Process Event 3: " << event3.m_bParam << "," << event3.m_iParam << "," << event3.m_value << std::endl;
    }
};


int main()
{
    BCLib::Utility::CKeyExecMgr<BCLib::uint32, CEventParams> eventManager;

    CEvent2Handler event2Handler;
    CEvent3Handler event3Handler;

    // Event1
    CEvent1Params e1Param;
    e1Param.m_iParam = 100;
    CEvent2Params e2Param;
    e2Param.m_iParam = 200;
    e2Param.m_value = "Event 2";
    CEvent3Params e3Param;
    e3Param.m_bParam = false;
    e3Param.m_iParam = 300;
    e3Param.m_value = "Event 3";

    // Test Empty Handler
    eventManager.dispatchParams(EEVENT_1, e1Param);
    eventManager.dispatchParams(EEVENT_2, e2Param);
    eventManager.dispatchParams(EEVENT_3, e3Param);

    // Test Event1
    std::cout << "Test Event1 ==> " << std::endl;
    eventManager.registerFunc(EEVENT_1, &processEvent1);
    eventManager.dispatchParams(EEVENT_1, e1Param);
    eventManager.dispatchParams(EEVENT_1, e2Param);   // wrong params
    std::cout << std::endl;

    // Test Event2
    std::cout << "Test Event2 ==>" << std::endl;
    eventManager.registerFunc<CEvent2Handler>(EEVENT_2, &CEvent2Handler::processEvent2, &event2Handler);
    eventManager.dispatchParams(EEVENT_2, e2Param);
    std::cout << std::endl;


    // Test Event3
    std::cout << "Test Event3 ==>" << std::endl;
    eventManager.registerFunc<CEvent3Handler>(EEVENT_3, &CEvent3Handler::processEvent3, &event3Handler);
    eventManager.dispatchParams(EEVENT_3, e3Param);

    std::cout << std::endl;
    eventManager.registerFunc(EEVENT_3, &processEvent3);
    eventManager.registerFunc(EEVENT_3, &CEvent3StaticHandler::processEvent3);
    eventManager.dispatchParams(EEVENT_3, e3Param);

    std::cout << std::endl;
    eventManager.unregisterFunc<CEvent3Handler>(EEVENT_3, &CEvent3Handler::processEvent3, &event3Handler);
    eventManager.dispatchParams(EEVENT_3, e3Param);
    std::cout << std::endl;


    eventManager.unregisterAllFunc();
    eventManager.dispatchParams(EEVENT_1, e1Param);
    eventManager.dispatchParams(EEVENT_2, e2Param);
    eventManager.dispatchParams(EEVENT_3, e3Param);
}
