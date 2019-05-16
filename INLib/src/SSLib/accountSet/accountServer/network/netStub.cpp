//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SSLib/accountSet/accountServer/network/netStub.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/logFile.h>
#include <BCLib/security/hash.h>
#include <BCLib/framework/msgExec.h>
#include <BCLib/utility/convert.h>
#include <MWLib/xmlRpc/XmlRpc.h>
#include <SSLib/message/msgTypeSrv.h>
#include <SSLib/accountSet/accountServer/message/accountServerMsg.h>
#include <SSLib/accountSet/accountServer/protobuf/accountAuth.pb.h>
#include <SSLib/accountSet/accountServer/protobuf/accountVerify.pb.h>
#include "netStub.h"
#include "../accountServer.h"
#include "../accountApp.h"
#include "../databaseTask/databaseTaskMgr.h"

#define ACCOUNT_ACCREDIT  "25fsjnu7kls5g6y1qt"

namespace SSLib
{
namespace AccountServer
{

// 权限判断
class CPermission
{
public:
    CPermission(){}
    ~CPermission(){}
    static bool canSel(BCLib::uint32 uPermission) {return (uPermission & 0x1) > 0;}
    static bool canAdd(BCLib::uint32 uPermission) {return (uPermission & 0x2) > 0;}
    static bool canMod(BCLib::uint32 uPermission) {return (uPermission & 0x4) > 0;}
    static bool canDel(BCLib::uint32 uPermission) {return (uPermission & 0x8) > 0;}
    static bool canSelKey(BCLib::uint32 uPermission) {return (uPermission & 0x10) > 0;}
    static bool canSelMetaData(BCLib::uint32 uPermission) {return (uPermission & 0x20) > 0;}
    static bool canModMetaData(BCLib::uint32 uPermission) {return (uPermission & 0x40) > 0;}
};

CTcpStub::CTcpStub(const BCLib::Network::CTcpConnectionSPtr& netConn)
: BCLib::Network::CTcpStub(netConn)
{
}

CTcpStub::~CTcpStub()
{
    ;
}

bool CTcpStub::sendMsg(const BCLib::Framework::CMessage& msg)
{
    BCLib::Utility::CStream stream;
    if (!msg.serializeTo(stream)) {
        return false;
    }
    return BCLib::Network::CTcpStub::send(stream) > 0;
}

bool CTcpStub::sendMsg(const BCLib::Framework::SMessage* msg, const BCLib::uint32 msgSize)
{
    return BCLib::Network::CTcpStub::send(msg, msgSize) > 0;
}

bool CTcpStub::_cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize)
{
    BCLib::Framework::SThdMsgLabel msgLabel;
    msgLabel.m_msgSize = msgSize;

    BCLib::Framework::SMessage* msg = (BCLib::Framework::SMessage*)msgBuff;
    if (BCLib::Framework::CMsgExecMgr::executeMessage(&msgLabel, msg)) {
        return true;
    }

    BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "无法解析消息type[%d]id[%d]size[%d]", msg->getType(), msg->getID(), msgSize);
    return false;
}

void CTcpStub::_cbTerminate()
{
    ;
}

