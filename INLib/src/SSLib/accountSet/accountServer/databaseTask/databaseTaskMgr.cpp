//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SSLib/accountSet/accountServer/databaseTask/databaseTaskMgr.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////

#include <BCLib/utility/logFile.h>
#include "databaseTaskMgr.h"
#include <BCLib/utility/string.h>
#include <BCLib/security/md5.h>

namespace SSLib
{
namespace AccountServer
{

#define CDATABASETASKMGR_TASK_PROCESS(_CASENAME_, _KEYNAME_)				\
	case EDB_TASK_TYPE_##_CASENAME_ :										\
	relase##_KEYNAME_##Task(dynamic_cast<C##_KEYNAME_##Task *>(pTaskReply));\
	break;

//////////////////////////////////////////////////////////////////////////

#define CDATABASETASKMGR_TASK_DEFINE(_KEYNAME_, _PTBUF_)												\
C##_KEYNAME_##Task * CDatabaseTaskMgr::create##_KEYNAME_##Task(_PTBUF_ ptBuf, _KEYNAME_##Reply reply)	\
{																										\
    BCLib::Utility::CMutexFun fun(&m_mutex##_KEYNAME_##Task);											\
    return m_pool##_KEYNAME_##Task.construct(ptBuf, reply);												\
}																										\
void CDatabaseTaskMgr::relase##_KEYNAME_##Task(C##_KEYNAME_##Task * pTask)								\
{																										\
    BCLib::Utility::CMutexFun fun(&m_mutex##_KEYNAME_##Task);											\
    if (pTask) m_pool##_KEYNAME_##Task.destroy(pTask);													\
}																										\
BCLib::int32 CDatabaseTaskMgr::on##_KEYNAME_##Reply(BCLib::Network::TcpStubID stubID, EchoID echoID, BCLib::Database::EDBTaskResult errCode, _PTBUF_ ptBuf) \
{																										\
	CMsgS2CRes##_KEYNAME_ msgRes;																		\
	msgRes.setEcho(echoID);																				\
	msgRes.setResult(errCode);																			\
	msgRes.serialize(&ptBuf);																			\
																										\
	BCLib::Utility::CStream tmpStream;																	\
	msgRes.serializeTo(tmpStream);																		\
	if (!CTcpStubMgr::singleton().sendMsg(stubID, tmpStream))											\
	{																									\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "发送消息失败");										\
	}																									\
																										\
	return 1;																							\
}

//////////////////////////////////////////////////////////////////////////

#define CDATABASETASKMGR_TASK_DEFINE_FOR_RECYCLE(_KEYNAME_, _PTBUF_)									\
C##_KEYNAME_##Task * CDatabaseTaskMgr::create##_KEYNAME_##Task(_PTBUF_ ptBuf, _KEYNAME_##Reply reply)	\
{																										\
    BCLib::Utility::CMutexFun fun(&m_mutex##_KEYNAME_##Task);											\
    return m_pool##_KEYNAME_##Task.construct(ptBuf, reply);												\
}																										\
void CDatabaseTaskMgr::relase##_KEYNAME_##Task(C##_KEYNAME_##Task * pTask)								\
{																										\
    BCLib::Utility::CMutexFun fun(&m_mutex##_KEYNAME_##Task);											\
    if (pTask) m_pool##_KEYNAME_##Task.destroy(pTask);													\
}																										\
BCLib::int32 CDatabaseTaskMgr::on##_KEYNAME_##Reply(BCLib::Network::TcpStubID stubID, EchoID echoID, BCLib::Database::EDBTaskResult errCode, _PTBUF_ ptBuf) \
{																										\
	CMsgS2CRes##_KEYNAME_ msgRes;																		\
	msgRes.setEcho(echoID);																				\
	msgRes.setResult(errCode);																			\
	msgRes.serialize(&ptBuf);																			\
																										\
	BCLib::Utility::CStream tmpStream;																	\
	msgRes.serializeTo(tmpStream);																		\
	if (!CTcpStubMgr::singleton().sendMsg(stubID, tmpStream))											\
	{																									\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "发送消息失败");										\
	}																									\
																										\
	CNetStubPtr netStub = CTcpStubMgr::singleton().getStubByStubID(stubID);								\
	if (!netStub)																						\
	{																									\
	    BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "onVerifyReply : no netStub!!!");					\
	    return 0;																						\
	}																									\
	if (!netStub->hasConnection())																		\
	{																									\
	    BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "onVerifyReply : !netStub->hasConnection()");		\
	    return 0;																						\
	}																									\
	netStub->setState(BCLib::Network::ESS_RECYCLE);														\
																										\
	return 1;																							\
}

//////////////////////////////////////////////////////////////////////////
BCLIB_SINGLETON_DEFINE(CDatabaseTaskMgr);

CDatabaseTaskMgr::CDatabaseTaskMgr()
{
}

CDatabaseTaskMgr::~CDatabaseTaskMgr()
{
}

void CDatabaseTaskMgr::_processTask(BCLib::Database::CDatabaseTask * pTaskReply)
{
    if (!pTaskReply->reply())
    {
        BCLIB_LOG_DEBUG(BCLib::ELOGMODULE_DEFAULT, "Task Manager,Task(%d) Replied 'False'!", pTaskReply->operator BCLib::int32 &());
    }

    switch (pTaskReply->type())
    {
		CDATABASETASKMGR_TASK_PROCESS(INSERT_ACCOUNT_AUTH, InsertAccountAuth)
		CDATABASETASKMGR_TASK_PROCESS(SELECT_ACCOUNT_AUTH, SelectAccountAuth)
		CDATABASETASKMGR_TASK_PROCESS(UPDATE_ACCOUNT_AUTH, UpdateAccountAuth)
		CDATABASETASKMGR_TASK_PROCESS(DELETE_ACCOUNT_AUTH, DeleteAccountAuth)

		CDATABASETASKMGR_TASK_PROCESS(VERIFY_NAME_PWD, VerifyNamePwd)
		CDATABASETASKMGR_TASK_PROCESS(INSERT_NAME_PWD, InsertNamePwd)
    default:
        BCLIB_LOG_DEBUG(BCLib::ELOGMODULE_DEFAULT, "Task Manager,Task Without Type!");
    }
}

CDATABASETASKMGR_TASK_DEFINE(InsertAccountAuth, PTBuf::CAccountAuth)
CDATABASETASKMGR_TASK_DEFINE(SelectAccountAuth, PTBuf::CAccountAuth)
CDATABASETASKMGR_TASK_DEFINE(UpdateAccountAuth, PTBuf::CAccountAuth)
CDATABASETASKMGR_TASK_DEFINE(DeleteAccountAuth, PTBuf::CAccountAuth)

CDATABASETASKMGR_TASK_DEFINE_FOR_RECYCLE(VerifyNamePwd, PTBuf::CAccountVerify)
CDATABASETASKMGR_TASK_DEFINE_FOR_RECYCLE(InsertNamePwd, PTBuf::CAccountVerify)

//////////////////////////////////////////////////////////////////////////

BCLib::int32 CDatabaseTaskMgr::onLoginReply(BCLib::Network::TcpStubID stubID, EchoID echoID, BCLib::Database::EDBTaskResult errCode, PTBuf::CAccountAuth ptBuf)
{
	CNetStubPtr netStub = CTcpStubMgr::singleton().getStubByStubID(stubID);
	if (!netStub)
	{
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "onLoginReply : no netStub!!!");
		return 0;
	}

