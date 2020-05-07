//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/uniqueID.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_UNIQUEID_H__
#define __BCLIB_UTILITY_UNIQUEID_H__

#include <set>
#include <BCLib/utility/thread/mutex.h>

namespace BCLib
{
namespace Utility
{
template<typename TYPE>
class CUniqueID
{
public:
    CUniqueID(bool canRecycle = false)
    {
        _init(1, (TYPE) - 1, canRecycle);
    }

    CUniqueID(TYPE startID, bool canRecycle = false)
    {
        _init(startID, (TYPE) - 1, canRecycle);
    }

    CUniqueID(TYPE startID, TYPE endID, bool canRecycle = false)
    {
        _init(startID, endID, canRecycle);
    }

    virtual ~CUniqueID()
    {
        m_mutex.lock();
        m_recycleIDList.clear();
        m_mutex.unlock();
    }

    TYPE invalidID()
    {
        return m_endID;
    }

    bool isValidID(TYPE id)
    {
        return id != m_endID;
    }

    TYPE getID()
    {
        TYPE resID = m_endID;
        m_mutex.lock();
        if(m_currID < m_endID)
        {
            resID = m_currID;
            ++m_currID;
            m_mutex.unlock();
            return resID;
        }

        if (!m_canRecycle)
        {
            m_currID = m_startID;
            resID = m_currID;
            ++m_currID;
            m_mutex.unlock();
            return resID;
        }

        if(!m_recycleIDList.empty())
        {
            typename std::set<TYPE>::iterator it = m_recycleIDList.begin();
            resID = *it;
            m_recycleIDList.erase(it);
            m_mutex.unlock();
            return resID;
        }
        m_mutex.unlock();
        return resID;
    }

    void putID(TYPE id)
    {
        if (!m_canRecycle)
        {
            return;
        }

        m_mutex.lock();
        if(m_startID <= id && id < m_endID)
        {
            typename std::set<TYPE>::iterator it = m_recycleIDList.find(id);
            if(it == m_recycleIDList.end())
            {
                m_recycleIDList.insert(id);
            }
        }
        m_mutex.unlock();
    }

private:
    void _init(TYPE startID, TYPE endID, bool canRecycle)
    {
        m_startID = startID;
        m_endID = endID;
        m_currID = m_startID;
        m_canRecycle = canRecycle;
    }

private:
    TYPE m_startID;
    TYPE m_endID;
    TYPE m_currID;
    bool m_canRecycle;
    CMutex m_mutex;
    std::set<TYPE> m_recycleIDList;
};

typedef CUniqueID<uint16> CUniqueID16;
typedef CUniqueID<uint32> CUniqueID32;
typedef CUniqueID<uint64> CUniqueID64;
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_UNIQUEID_H__
