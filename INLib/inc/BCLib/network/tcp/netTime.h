//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/tcp/netTime.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_NETWORK_TCP_NETTIME_H__
#define __BCLIB_NETWORK_TCP_NETTIME_H__

#include <BCLib/network/baseDef.h>
#include <BCLib/utility/singleton.h>
#include <BCLib/utility/dateTime.h>
#include <BCLib/utility/smoothVar.h>
#include <BCLib/utility/thread/mutex.h>
#include <BCLib/utility/function.h>


namespace BCLib
{
namespace Network
{
class BCLIB_NETWORK_API CNetTime
{
public:
    CNetTime();
    ~CNetTime();

    bool init(bool bMasterMode, BCLib::int64 netTimeAdjThreshold, BCLib::int64 netTimeAdjPerSec);
    bool isMasterMode() const;
    bool isReady() const;
    bool addMasterNetTime(BCLib::int64 masterNetTime);

    BCLib::int64 getMilliseconds();

    CNetTime& operator=(const CNetTime& netTime);

protected:
    bool m_bMasterMode;
    bool m_bInit;
    bool m_bReady;
    BCLib::Utility::CSteadyTime m_steadyTime;
    BCLib::Utility::CSmoothVar<BCLib::int64> m_startOffset;
};


class BCLIB_NETWORK_API CNetTimeMgr
{
    BCLIB_SINGLETON_DECLARE(CNetTimeMgr);
public:
    bool init(bool bMasterMode = false, BCLib::int64 netTimeAdjThreshold = 10, BCLib::int64 netTimeAdjPerSec = 990);
    bool isMasterMode() const;
    bool isReady() const;

    BCLib::int64 getMilliseconds();

protected:
    CNetTimeMgr();
    ~CNetTimeMgr();

    bool _addMasterNetTime(BCLib::int64 masterNetTime);
    void _setNetTimeToAllFunc();

    typedef BCLib::Utility::CFunction<void, const CNetTime&> NetTimeSetFunc;
    template<class CLASS>
    bool _addNetTimerSetFunc(void(CLASS::*fun)(const CNetTime&), CLASS* obj)
    {
        BCLib::Utility::CFunctionObject<void, CLASS, const CNetTime&> setFuncWithObj(fun, obj);
        NetTimeSetFunc* setFunc = setFuncWithObj.clone();
        if(NULL == setFunc || !setFunc->isValid())
        {
            return false;
        }
        BCLib::Utility::CMutexFun mutexfun(&m_mutex);
        SFuncNode node;
        node.m_obj = obj;
        node.m_setFunc = setFunc;
        m_funcNodeList.push_back(node);
        (*setFunc)(m_netTime);
        return true;
    }
    template<class CLASS>
    void _removeNetTimerSetFunc(CLASS* obj)
    {
        BCLib::Utility::CMutexFun mutexfun(&m_mutex);
        for(BCLib::uint32 i=0; i<m_funcNodeList.size();)
        {
            if(m_funcNodeList[i].m_obj == obj)
            {
                BCLIB_SAFE_DELETE(m_funcNodeList[i].m_setFunc);
                m_funcNodeList[i] = m_funcNodeList.back();
                m_funcNodeList.pop_back();
            }
            else
            {
                ++i;
            }
        }
    }

    bool _getUpdateAuthority(void* updateObj);
    bool _releaseUpdateAuthority(void* updateObj);

protected:
    BCLib::Utility::CMutex m_mutex;
    CNetTime m_netTime;
    struct SFuncNode
    {
        void* m_obj;
        NetTimeSetFunc* m_setFunc;
    };
    typedef std::vector<SFuncNode> SFuncNodeList;
    SFuncNodeList m_funcNodeList;
    void* m_updateObj;

    friend class CTcpConnection;
    friend class CTcpConnectionMgr;
};
}//Network
}//BCLib

#endif//__BCLIB_NETWORK_TCP_NETTIME_H__