	if (!netStub->hasConnection())
	{
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "onLoginReply : !netStub->hasConnection()");
		return 0;
	}

	SMsgS2CResLogin resLogin;
	resLogin.setEcho(echoID);

	if (errCode == BCLib::Database::EDB_TASK_ERROR_NOCONNECTION)
	{
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "onLoginReply : errCode == EDB_TASK_ERROR_NOCONNECTION");
		resLogin.m_eTaskResult = BCLib::Database::EDB_TASK_ERROR_NOCONNECTION;
		goto cut;
	}

	goto setok;

cut:
	netStub->sendMsg(&resLogin, sizeof(resLogin));
	netStub->setState(BCLib::Network::ESS_RECYCLE);
	return 0;

setok:
	resLogin.m_eTaskResult = BCLib::Database::EDB_TASK_ERROR_OK;
	resLogin.m_accountID = ptBuf.accountid();
	netStub->setState(BCLib::Network::ESS_OKAY);
	netStub->setAccountAuth(ptBuf);
	netStub->sendMsg(&resLogin, sizeof(resLogin));
	return 1;
}

BCLib::int32 CDatabaseTaskMgr::onQueryAccountNamePwdReply(BCLib::Network::TcpStubID stubID, EchoID echoID, BCLib::Database::EDBTaskResult errCode, PTBuf::CAccountAuth ptBuf)
 {
     //
     SMsgS2CResQueryAccountNamePwd resMsg;
     resMsg.setEcho(echoID);
     resMsg.m_eTaskResult = errCode;

     //if (pAccount->getIsRight())
     //{
     //    resMsg.m_eVerifyResult = SSLib::AccountServer::eVerifyRes_OK;
     //    if (!singleton().getKeyHmap().getValue(pAccount->getAccountName() + SAccount02::OriginToFlag(pAccount->getAccountOrigin()),resMsg.m_key))
     //    {
     //        resMsg.m_key = GetCurrentTime();
     //        singleton().getKeyHmap().setValue(pAccount->getAccountName() + SAccount02::OriginToFlag(pAccount->getAccountOrigin()),resMsg.m_key);
     //    }
     //    SAccount01 *pQueryAccount = CCacheManager::singleton().createQueryAccount();
     //    if (pQueryAccount == NULL)
     //    {
     //        BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "CCacheManager memory full");
     //        return 0;
     //    }
     //    pQueryAccount->setAccountName(pAccount->getAccountName().c_str());
     //    pQueryAccount->setAccountPwd(pAccount->getAccountPwd().c_str());
     //    pQueryAccount->setAccountOrigin(pAccount->getAccountOrigin());
     //    CCacheManager::singleton().setQueryAccount(pQueryAccount);
     //}
     //else
     //{
     //    resMsg.m_eVerifyResult = SSLib::AccountServer::eVerifyRes_Error;
     //    resMsg.m_key = 0;
     //}
     //resMsg.setName(pAccount->getAccountName());

	 resMsg.m_eVerifyResult = SSLib::AccountServer::eVerifyRes_OK;
	 //if (!singleton().getKeyHmap().getValue(refAccountAuth.accountname() + SAccount02::OriginToFlag(pAccount->getAccountOrigin()),resMsg.m_key))
	 //{
		// resMsg.m_key = GetCurrentTime();
		// singleton().getKeyHmap().setValue(refAccountAuth.accountname() + SAccount02::OriginToFlag(pAccount->getAccountOrigin()),resMsg.m_key);
	 //}
	 //SAccount01 *pQueryAccount = CCacheManager::singleton().createQueryAccount();
	 //if (pQueryAccount == NULL)
	 //{
		// BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "CCacheManager memory full");
		// return 0;
	 //}
	 //pQueryAccount->setAccountName(refAccountAuth.accountname());
	 //pQueryAccount->setAccountPwd(refAccountAuth.accountpwd());
	 //pQueryAccount->setAccountOrigin(refAccountAuth.accountorigin());
	 //CCacheManager::singleton().setQueryAccount(pQueryAccount);

	 resMsg.setName(ptBuf.accountname());

     if (!CTcpStubMgr::singleton().sendMsg(stubID, &resMsg, sizeof(resMsg)))
     {
         BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "与客户端断开连接，无法发送消息");
     }

     CNetStubPtr netStub = CTcpStubMgr::singleton().getStubByStubID(stubID);
     if (netStub)
     {
         netStub->setState(BCLib::Network::ESS_RECYCLE);
     }
     return 1;
 }

BCLib::int32 CDatabaseTaskMgr::onAccountSetEssOkayReply(BCLib::Network::TcpStubID stubID, EchoID echoID, BCLib::Database::EDBTaskResult errCode, PTBuf::CAccountAuth ptBuf)
{
    CNetStubPtr netStub = CTcpStubMgr::singleton().getStubByStubID(stubID);
    if (!netStub)
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "onReqAccountSetESSOkayReply : !netStub!!!");
        return 0;
    }
    if (!netStub->hasConnection())
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "onReqAccountSetESSOkayReply : !netStub->hasConnection()");
        return 0;
    }

	if (errCode == BCLib::Database::EDB_TASK_ERROR_NOCONNECTION)
	{
		netStub->setState(BCLib::Network::ESS_RECYCLE);
		return 0;
	}

	netStub->setState(BCLib::Network::ESS_OKAY);
	netStub->setAccountAuth(ptBuf);
	return 1;
}

}//AccountServer
}//SSLib