bool CTcpStub::_createMsgExecPtr(BCLib::uint16 type, BCLib::uint16 id, BCLib::Framework::CMsgExecPtr& msgExecPtr)
{
    BCLIB_MSGEXEC_DEFINE_BEGIN(type)
        BCLIB_MSGEXEC_DEFINE_TYPE_BEGIN(SSLib::EMT_ACCOUNTSERVER, id)
            BCLIB_MSGEXEC_DEFINE_ID(msgExecPtr, EMID_C2S_ACCOUNT_SET_ESS_OKAY, new BCLib::Framework::CMsgExec(&CTcpStub::_onC2SReqAccountSetESSOkay, this))
            BCLIB_MSGEXEC_DEFINE_ID(msgExecPtr, EMID_C2S_REQ_LOGIN, new BCLib::Framework::CMsgExec(&CTcpStub::_onC2SReqLogin, this))
            BCLIB_MSGEXEC_DEFINE_ID(msgExecPtr, EMID_C2S_REQ_INSERT_ACCOUNT_AUTH, new BCLib::Framework::CMsgExec(&CTcpStub::_onC2SReqInsertAccountAuth, this))
            BCLIB_MSGEXEC_DEFINE_ID(msgExecPtr, EMID_C2S_REQ_SELECT_ACCOUNT_AUTH, new BCLib::Framework::CMsgExec(&CTcpStub::_onC2SReqSelectAccountAuth, this))
            BCLIB_MSGEXEC_DEFINE_ID(msgExecPtr, EMID_C2S_REQ_UPDATE_ACCOUNT_AUTH, new BCLib::Framework::CMsgExec(&CTcpStub::_onC2SReqUpdateAccountAuth, this))
            BCLIB_MSGEXEC_DEFINE_ID(msgExecPtr, EMID_C2S_REQ_DELETE_ACCOUNT_AUTH, new BCLib::Framework::CMsgExec(&CTcpStub::_onC2SReqDeleteAccountAuth, this))
            BCLIB_MSGEXEC_DEFINE_ID(msgExecPtr, EMID_C2S_REQ_QUERY_ACCOUNT_NAME_KEY, new BCLib::Framework::CMsgExec(&CTcpStub::_onEXS2SReqQueryAccount, this))
            BCLIB_MSGEXEC_DEFINE_ID(msgExecPtr, EMID_C2S_REQ_QUERY_ACCOUNT_NAME_PWD, new BCLib::Framework::CMsgExec(&CTcpStub::_onSS2ASReqQueryAccount, this))
            BCLIB_MSGEXEC_DEFINE_ID(msgExecPtr, EMID_C2S_REQ_VERIFY_NAME_PWD, new BCLib::Framework::CMsgExec(&CTcpStub::_onC2SReqVerifyNamePwd, this))
			BCLIB_MSGEXEC_DEFINE_ID(msgExecPtr, EMID_C2S_REQ_INSERT_NAME_PWD, new BCLib::Framework::CMsgExec(&CTcpStub::_onC2SReqInsertNamePwd, this))
        BCLIB_MSGEXEC_DEFINE_TYPE_END
    BCLIB_MSGEXEC_DEFINE_END
    return true;
}

bool CTcpStub::_createMsgExecPtr(BCLib::uint16 type, BCLib::Framework::CMsgExecPtr& msgExecPtr)
{
    return true;
}

void CTcpStub::_onC2SReqAccountSetESSOkay(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg)
{
    //BCLib::Network::CTcpStub::setState(BCLib::Network::ESS_OKAY);
    SMsgC2SAccountSetEssOkay* msgReq = (SMsgC2SAccountSetEssOkay*)msg;

	PTBuf::CAccountAuth ptBuf;
	//reqInsertAccountAuth.Deserialize(&ptBuf);
	ptBuf.set_accountname(msgReq->m_name);
	ptBuf.set_accountpwd(msgReq->m_pwd);
	std::string accountName = ptBuf.accountname();

    CSelectAccountAuthTask* pTask = CDatabaseTaskMgr::singleton().createSelectAccountAuthTask(ptBuf, CDatabaseTaskMgr::onAccountSetEssOkayReply);
    if (pTask == NULL)
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "pTask == NULL, accountName = %s", accountName.c_str());
		return;
    }

    pTask->setEchoID(msgReq->getEcho());
    pTask->setStubID(this->getStubID());
    if (!CDatabaseTaskMgr::singleton().addTask(CDatabaseTaskMgr::singleton().hash(BCLib::Security::CHash::getBKDRHash(accountName.c_str())), pTask))
	{
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "!CDatabaseTaskMgr::singleton().addTask(pTask), accountName = %s", accountName.c_str());
		CDatabaseTaskMgr::singleton().relaseSelectAccountAuthTask(pTask);
    }
}

