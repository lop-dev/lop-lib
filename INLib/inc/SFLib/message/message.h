//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/message/message.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_MESSAGE_MESSAGE_H__
#define __SFLIB_MESSAGE_MESSAGE_H__

#include <BCLib/network/baseDef.h>
#include <BCLib/framework/msgType.h>
#include <BCLib/framework/msgDebug.h>

namespace SFLib
{
enum EServerType
{
    // 特殊服务器类型
    ESERVER_UNKNOW      = 0,    // 无效
    ESERVER_LOCAL,              // "LC" 本地的进程
    ESERVER_ANYXX,              // "XX" 任一服务器或客户端
    ESERVER_ANYXS,              // "XS" 任一服务器

    ESERVER_GAMECLIENT  = 8,    // "GC"
	ESERVER_MASTER,             // "MS"

    // 组内服务器类型
	ESERVER_LOGIC_START,		// "LS"...
	ESERVER_LOGIC_END = ESERVER_LOGIC_START + 10,  // "LS"...

    // 组外服务器类型
    ESERVER_EXTERNAL_START,       // "EX" ...
    ESERVER_EXTERNAL_END = ESERVER_EXTERNAL_START + 10, // "EX" ...

    ESERVER_MAX
};

enum EGCServerType
{
    EGCSERVER_LOGIN,
    EGCSERVER_GATEWAY,

    EGCSERVER_MAX
};

extern std::string getServerTypeName(EServerType serverType);
class CServerTypeNameHelper
{
public:
	CServerTypeNameHelper();
	virtual ~CServerTypeNameHelper();
    static CServerTypeNameHelper* getHelper() { return m_serverTypeNameHelper; }

    virtual std::string getLogicServerTypeName(EServerType serverType);
    virtual std::string getExternalServerTypeName(EServerType serverType);

private:
    static CServerTypeNameHelper* m_serverTypeNameHelper;
};

// 功能类型
enum EFuncType
{
    EFUNC_UNKNOW,           // 无效
    EFUNC_GAMEFRAME,        // 给 SFLib 框架使用的消息
    EFUNC_EXTEND_SERVER,    // 从这个变量开始的消息，一旦未找到处理函数，都被丢到 Server 中处理
    EFUNC_EXTEND_UNHANDLE,  // 从这个变量开始的消息，一旦未找到处理函数，都被丢到 CNetMsgQueue 队列中
};

extern std::string getFuncMsgIDName(BCLib::uint16 funcID, BCLib::int32 msgID);
class CFuncMsgIDNameHelper
{
public:
	CFuncMsgIDNameHelper();
	virtual ~CFuncMsgIDNameHelper();
	static CFuncMsgIDNameHelper* getHelper() { return m_funcMsgIDNameHelper; }

	virtual std::string getFuncMsgIDName(BCLib::uint16 funcID, BCLib::int32 msgID);

private:
	static CFuncMsgIDNameHelper* m_funcMsgIDNameHelper;
};

namespace Message
{
#define SFLIB_VERSION_TEXT_MAX  16
//#define SFLIB_IP_TEXT_MAX       63
#define SFLIB_RANDKEY_TEXT_MAX  32
#define SFLIB_MSG_TYPE(server, func) ( BCLIB_HI_SHIFT(server, 8) | func)
#define SFLIB_MSGDEBUG(server, fun, id) BCLIB_MSGDEBUG(SFLIB_MSG_TYPE(server, fun), id)

#ifdef WIN32
#pragma pack (push, 1)
#else
#pragma pack (1)
#endif

// server 指定处理服务器
struct SNetMessage : public BCLib::Framework::SMessage
{
public:
    SNetMessage(BCLib::uint8 server, BCLib::uint8 func, BCLib::uint16 id)
    :BCLib::Framework::SMessage(SFLIB_MSG_TYPE(server, func), id)
    {
    }
    BCLib::uint8 getServer() const
    {
        BCLib::uint16 type = BCLib::Framework::SMessage::getType();
        return (BCLib::uint8)BCLIB_LO_SHIFT((type & 0xFF00), 8);
    }

    BCLib::uint8 getFunc() const
    {
        BCLib::uint16 type = BCLib::Framework::SMessage::getType();
        return (BCLib::uint8)(type & 0x00FF);
    }

    std::string getDebugPrompt() const
    {
        BCLib::Utility::CStringA strPrompt = "";

        strPrompt.format("Server[%s] Func[%d] ID[%d] Prompt[%s]",
            getServerTypeName((EServerType)getServer()).c_str(), getFunc(), getID(),
            BCLib::Framework::CMsgDebug::singleton().getPrompt((SFLib::Message::SNetMessage*)this).c_str());

        return strPrompt;
    }
};

#ifdef WIN32
#pragma pack (pop)
#else
#pragma pack ()
#endif


class CNetMessage : public BCLib::Framework::CMessage
{
public:
    CNetMessage(BCLib::uint8 server, BCLib::uint8 func, BCLib::uint16 id)
    :BCLib::Framework::CMessage(SFLIB_MSG_TYPE(server, func), id)
    {
    }

    BCLib::uint8 getServer() const
    {
        BCLib::uint16 type = BCLib::Framework::CMessage::getType();
        return (BCLib::uint8)BCLIB_LO_SHIFT((type & 0xFF00), 8);
    }

    BCLib::uint8 getFunc() const
    {
        BCLib::uint16 type = BCLib::Framework::CMessage::getType();
        return (BCLib::uint8)(type & 0x00FF);
    }

    std::string getDebugPrompt() const
    {
        BCLib::Utility::CStringA strPrompt = "";

        strPrompt.format("Server[%s] Func[%d] ID[%d] Prompt[%s]",
            getServerTypeName((EServerType)getServer()).c_str(), getFunc(), getID(),
            BCLib::Framework::CMsgDebug::singleton().getPrompt((SFLib::Message::SNetMessage*)this).c_str());

        return strPrompt;
    }

public:
	virtual bool serialize(const void* pBuf) { return false; }
	virtual bool deserialize(void* pBuf) { return false; }

	virtual void setResult(BCLib::int16 echo) {}
	virtual void setTracer(const std::string& strTracer) {}
};

// 预留给CNetMessage中serialize和deserialize对应的pBuf扩展用
class IPBuf
{
public:
	IPBuf() {};
	virtual ~IPBuf() {};

	virtual bool serialize(std::string* data) = 0;
	virtual bool deserialize(const std::string& data) = 0;
};

}//Message

}//SFLib

#endif//__SFLIB_MESSAGE_MESSAGE_H__
