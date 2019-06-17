//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SSLib/accountSet/accountServer/message/accountServerMsg.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SSLIB_ACCOUNTSET_ACCOUNTSERVER_MESSAGE_ACCOUNTSERVERMSG_H__
#define __SSLIB_ACCOUNTSET_ACCOUNTSERVER_MESSAGE_ACCOUNTSERVERMSG_H__

#include <sstream>
#include <BCLib/framework/msgDebug.h>
#include <BCLib/utility/strHelper.h>
#include <SSLib/message/message.h>
#include <SSLib/message/msgTypeSrv.h>

#include "../protobuf/accountAuth.pb.h"
#include "../protobuf/accountVerify.pb.h"

namespace SSLib
{
namespace AccountServer
{

#define     ACCOUNT_NAME_LEN        (20)
#define     ACCOUNT_PWD_LEN         (32)
#define     ACCOUNT_ACCREDIT_LEN    (32)
#define     ACCOUNT_FLAG_LEN       (16)

#ifdef WIN32
#pragma pack (push, 1)
#else
#pragma pack (1)
#endif


enum EMsgIDAccountSet
{
    EMID_C2S_ACCOUNT_SET_ESS_OKAY = 1,

    EMID_C2S_REQ_LOGIN, // 重要
    EMID_S2C_RES_LOGIN,

    EMID_C2S_REQ_INSERT_ACCOUNT_AUTH,
    EMID_S2C_RES_INSERT_ACCOUNT_AUTH,

    EMID_C2S_REQ_SELECT_ACCOUNT_AUTH,
    EMID_S2C_RES_SELECT_ACCOUNT_AUTH,

    EMID_C2S_REQ_UPDATE_ACCOUNT_AUTH,
    EMID_S2C_RES_UPDATE_ACCOUNT_AUTH,

    EMID_C2S_REQ_DELETE_ACCOUNT_AUTH,
    EMID_S2C_RES_DELETE_ACCOUNT_AUTH,

	// 目前游戏服会用到
    EMID_C2S_REQ_QUERY_ACCOUNT_NAME_PWD = 100,
    EMID_S2C_RES_QUERY_ACCOINT_NAME_PWD,

    EMID_C2S_REQ_QUERY_ACCOUNT_NAME_KEY,
    EMID_S2C_RES_QUERY_ACCOINT_NAME_KEY,

    EMID_C2S_REQ_VERIFY_NAME_PWD = 200,
    EMID_S2C_RES_VERIFY_NAME_PWD,