void CTcpStub::_onC2SReqLogin(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg)
{
    SMsgC2SReqLogin* msgReq = (SMsgC2SReqLogin*)msg;

	PTBuf::CAccountAuth ptBuf;
	//reqInsertAccountAuth.Deserialize(&ptBuf);
	ptBuf.set_accountname(msgReq->m_name);
	ptBuf.set_accountpwd(msgReq->m_pwd);
	std::string accountName = ptBuf.accountname();

    CSelectAccountAuthTask* pTask = CDatabaseTaskMgr::singleton().createSelectAccountAuthTask(ptBuf, CDatabaseTaskMgr::onLoginReply);
    if (pTask == NULL)
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "pTask == NULL, accountName = %s", accountName.c_str());
        return;
    }

    pTask->setEchoID(msgReq->getEcho());
    pTask->setStubID(this->getStubID());
    if (!CDatabaseTaskMgr::singleton().addTask(CDatabaseTaskMgr::singleton().hash(BCLib::Security::CHash::getBKDRHash(accountName.c_str())), pTask))
	{
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "!CDatabaseTaskMgr::singleton().addTask(pTask), accountName = %s", accountName.c_str());
		CDatabaseTaskMgr::singleton().relaseSelectAccountAuthTask(pTask);
    }
}

void CTcpStub::_onC2SReqInsertAccountAuth(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg)
{
    if (!CPermission::canAdd(m_AccountAuth.permission()))
    {
        return;
    }

	CMsgC2SReqInsertAccountAuth msgReq;
	msgReq.serializeFrom(msg,msgLabel->m_msgSize);

	PTBuf::CAccountAuth ptBuf;
	msgReq.deserialize(&ptBuf);
	std::string accountName = ptBuf.accountname();

	// 排除法律禁止的字符串
    if (CAccountServer::singleton().hasUnLawKeyWord(accountName))
    {
        CMsgS2CResInsertAccountAuth msgRes;
        msgRes.setEcho(msgReq.getEcho());
        msgRes.setResult(BCLib::Database::EDB_TASK_ERROR_ILLEGAL_STRING);
		sendMsg(msgRes);
        return;
    }

    CInsertAccountAuthTask* pTask = CDatabaseTaskMgr::singleton().createInsertAccountAuthTask(ptBuf, CDatabaseTaskMgr::onInsertAccountAuthReply);
    if (pTask == NULL)
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "pTask == NULL, accountName = %s", accountName.c_str());
        return;
    }

    pTask->setEchoID(msgReq.getEcho());
    pTask->setStubID(this->getStubID());
	if (!CDatabaseTaskMgr::singleton().addTask(CDatabaseTaskMgr::singleton().hash(BCLib::Security::CHash::getBKDRHash(accountName.c_str())), pTask))
	{
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "!CDatabaseTaskMgr::singleton().addTask(pTask), accountName = %s", accountName.c_str());
		CDatabaseTaskMgr::singleton().relaseInsertAccountAuthTask(pTask);
    }
}

