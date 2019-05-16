//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/serviceID.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_SERVICEID_H__
#define __BCLIB_FRAMEWORK_SERVICEID_H__

#include <BCLib/framework/baseDef.h>

#define BCLIB_FW_CTRL_NAME_MAX 32
#define BCLIB_FW_SERVICE_NAME_MAX 32

namespace BCLib
{
namespace Framework
{
#if defined(_WIN32)
#pragma pack (push, 1)
#elif defined(_LINUX)
#pragma pack (1)
#endif

class BCLIB_FRAMEWORK_API CCtrlID
{
private:
    union
    {
        uint16 m_index;
        struct
        {
            uint8 m_mainCtrlNum;
            uint8 m_ctrlNum;
        } m_ctrlID;
    };

public:
    CCtrlID(uint16 index)
    :m_index(index)
    {
    }
    CCtrlID(uint8 mainCtrlNum, uint8 ctrlNum)
    :m_index(0)
    {
        setCtrlID(mainCtrlNum, ctrlNum);
    }
    CCtrlID(const CCtrlID& other)
    :m_index(other.m_index)
    {
    }

    uint16 getIndex() const
    {
        return m_index;
    }
    uint8 getCtrlNum() const
    {
        return m_ctrlID.m_ctrlNum;
    }
    uint8 getMainCtrlNum() const
    {
        return m_ctrlID.m_mainCtrlNum;
    }

    void setCtrlID(uint16 index)
    {
        m_index = index;
    }
    void setCtrlID(uint8 mainCtrlNum, uint8 ctrlNum)
    {
        m_ctrlID.m_mainCtrlNum = mainCtrlNum; m_ctrlID.m_ctrlNum = ctrlNum;
    }

    bool isMainCtrl() const
    {
        return m_ctrlID.m_ctrlNum == m_ctrlID.m_mainCtrlNum && m_ctrlID.m_ctrlNum != 0;
    }

    bool operator ==(const CCtrlID& other) const
    {
        return m_index == other.m_index;
    }
    bool operator !=(const CCtrlID& other) const
    {
        return m_index != other.m_index;
    }

    bool operator ==(uint16 index) const
    {
        return m_index == index;
    }
    bool operator !=(uint16 index) const
    {
        return m_index != index;
    }

    CCtrlID& operator =(const CCtrlID& other)
    {
        m_index = other.m_index; return *this;
    }
};


class BCLIB_FRAMEWORK_API CServiceID
{
private:
    union
    {
        uint32 m_index;
        struct
        {
            uint16 m_ctrlID;
            uint16 m_serviceNum;
        } m_serviceID;
    };

public:
    CServiceID(uint32 index = 0)
    :m_index(index)
    {
    }
    CServiceID(const CCtrlID& ctrlID, uint16 serviceNum)
    :m_index(0)
    {
        setServiceID(ctrlID, serviceNum);
    }
    CServiceID(const CServiceID& other)
    :m_index(other.m_index)
    {
    }

    uint32 getIndex() const
    {
        return m_index;
    }
    CCtrlID getCtrlID() const
    {
        return CCtrlID(m_serviceID.m_ctrlID);
    }
    uint16 getServiceNum() const
    {
        return m_serviceID.m_serviceNum;
    }

    void setServiceID(uint32 index)
    {
        m_index = index;
    }
    void setServiceID(const CCtrlID& ctrlID, uint16 serviceNum)
    {
        m_serviceID.m_ctrlID = ctrlID.getIndex();
        m_serviceID.m_serviceNum = serviceNum;
    }

    bool isInvalid() const
    {
        return m_index == 0;
    }

    bool operator ==(const CServiceID& other) const
    {
        return m_index == other.m_index;
    }
    bool operator !=(const CServiceID& other) const
    {
        return m_index != other.m_index;
    }

    bool operator ==(uint32 index) const
    {
        return m_index == index;
    }
    bool operator !=(uint32 index) const
    {
        return m_index != index;
    }

    CServiceID& operator =(const CServiceID& other)
    {
        m_index = other.m_index; return *this;
    }
};

#if defined(_WIN32)
#pragma pack (pop)
#elif defined(_LINUX)
#pragma pack ()
#endif
}//Framework
}//BCLib

#endif//__BCLIB_FRAMEWORK_SERVICEID_H__