	EMID_C2S_REQ_INSERT_NAME_PWD,
	EMID_S2C_RES_INSERT_NAME_PWD,
};

BCLIB_MSGDEBUG(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_ACCOUNT_SET_ESS_OKAY);
struct SMsgC2SAccountSetEssOkay : public SSLib::Message::SNetMessage
{
public:
    char m_name[ACCOUNT_NAME_LEN + 1];
    char m_pwd[ACCOUNT_PWD_LEN + 1];
    char m_dbFlag[ACCOUNT_FLAG_LEN + 1];
    SMsgC2SAccountSetEssOkay() : SSLib::Message::SNetMessage(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_ACCOUNT_SET_ESS_OKAY)
    {
        memset(m_name,0,ACCOUNT_NAME_LEN + 1);
        memset(m_pwd,0,ACCOUNT_PWD_LEN + 1);
        memset(m_dbFlag,0,ACCOUNT_FLAG_LEN + 1);
    }
    void setName(const std::string& text)
    {
        BCLib::Utility::CStrHelper::strNCpy(m_name, sizeof(m_name), text.c_str(), ACCOUNT_NAME_LEN);
    }
    void setPwd(const std::string& text)
    {
        BCLib::Utility::CStrHelper::strNCpy(m_pwd, sizeof(m_pwd), text.c_str(), ACCOUNT_PWD_LEN);
    }
    void setAccountType(const std::string& text)
    {
        BCLib::Utility::CStrHelper::strNCpy(m_dbFlag, sizeof(m_dbFlag), text.c_str(), ACCOUNT_FLAG_LEN);
    }
};

BCLIB_MSGDEBUG(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_REQ_LOGIN);
struct SMsgC2SReqLogin : public SSLib::Message::SNetMessage
{
public:
    char m_name[ACCOUNT_NAME_LEN + 1];
    char m_pwd[ACCOUNT_PWD_LEN + 1];
    char m_dbFlag[ACCOUNT_FLAG_LEN + 1];
    SMsgC2SReqLogin() : SSLib::Message::SNetMessage(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_REQ_LOGIN)
    {
        memset(m_name,0,ACCOUNT_NAME_LEN + 1);
        memset(m_pwd,0,ACCOUNT_PWD_LEN + 1);
        memset(m_dbFlag,0,ACCOUNT_FLAG_LEN + 1);
    }
    void setName(const std::string& text)
    {
        BCLib::Utility::CStrHelper::strNCpy(m_name, sizeof(m_name), text.c_str(), ACCOUNT_NAME_LEN);
    }
    void setPwd(const std::string& text)
    {
        BCLib::Utility::CStrHelper::strNCpy(m_pwd, sizeof(m_pwd), text.c_str(), ACCOUNT_PWD_LEN);
    }
    void setAccountType(const std::string& text)
    {
        BCLib::Utility::CStrHelper::strNCpy(m_dbFlag, sizeof(m_dbFlag), text.c_str(), ACCOUNT_FLAG_LEN);
    }
};

BCLIB_MSGDEBUG(SSLib::EMT_ACCOUNTCLIENT, EMID_S2C_RES_LOGIN);
struct SMsgS2CResLogin : public SSLib::Message::SNetMessage
{
public:
    //EDBTaskResult m_eTaskResult;
	int m_eTaskResult;
    AccountID m_accountID;
    SMsgS2CResLogin() : SNetMessage(SSLib::EMT_ACCOUNTCLIENT, EMID_S2C_RES_LOGIN)
    {
        m_accountID = 0;
    }
};

//
BCLIB_MSGDEBUG(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_REQ_INSERT_ACCOUNT_AUTH);
class CMsgC2SReqInsertAccountAuth : public SSLib::Message::CBufMessage<PTBuf::CAccountAuth>
{
public:
    CMsgC2SReqInsertAccountAuth() : SSLib::Message::CBufMessage<PTBuf::CAccountAuth>(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_REQ_INSERT_ACCOUNT_AUTH)
    {
    }
};

BCLIB_MSGDEBUG(SSLib::EMT_ACCOUNTSERVER, EMID_S2C_RES_INSERT_ACCOUNT_AUTH);
class CMsgS2CResInsertAccountAuth : public SSLib::Message::CBufMessage<PTBuf::CAccountAuth>
{
public:
    CMsgS2CResInsertAccountAuth() : SSLib::Message::CBufMessage<PTBuf::CAccountAuth>(SSLib::EMT_ACCOUNTSERVER, EMID_S2C_RES_INSERT_ACCOUNT_AUTH)
    {
    }
};

//
BCLIB_MSGDEBUG(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_REQ_SELECT_ACCOUNT_AUTH);
class CMsgC2SReqSelectAccountAuth : public SSLib::Message::CBufMessage<PTBuf::CAccountAuth>
{
public:
    CMsgC2SReqSelectAccountAuth() : SSLib::Message::CBufMessage<PTBuf::CAccountAuth>(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_REQ_SELECT_ACCOUNT_AUTH)
    {
    }

};

BCLIB_MSGDEBUG(SSLib::EMT_ACCOUNTSERVER, EMID_S2C_RES_SELECT_ACCOUNT_AUTH);
class CMsgS2CResSelectAccountAuth : public SSLib::Message::CBufMessage<PTBuf::CAccountAuth>
{
public:
    CMsgS2CResSelectAccountAuth() : SSLib::Message::CBufMessage<PTBuf::CAccountAuth>(SSLib::EMT_ACCOUNTSERVER, EMID_S2C_RES_SELECT_ACCOUNT_AUTH)
    {
    }
};

//
BCLIB_MSGDEBUG(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_REQ_UPDATE_ACCOUNT_AUTH);
class CMsgC2SReqUpdateAccountAuth : public SSLib::Message::CBufMessage<PTBuf::CAccountAuth>
{
public:
    CMsgC2SReqUpdateAccountAuth() : SSLib::Message::CBufMessage<PTBuf::CAccountAuth>(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_REQ_UPDATE_ACCOUNT_AUTH)
    {
    }
};

BCLIB_MSGDEBUG(SSLib::EMT_ACCOUNTSERVER, EMID_S2C_RES_UPDATE_ACCOUNT_AUTH);
class CMsgS2CResUpdateAccountAuth : public SSLib::Message::CBufMessage<PTBuf::CAccountAuth>
{
public:
    CMsgS2CResUpdateAccountAuth() : SSLib::Message::CBufMessage<PTBuf::CAccountAuth>(SSLib::EMT_ACCOUNTSERVER, EMID_S2C_RES_UPDATE_ACCOUNT_AUTH)
    {
    }
};

//
BCLIB_MSGDEBUG(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_REQ_DELETE_ACCOUNT_AUTH);
class CMsgC2SReqDeleteAccountAuth : public SSLib::Message::CBufMessage<PTBuf::CAccountAuth>
{
public:
    CMsgC2SReqDeleteAccountAuth() : SSLib::Message::CBufMessage<PTBuf::CAccountAuth>(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_REQ_DELETE_ACCOUNT_AUTH)
    {
    }
};

BCLIB_MSGDEBUG(SSLib::EMT_ACCOUNTSERVER, EMID_S2C_RES_DELETE_ACCOUNT_AUTH);
class CMsgS2CResDeleteAccountAuth : public SSLib::Message::CBufMessage<PTBuf::CAccountAuth>
{
public:
    CMsgS2CResDeleteAccountAuth() : SSLib::Message::CBufMessage<PTBuf::CAccountAuth>(SSLib::EMT_ACCOUNTSERVER, EMID_S2C_RES_DELETE_ACCOUNT_AUTH)
    {
    }
};

//
BCLIB_MSGDEBUG(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_REQ_QUERY_ACCOUNT_NAME_PWD);
struct SMsgC2SReqQueryAccountNamePwd : public SSLib::Message::SNetMessage
{
    char m_name[ACCOUNT_NAME_LEN + 1];
    char m_pwd[ACCOUNT_PWD_LEN+1];
    char m_accredit[ACCOUNT_ACCREDIT_LEN+1];
    char m_dbFlag[ACCOUNT_FLAG_LEN+1];
    SMsgC2SReqQueryAccountNamePwd() : SNetMessage(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_REQ_QUERY_ACCOUNT_NAME_PWD)
    {
        memset(m_name,0,ACCOUNT_NAME_LEN + 1);
        memset(m_pwd,0,ACCOUNT_PWD_LEN + 1);
        memset(m_dbFlag,0,ACCOUNT_FLAG_LEN + 1);
        setAccredit("237hdwtjd92f8");
    }