void CTcpStub::_onC2SReqSelectAccountAuth(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg)
{
	CMsgC2SReqSelectAccountAuth msgReq;
	msgReq.serializeFrom(msg,msgLabel->m_msgSize);

	PTBuf::CAccountAuth ptBuf;
	msgReq.deserialize(&ptBuf);
	std::string accountName = ptBuf.accountname();

    CSelectAccountAuthTask* pTask = CDatabaseTaskMgr::singleton().createSelectAccountAuthTask(ptBuf, CDatabaseTaskMgr::onSelectAccountAuthReply);
    if (pTask == NULL)
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "pTask == NULL, accountName = %s", accountName.c_str());
        return;
    }

    pTask->setEchoID(msgReq.getEcho());
    pTask->setStubID(this->getStubID());
    if (!CDatabaseTaskMgr::singleton().addTask(CDatabaseTaskMgr::singleton().hash(BCLib::Security::CHash::getBKDRHash(accountName.c_str())), pTask))
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "!CDatabaseTaskMgr::singleton().addTask(pTask), accountName = %s", accountName.c_str());
        CDatabaseTaskMgr::singleton().relaseSelectAccountAuthTask(pTask);
    }

	//CInsertAccountAuthTask* pTask = CDatabaseTaskMgr::singleton().createInsertAccountAuthTask(ptBuf, CDatabaseTaskMgr::onInsertAccountAuthReply);
	//if (pTask == NULL)
	//{
	//	BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "pTask == NULL, accountName = %s", accountName.c_str());
	//	return;
	//}

	//pTask->setEchoID(msgReq.getEcho());
	//pTask->setStubID(this->getStubID());
	//if (!CDatabaseTaskMgr::singleton().addTask(CDatabaseTaskMgr::singleton().hash(CDatabaseMgr::singleton().getHash(accountName.c_str())), pTask))
	//{
	//	BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "!CDatabaseTaskMgr::singleton().addTask(pTask), accountName = %s", accountName.c_str());
	//	CDatabaseTaskMgr::singleton().relaseInsertAccountAuthTask(pTask);
	//}

	//CUpdateAccountAuthTask* pTask = CDatabaseTaskMgr::singleton().createUpdateAccountAuthTask(ptBuf, CDatabaseTaskMgr::onUpdateAccountAuthReply);
	//if (pTask == NULL)
	//{
	//	BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "pTask == NULL, accountName = %s", accountName.c_str());
	//	return;
	//}

	//pTask->setEchoID(msgReq.getEcho());
	//pTask->setStubID(this->getStubID());
	//if (!CDatabaseTaskMgr::singleton().addTask(CDatabaseTaskMgr::singleton().hash(CDatabaseMgr::singleton().getHash(accountName.c_str())), pTask))
	//{
	//	BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "!CDatabaseTaskMgr::singleton().addTask(pTask), accountName = %s", accountName.c_str());
	//	CDatabaseTaskMgr::singleton().relaseUpdateAccountAuthTask(pTask);
	//}

	//CDeleteAccountAuthTask* pTask = CDatabaseTaskMgr::singleton().createDeleteAccountAuthTask(ptBuf, CDatabaseTaskMgr::onDeleteAccountAuthReply);
	//if (pTask == NULL)
	//{
	//	BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "pTask == NULL, accountName = %s", accountName.c_str());
	//	return;
	//}

	//pTask->setEchoID(msgReq.getEcho());
	//pTask->setStubID(this->getStubID());
	//if (!CDatabaseTaskMgr::singleton().addTask(pTask))
	//{
	//	BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "!CDatabaseTaskMgr::singleton().addTask(pTask), accountName = %s", accountName.c_str());
	//	CDatabaseTaskMgr::singleton().relaseDeleteAccountAuthTask(pTask);
	//}
}

void CTcpStub::_onC2SReqUpdateAccountAuth(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg)
{
	CMsgC2SReqUpdateAccountAuth msgReq;
	msgReq.serializeFrom(msg,msgLabel->m_msgSize);

	PTBuf::CAccountAuth ptBuf;
	msgReq.deserialize(&ptBuf);
	std::string accountName = ptBuf.accountname();

    if (!CPermission::canMod(m_AccountAuth.permission()) && m_AccountAuth.accountname() != accountName)
    {
        return;
    }

    CUpdateAccountAuthTask* pTask = CDatabaseTaskMgr::singleton().createUpdateAccountAuthTask(ptBuf, CDatabaseTaskMgr::onUpdateAccountAuthReply);
    if (pTask == NULL)
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "pTask == NULL, accountName = %s", accountName.c_str());
		return;
    }

    pTask->setEchoID(msgReq.getEcho());
    pTask->setStubID(this->getStubID());
    if (!CDatabaseTaskMgr::singleton().addTask(CDatabaseTaskMgr::singleton().hash(BCLib::Security::CHash::getBKDRHash(accountName.c_str())), pTask))
	{
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "!CDatabaseTaskMgr::singleton().addTask(pTask), accountName = %s", accountName.c_str());
		CDatabaseTaskMgr::singleton().relaseUpdateAccountAuthTask(pTask);
    }
}

