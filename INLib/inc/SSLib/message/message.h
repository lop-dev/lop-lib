//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SSLib/message/message.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SSLIB_MESSAGE_MESSAGE_H__
#define __SSLIB_MESSAGE_MESSAGE_H__

#include <BCLib/framework/message.h>
#include <google/protobuf/message.h>
#include <BCLib/security/base64.h>

namespace SSLib
{

typedef BCLib::uint64 AccountID;
const AccountID INVALID_ACCOUNT_ID = 0;

typedef BCLib::uint16 EchoID;

namespace Message
{

#ifdef WIN32
#pragma pack (push, 1)
#else
#pragma pack (1)
#endif

struct SNetMessage : public BCLib::Framework::SMessage
{
public:
    SNetMessage(BCLib::uint16 type, BCLib::uint16 id) : BCLib::Framework::SMessage(type, id) {}

    void setEcho(EchoID echo) {m_echo = echo;}
    EchoID getEcho(){return m_echo;}

private:
    EchoID m_echo;
};

#ifdef WIN32
#pragma pack (pop)
#else
#pragma pack ()
#endif


class CNetMessage : public BCLib::Framework::CMessage
{
public:
    CNetMessage(BCLib::uint16 type, BCLib::uint16 id)
		:BCLib::Framework::CMessage(type, id)
		,m_echo(0)
	{}

    void setEcho(EchoID echo) {m_echo = echo;}
    EchoID getEcho(){return m_echo;}

protected:
    virtual bool _serializeTo(BCLib::Utility::CStream& stream) const
    {
        stream << m_echo;
        return true;
    }

    virtual bool _serializeFrom(BCLib::Utility::CStream& stream)
    {
        stream >> m_echo;
        return true;
    }
private:
    EchoID m_echo;
};

template<class PTBUF>
class CBufMessage : public CNetMessage
{
public:
	CBufMessage(BCLib::uint16 type, BCLib::uint16 id)
		:CNetMessage(type, id)
		,m_String("")
		,m_Result(0)
	{}

	bool serialize(PTBUF* ptBuf)
	{
		if (ptBuf == NULL) return false;
		ptBuf->SerializeToString(&m_String);
		return true;
	}

	bool deserialize(PTBUF* ptBuf)
	{
		if (ptBuf == NULL) return false;
		if (m_String == "") return false;
		ptBuf->ParseFromString(m_String);
		return true;
	}

	void setResult(BCLib::int16 echo) {m_Result = echo;}
	BCLib::int16 getResult(){return m_Result;}

protected:
	virtual bool _serializeTo(BCLib::Utility::CStream& stream) const
	{
		CNetMessage::_serializeTo(stream);
		stream << m_String;
		stream << m_Result;
		return true;
	}

	virtual bool _serializeFrom(BCLib::Utility::CStream& stream)
	{
		CNetMessage::_serializeFrom(stream);
		stream >> m_String;
		stream >> m_Result;
		return true;
	}

private:
	std::string m_String;
	BCLib::int16 m_Result;
};

}//Message

}//SSLib

#endif//__SSLIB_MESSAGE_MESSAGE_H__