    void setName(const std::string& text)
    {
        BCLib::Utility::CStrHelper::strNCpy(m_name, sizeof(m_name), text.c_str(), ACCOUNT_NAME_LEN);
    }
    void setPwd(const std::string& text)
    {
        BCLib::Utility::CStrHelper::strNCpy(m_pwd, sizeof(m_pwd), text.c_str(), ACCOUNT_PWD_LEN);
    }
    void setAccredit(const std::string& text)
    {
        BCLib::Utility::CStrHelper::strNCpy(m_accredit, sizeof(m_accredit), text.c_str(), ACCOUNT_ACCREDIT_LEN);
    }
    void setDBFlag(const std::string& text)
    {
        BCLib::Utility::CStrHelper::strNCpy(m_dbFlag, sizeof(m_dbFlag), text.c_str(), ACCOUNT_FLAG_LEN);
    }

};

enum EVerifyResult
{
    eVerifyRes_OK = 1,
    eVerifyRes_Stop,
    eVerifyRes_Error,
    eVerifyRes_Unknown,
};

BCLIB_MSGDEBUG(SSLib::EMT_ACCOUNTSERVER, EMID_S2C_RES_QUERY_ACCOINT_NAME_PWD);
struct SMsgS2CResQueryAccountNamePwd : public SSLib::Message::SNetMessage
{
public:
    //EDBTaskResult m_eTaskResult;
	int m_eTaskResult;
    BCLib::uint8 m_eVerifyResult; // 对应于 EVerifyResult 中的值
    AccountID m_account;
    char m_name[ACCOUNT_NAME_LEN + 1];
    BCLib::uint32 m_key;
    BCLib::uint16 m_serverID;
    BCLib::uint16 m_bufSize;
    BCLib::int8 m_bufData[0];
    SMsgS2CResQueryAccountNamePwd() : SNetMessage(SSLib::EMT_ACCOUNTSERVER, EMID_S2C_RES_QUERY_ACCOINT_NAME_PWD)
    {
        m_eVerifyResult = eVerifyRes_Unknown;
        m_account = INVALID_ACCOUNT_ID;
        memset(m_name,0,ACCOUNT_NAME_LEN + 1);
        m_key = 0;
        m_serverID = 0;
        m_bufSize = 0;
    }
    void setName(const std::string& text)
    {
        BCLib::Utility::CStrHelper::strNCpy(m_name, sizeof(m_name), text.c_str(), ACCOUNT_NAME_LEN);
    }
};

BCLIB_MSGDEBUG(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_REQ_QUERY_ACCOUNT_NAME_KEY);
struct SMsgC2SReqQueryAccountNameKey : public SSLib::Message::SNetMessage
{
    char m_name[ACCOUNT_NAME_LEN + 1];
    char m_flag[ACCOUNT_FLAG_LEN+1];
    BCLib::uint32 m_key;
    BCLib::uint16 m_serverID;
    SMsgC2SReqQueryAccountNameKey() : SNetMessage(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_REQ_QUERY_ACCOUNT_NAME_KEY)
    {
        memset(m_name,0,ACCOUNT_NAME_LEN + 1);
        memset(m_flag,0,ACCOUNT_FLAG_LEN + 1);
        m_key = 0;
        m_serverID = 0;
    }