void CTcpStub::_onC2SReqDeleteAccountAuth(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg)
{
    if (!CPermission::canDel(m_AccountAuth.permission()))
    {
        return;
    }

	CMsgC2SReqDeleteAccountAuth msgReq;
	msgReq.serializeFrom(msg,msgLabel->m_msgSize);

	PTBuf::CAccountAuth ptBuf;
	msgReq.deserialize(&ptBuf);
	std::string accountName = ptBuf.accountname();

    CDeleteAccountAuthTask* pTask = CDatabaseTaskMgr::singleton().createDeleteAccountAuthTask(ptBuf, CDatabaseTaskMgr::onDeleteAccountAuthReply);
    if (pTask == NULL)
    {
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "pTask == NULL, accountName = %s", accountName.c_str());
		return;
    }

    pTask->setEchoID(msgReq.getEcho());
    pTask->setStubID(this->getStubID());
    if (!CDatabaseTaskMgr::singleton().addTask(pTask))
	{
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "!CDatabaseTaskMgr::singleton().addTask(pTask), accountName = %s", accountName.c_str());
		CDatabaseTaskMgr::singleton().relaseDeleteAccountAuthTask(pTask);
    }
}

void CTcpStub::_onEXS2SReqQueryAccount(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg)
{
    if (!CPermission::canSelKey(m_AccountAuth.permission()))
    {
        return;
    }

    SMsgC2SReqQueryAccountNameKey* msgReq = (SMsgC2SReqQueryAccountNameKey*)msg;
    SMsgS2CResQueryAccountNameKey msgRes;

    std::string accountName(msgReq->m_name);
    std::string accountFlag(msgReq->m_flag);
    std::string nameFlag(accountName + accountFlag);
    BCLib::uint32 key;
    BCLib::Utility::CHashMap<std::string, BCLib::uint32>& keyMap = CDatabaseTaskMgr::singleton().getKeyHmap();
    if (keyMap.getValue(nameFlag, key))
    {
        if (key == msgReq->m_key)
        {
            BCLib::uint16* serverlist = NULL;
            if (CDatabaseTaskMgr::singleton().getServerHmap().getValue(nameFlag, serverlist) && serverlist)
            {
                for (BCLib::uint16 i = 1; i < serverlist[0]; i++ )
                {
                    //if (serverlist[i] == hiReqMsg->m_serverID)
                    {
                        msgRes.m_isright = true;
                        break;
                    }
                }
            }
            CDatabaseTaskMgr::singleton().getServerHmap().remove(nameFlag);
            delete[] serverlist;
        }
    }
    msgRes.setName(accountName);
    msgRes.setFlag(accountFlag);
    msgRes.setEcho(msgReq->getEcho());
    send(&msgRes,sizeof(msgRes));
}

void CTcpStub::_onSS2ASReqQueryAccount(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg)
{
    //setState(BCLib::Network::ESS_OKAY);
    SMsgC2SReqQueryAccountNamePwd* msgReq = (SMsgC2SReqQueryAccountNamePwd*)msg;
    std::string accredit(msgReq->m_accredit);
    if (accredit != "237hdwtjd92f8")
    {
        setState(BCLib::Network::ESS_RECYCLE);
        return;
    }
    std::string accountName(msgReq->m_name);
    if (accountName.length() < 3)
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "accountName 长度非法！！：%s",accountName.c_str());
        setState(BCLib::Network::ESS_RECYCLE);
        return;
    }
    for (BCLib::uint8 i = 0;i < accountName.length(); i++)
    {
        if (!((accountName[i]>47&&accountName[i]<58)||(accountName[i]>64&&accountName[i]<91)||(accountName[i]>96&&accountName[i]<123)||accountName[i]==95))
        {
            BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "accountName 非法字符！！：%s",accountName.c_str());
            setState(BCLib::Network::ESS_RECYCLE);
            return ;
        }
    }

	PTBuf::CAccountAuth ptBuf;
	//reqInsertAccountAuth.Deserialize(&ptBuf);
	ptBuf.set_accountname(msgReq->m_name);
	ptBuf.set_accountpwd(msgReq->m_pwd);

    CSelectAccountAuthTask* pTask = CDatabaseTaskMgr::singleton().createSelectAccountAuthTask(ptBuf, CDatabaseTaskMgr::onQueryAccountNamePwdReply);
    if (pTask == NULL)
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "pTask == NULL, accountName = %s", accountName.c_str());
		return;
    }

    pTask->setEchoID(msgReq->getEcho());
    pTask->setStubID(this->getStubID());
    if (!CDatabaseTaskMgr::singleton().addTask(CDatabaseTaskMgr::singleton().hash(BCLib::Security::CHash::getBKDRHash(accountName.c_str())), pTask))
	{
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "!CDatabaseTaskMgr::singleton().addTask(pTask), accountName = %s", accountName.c_str());
		CDatabaseTaskMgr::singleton().relaseSelectAccountAuthTask(pTask);
    }
}

