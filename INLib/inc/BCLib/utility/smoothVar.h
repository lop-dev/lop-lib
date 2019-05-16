//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/smoothVar.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_SMOOTHVAR_H__
#define __BCLIB_UTILITY_SMOOTHVAR_H__

#include <BCLib/utility/dateTime.h>


namespace BCLib
{
namespace Utility
{
template<class VARTYPE>
class CSmoothVar
{
public:
    CSmoothVar()
    :m_adjustThreshold(0)
    ,m_adjustPerSec(0)
    ,m_var(0)
    ,m_newVar(0)
    ,m_adjustFlag(0)
    ,m_adjustStartTime(0)
    ,m_adjustDuration(0)
    {
        ;
    }

    bool init(VARTYPE adjustThreshold, VARTYPE adjustPerSec)
    {
        if(adjustThreshold <= 0 || adjustPerSec <= 0)
        {
            return false;
        }
        m_adjustThreshold = adjustThreshold;
        m_adjustPerSec = adjustPerSec;
        return true;
    }

    void clear()
    {
        m_var = 0;
        m_newVar = 0;
        m_adjustFlag = 0;
        m_adjustStartTime = 0;
        m_adjustDuration = 0;
    }

    void addVar(VARTYPE var)
    {
        if(0 != m_adjustFlag)
        {
            m_var = getVar();
            m_adjustFlag = 0;
        }
        else if(0 == m_var)
        {
            m_var = var;
        }

        if(var < m_var && var + m_adjustThreshold < m_var)
        {
            m_newVar = var;
            m_adjustFlag = -1;
            m_steadyTime.now();
            m_adjustStartTime = m_steadyTime.getMilliseconds();
            m_adjustDuration = (m_var - var) * 1000 / m_adjustPerSec;
        }
        else if(m_var < var && m_var + m_adjustThreshold < var)
        {
            m_newVar = var;
            m_adjustFlag = 1;
            m_steadyTime.now();
            m_adjustStartTime = m_steadyTime.getMilliseconds();
            m_adjustDuration = (var - m_var) * 1000 / m_adjustPerSec;
        }
    }

    VARTYPE getVar()
    {
        if(0 == m_adjustFlag)
        {
            return m_var;
        }

        m_steadyTime.now();
        BCLib::int64 nowTime = m_steadyTime.getMilliseconds();
        if(m_adjustStartTime + m_adjustDuration < nowTime)
        {
            m_adjustFlag = 0;
            m_var = m_newVar;
            return m_var;
        }

        return (VARTYPE)(m_var + m_adjustFlag * (nowTime - m_adjustStartTime) * m_adjustPerSec / 1000);
    }

protected:
    CSteadyTime m_steadyTime;
    VARTYPE m_adjustThreshold;
    VARTYPE m_adjustPerSec;
    VARTYPE m_var;
    VARTYPE m_newVar;
    BCLib::int8 m_adjustFlag;   //! 0:不调整,-1:负调整,1:正调整
    BCLib::int64 m_adjustStartTime;
    BCLib::int64 m_adjustDuration;
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_SMOOTHVAR_H__
