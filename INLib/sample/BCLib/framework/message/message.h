//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/netfw/message/message.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_FRAMEWORK_NETFW_MESSAGE_MESSAGE_H__
#define __BCLIB_SAMPLE_FRAMEWORK_NETFW_MESSAGE_MESSAGE_H__
#include <BCLib/framework/message.h>
#include <BCLib/framework/msgType.h>
#include <BCLib/framework/msgDebug.h>

enum EMsgType
{
    EMT_SAMPLE = BCLib::Message::EMT_EXTEND,
};

enum EMsgID
{
    EMID_SAMPLE_NTF_OK,
    EMID_SAMPLE_REQ_SAY,
    EMID_SAMPLE_RES_SAY,
    EMID_SAMPLE_REQ_SAY_C,
    EMID_SAMPLE_RES_SAY_C,
};

#define SAMPLE_TEXT_MAX 256


#ifdef WIN32
#pragma pack (push, 1)
#else
#pragma pack (1)
#endif

BCLIB_MSGDEBUG(EMT_SAMPLE, EMID_SAMPLE_NTF_OK);
struct SMsgSampleNtfOk : public BCLib::Framework::SMessage
{
public:
    SMsgSampleNtfOk()
    :BCLib::Framework::SMessage(EMT_SAMPLE, EMID_SAMPLE_NTF_OK)
    {
    }
};

BCLIB_MSGDEBUG(EMT_SAMPLE, EMID_SAMPLE_REQ_SAY);
class CMsgSampleReqSay : public BCLib::Framework::CMessage
{
public:
    CMsgSampleReqSay()
    :BCLib::Framework::CMessage(EMT_SAMPLE, EMID_SAMPLE_REQ_SAY)
    {
    }

    virtual bool _serializeTo(BCLib::Utility::CStream& stream) const
    {
        stream << m_bool;
        stream << m_byte;
        stream << len;
        stream << m_bytes;
        stream << m_double;
        stream << m_float;
        stream << m_int;
        stream << m_long;
        stream << m_short;
        stream << m_string;
        stream << m_uint;
        stream << m_ulong;
        stream << m_ushort;
        return true;
    }

    virtual bool _serializeFrom(BCLib::Utility::CStream& stream)
    {
        stream >> m_bool;
        stream >> m_byte;
        stream >> len;
        stream >> m_bytes;
        stream >> m_double;
        stream >> m_float;
        stream >> m_int;
        stream >> m_long;
        stream >> m_short;
        stream >> m_string;
        stream >> m_uint;
        stream >> m_ulong;
        stream >> m_ushort;
        return true;
    }
    bool m_bool;
    byte m_byte;
    byte m_bytes[5];
    BCLib::uint32 len;
    double m_double;
    float m_float;
    BCLib::int32 m_int;
    BCLib::int64 m_long;
    BCLib::int16 m_short;
    std::string m_string;
    BCLib::uint32 m_uint;
    BCLib::uint64 m_ulong;
    BCLib::uint16 m_ushort;
    
};

BCLIB_MSGDEBUG(EMT_SAMPLE, EMID_SAMPLE_RES_SAY);
class CMsgSampleResSay : public BCLib::Framework::CMessage
{
public:
    CMsgSampleResSay()
    :BCLib::Framework::CMessage(EMT_SAMPLE, EMID_SAMPLE_RES_SAY)
    {
    }

    virtual bool _serializeTo(BCLib::Utility::CStream& stream) const
    {
        stream << m_bool;
        stream << m_byte;
        stream << len;
        stream << m_bytes;
        stream << m_double;
        stream << m_float;
        stream << m_int;
        stream << m_long;
        stream << m_short;
        stream << m_string;
        stream << m_uint;
        stream << m_ulong;
        stream << m_ushort;
        return true;
    }

    virtual bool _serializeFrom(BCLib::Utility::CStream& stream)
    {
        stream >> m_bool;
        stream >> m_byte;
        stream >> len;
        stream >> m_bytes;
        stream >> m_double;
        stream >> m_float;
        stream >> m_int;
        stream >> m_long;
        stream >> m_short;
        stream >> m_string;
        stream >> m_uint;
        stream >> m_ulong;
        stream >> m_ushort;
        return true;
    }
    bool m_bool;
    byte m_byte;
    byte m_bytes[5];
    BCLib::uint32 len;
    double m_double;
    float m_float;
    BCLib::int32 m_int;
    BCLib::int64 m_long;
    BCLib::int16 m_short;
    std::string m_string;
    BCLib::uint32 m_uint;
    BCLib::uint64 m_ulong;
    BCLib::uint16 m_ushort;
};

#ifdef WIN32
#pragma pack (pop)
#else
#pragma pack ()
#endif

BCLIB_MSGDEBUG(EMT_SAMPLE, EMID_SAMPLE_REQ_SAY_C);
class CMsgSampleReqSayC : public BCLib::Framework::CMessage
{
public:
    std::string m_strSay;

    CMsgSampleReqSayC()
    :BCLib::Framework::CMessage(EMT_SAMPLE, EMID_SAMPLE_REQ_SAY_C)
    {
        ;
    }

    virtual bool _serializeTo(BCLib::Utility::CStream& stream) const
    {
        stream << m_strSay;
        return true;
    }

    virtual bool _serializeFrom(BCLib::Utility::CStream& stream)
    {
        stream >> m_strSay;
        return true;
    }
};

BCLIB_MSGDEBUG(EMT_SAMPLE, EMID_SAMPLE_RES_SAY_C);
class CMsgSampleResSayC : public BCLib::Framework::CMessage
{
public:
    std::string m_strSay;

    CMsgSampleResSayC()
    :BCLib::Framework::CMessage(EMT_SAMPLE, EMID_SAMPLE_RES_SAY_C)
    {
        ;
    }

    virtual bool _serializeTo(BCLib::Utility::CStream& stream) const
    {
        stream << m_strSay;
        return true;
    }

    virtual bool _serializeFrom(BCLib::Utility::CStream& stream)
    {
        stream >> m_strSay;
        return true;
    }
};
#endif//__BCLIB_SAMPLE_FRAMEWORK_NETFW_MESSAGE_MESSAGE_H__