    void setName(const std::string& text)
    {
        BCLib::Utility::CStrHelper::strNCpy(m_name, sizeof(m_name), text.c_str(), ACCOUNT_NAME_LEN);
    }
    void setFlag(const std::string& text)
    {
        BCLib::Utility::CStrHelper::strNCpy(m_flag, sizeof(m_flag), text.c_str(), ACCOUNT_FLAG_LEN);
    }
};

BCLIB_MSGDEBUG(SSLib::EMT_ACCOUNTSERVER, EMID_S2C_RES_QUERY_ACCOINT_NAME_KEY);
struct SMsgS2CResQueryAccountNameKey : public SSLib::Message::SNetMessage
{
public:
    bool m_isright;
    char m_name[ACCOUNT_NAME_LEN + 1];
    char m_flag[ACCOUNT_FLAG_LEN+1];
    SMsgS2CResQueryAccountNameKey() : SNetMessage(SSLib::EMT_ACCOUNTSERVER, EMID_S2C_RES_QUERY_ACCOINT_NAME_KEY)
    {
        memset(m_name,0,ACCOUNT_NAME_LEN + 1);
        memset(m_flag,0,ACCOUNT_FLAG_LEN + 1);
        m_isright = false;
    }
    void setName(const std::string& text)
    {
        BCLib::Utility::CStrHelper::strNCpy(m_name, sizeof(m_name), text.c_str(), ACCOUNT_NAME_LEN);
    }
    void setFlag(const std::string& text)
    {
        BCLib::Utility::CStrHelper::strNCpy(m_flag, sizeof(m_flag), text.c_str(), ACCOUNT_FLAG_LEN);
    }
};

//
BCLIB_MSGDEBUG(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_REQ_VERIFY_NAME_PWD);
class CMsgC2SReqVerifyNamePwd : public SSLib::Message::CBufMessage<PTBuf::CAccountVerify>
{
public:
	CMsgC2SReqVerifyNamePwd() : SSLib::Message::CBufMessage<PTBuf::CAccountVerify>(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_REQ_VERIFY_NAME_PWD)
	{
	}
};

BCLIB_MSGDEBUG(SSLib::EMT_ACCOUNTCLIENT, EMID_S2C_RES_VERIFY_NAME_PWD);
class CMsgS2CResVerifyNamePwd : public SSLib::Message::CBufMessage<PTBuf::CAccountVerify>
{
public:
	CMsgS2CResVerifyNamePwd() : SSLib::Message::CBufMessage<PTBuf::CAccountVerify>(SSLib::EMT_ACCOUNTCLIENT, EMID_S2C_RES_VERIFY_NAME_PWD)
	{
	}
};

//
BCLIB_MSGDEBUG(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_REQ_INSERT_NAME_PWD);
class CMsgC2SReqInsertNamePwd : public SSLib::Message::CBufMessage<PTBuf::CAccountVerify>
{
public:
	CMsgC2SReqInsertNamePwd() : SSLib::Message::CBufMessage<PTBuf::CAccountVerify>(SSLib::EMT_ACCOUNTSERVER, EMID_C2S_REQ_INSERT_NAME_PWD)
	{
	}
};

BCLIB_MSGDEBUG(SSLib::EMT_ACCOUNTCLIENT, EMID_S2C_RES_INSERT_NAME_PWD);
class CMsgS2CResInsertNamePwd : public SSLib::Message::CBufMessage<PTBuf::CAccountVerify>
{
public:
	CMsgS2CResInsertNamePwd() : SSLib::Message::CBufMessage<PTBuf::CAccountVerify>(SSLib::EMT_ACCOUNTCLIENT, EMID_S2C_RES_INSERT_NAME_PWD)
	{
	}
};

#ifdef WIN32
#pragma pack (pop)
#else
#pragma pack ()
#endif

}//AccountServer
}//SSLib

#endif//__SSLIB_ACCOUNTSET_ACCOUNTSERVER_MESSAGE_ACCOUNTSERVERMSG_H__