void CTcpStub::_onC2SReqVerifyNamePwd(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg)
{
    CMsgC2SReqVerifyNamePwd msgReq;
    static BCLib::uint32 uCount = 1;
    BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "_onC2SReqVerifyNamePwd(%d)", uCount++);
    msgReq.serializeFrom(msg,msgLabel->m_msgSize);

    //if (msgReq.m_accredit != ACCOUNT_ACCREDIT)
    //{
    //    setState(BCLib::Network::ESS_RECYCLE);
    //    return;
    //}

	PTBuf::CAccountVerify ptBuf;
	msgReq.deserialize(&ptBuf);
	std::string accountName = ptBuf.accountname();

    CVerifyNamePwdTask* pTask = CDatabaseTaskMgr::singleton().createVerifyNamePwdTask(ptBuf, CDatabaseTaskMgr::onVerifyNamePwdReply);
    if (pTask == NULL)
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "pTask == NULL, accountName = %s", accountName.c_str());
        return;
    }

    pTask->setEchoID(msgReq.getEcho());
    pTask->setStubID(this->getStubID());
    if (!CDatabaseTaskMgr::singleton().addTask(CDatabaseTaskMgr::singleton().hash(BCLib::Security::CHash::getBKDRHash(accountName.c_str())), pTask))
	{
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "!CDatabaseTaskMgr::singleton().addTask(pTask), accountName = %s", accountName.c_str());
        CDatabaseTaskMgr::singleton().relaseVerifyNamePwdTask(pTask);
    }
}

void CTcpStub::_onC2SReqInsertNamePwd(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg)
{
	CMsgC2SReqInsertNamePwd msgReq;
	msgReq.serializeFrom(msg,msgLabel->m_msgSize);

	PTBuf::CAccountVerify ptBuf;
	msgReq.deserialize(&ptBuf);
	std::string accountName = ptBuf.accountname();

	// 排除法律禁止的字符串
	if (CAccountServer::singleton().hasUnLawKeyWord(accountName))
	{
		CMsgS2CResInsertAccountAuth msgRes;
		msgRes.setEcho(msgReq.getEcho());
		msgRes.setResult(BCLib::Database::EDB_TASK_ERROR_ILLEGAL_STRING);
		sendMsg(msgRes);
		return;
	}

	CInsertNamePwdTask* pTask = CDatabaseTaskMgr::singleton().createInsertNamePwdTask(ptBuf, CDatabaseTaskMgr::onInsertNamePwdReply);
	if (pTask == NULL)
	{
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "pTask == NULL, accountName = %s", accountName.c_str());
		return;
	}

	pTask->setEchoID(msgReq.getEcho());
	pTask->setStubID(this->getStubID());
	if (!CDatabaseTaskMgr::singleton().addTask(CDatabaseTaskMgr::singleton().hash(BCLib::Security::CHash::getBKDRHash(accountName.c_str())), pTask))
	{
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "!CDatabaseTaskMgr::singleton().addTask(pTask), accountName = %s", accountName.c_str());
		CDatabaseTaskMgr::singleton().relaseInsertNamePwdTask(pTask);
	}
}

}//AccountServer
}//SSLib
