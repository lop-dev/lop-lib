//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/msgCounter.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_MSGCOUNTER_H__
#define __BCLIB_FRAMEWORK_MSGCOUNTER_H__

#include <BCLib/utility/singleton.h>
#include <BCLib/utility/dateTime.h>
#include <BCLib/framework/message.h>


namespace BCLib
{
namespace Framework
{

class CMsgCounter
{
    //BCLIB_SINGLETON_DECLARE(CMsgCounter);

public:
    struct SMsgSt
    {
        BCLib::uint16 m_msgType;
        BCLib::uint16 m_msgID;
        BCLib::uint64 m_totalCnt;
        BCLib::uint64 m_totalSize;
        double m_fKbPerSec;
        bool m_bInPack;
    };
    typedef std::map<BCLib::uint32, SMsgSt> SMsgStMap;
    typedef std::vector<SMsgSt> SMsgStVec;

public:
    CMsgCounter();
    virtual ~CMsgCounter();

    void start();
    void stop();

    bool isStart() const;

    bool recordSendMsg(SMessage* msg, BCLib::uint32 msgSize);
    bool recordRecvMsg(SMessage* msg, BCLib::uint32 msgSize);

    bool recordSendMsg(BCLib::uint16 msgType, BCLib::uint16 msgID, BCLib::uint32 msgSize);
    bool recordRecvMsg(BCLib::uint16 msgType, BCLib::uint16 msgID, BCLib::uint32 msgSize, bool bInPack);

    virtual void statistic();

protected:
    bool _recordMsg(SMsgStMap& msgMap, BCLib::uint16 msgType, BCLib::uint16 msgID, BCLib::uint32 msgSize, bool bInPack);
    void _countMsg(SMsgStMap& msgMap, SMsgStVec& msgVec, BCLib::uint64& totalCnt, BCLib::uint64& totalSize, double& totalRate, float dura);
    void _print(const char* title, SMsgStVec& msgVec, BCLib::uint64 totalCnt, BCLib::uint64 totalSize, double totalRate, float dura);

protected:
    bool m_bStart;
    BCLib::Utility::CSteadyTime m_steadyTime;
    BCLib::int64 m_startTime;
    SMsgStMap m_sendMap;
    SMsgStMap m_recvMap;

    float m_fDura;
};

}//Framework
}//BCLib
#endif//__BCLIB_FRAMEWORK_MSGCOUNTER_H__
