//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/message.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_MESSAGE_H__
#define __BCLIB_FRAMEWORK_MESSAGE_H__

#include <BCLib/utility/baseDef.h>
#include <BCLib/utility/stream.h>
#include <BCLib/framework/baseDef.h>

namespace BCLib
{
namespace Framework
{
#if defined(_WIN32)
#pragma pack (push, 1)
#elif defined(_LINUX)
#pragma pack (1)
#endif

struct BCLIB_FRAMEWORK_API SMessage
{
public:
    SMessage(uint16 type, uint16 id)
    :m_type(type)
    ,m_id(id)
    {
    }

    uint16 getType() const
    {
        return m_type;
    }
    uint16 getID() const
    {
        return m_id;
    }

    uint32 getUniqueID() const
    {
        return  BCLIB_HI_SHIFT(m_type, 16) + m_id;
    }

private:
    uint16 m_type;
    uint16 m_id;
};

#if defined(_WIN32)
#pragma pack (pop)
#elif defined(_LINUX)
#pragma pack ()
#endif

class BCLIB_FRAMEWORK_API CMessage
{
public:
    CMessage(uint16 type, uint16 id);
    virtual ~CMessage()
    {
    }

    uint16 getType() const
    {
        return m_baseMsg.getType();
    }
    uint16 getID() const
    {
        return m_baseMsg.getID();
    }

    uint32 getUniqueID() const
    {
        return  m_baseMsg.getUniqueID();
    }

    bool serializeTo(Utility::CStream& stream) const;
    bool serializeFrom(const SMessage* msg, uint32 size);

protected:
    virtual bool _serializeTo(Utility::CStream& stream) const = 0;
    virtual bool _serializeFrom(Utility::CStream& stream) = 0;

private:
    SMessage m_baseMsg;
};

inline std::string messageToStr(SMessage&)
{
    return "";
}
inline std::string messageToStr(SMessage&, int)
{
    return "";
}
inline std::string messageToStr(CMessage&)
{
    return "";
}
inline std::string messageToStr(CMessage&, int)
{
    return "";
}
}//Framework
}//BCLib

#endif//__BCLIB_FRAMEWORK_MESSAGE_H__
