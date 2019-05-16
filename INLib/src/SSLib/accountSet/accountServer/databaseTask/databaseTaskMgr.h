//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SSLib/accountSet/accountServer/databaseTask/databaseTaskMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SSLIB_ACCOUNTSET_ACCOUNTSERVER_DATABASETASK_DATABASETASKMGR_H__
#define __SSLIB_ACCOUNTSET_ACCOUNTSERVER_DATABASETASK_DATABASETASKMGR_H__

#include <BCLib/utility/singleton.h>
#include <BCLib/utility/objectPool.h>
#include <BCLib/utility/hashMap.h>
#include <BCLib/utility/timer.h>
#include <BCLib/utility/thread/thread.h>
#include <BCLib/database/task/databaseTaskMgr.h>

#include <SSLib/accountSet/accountServer/message/accountServerMsg.h>

#include "../databaseMgr/databaseMgr.h"
#include "../network/netStub.h"
#include "../network/netStubMgr.h"

#include "task/accountAuthTask.h"
#include "task/namePwdTask.h"

namespace SSLib
{
namespace AccountServer
{

#define CDATABASETASKMGR_TASK_DECLARE(_KEYNAME_, _PTBUF_)																							\
public:																																				\
	C##_KEYNAME_##Task *	create##_KEYNAME_##Task(_PTBUF_ ptBuf, _KEYNAME_##Reply);																\
	void					relase##_KEYNAME_##Task(C##_KEYNAME_##Task *);																			\
	static BCLib::int32     on##_KEYNAME_##Reply(BCLib::Network::TcpStubID stubID, EchoID echoID, BCLib::Database::EDBTaskResult, _PTBUF_ ptBuf);	\
private:																																			\
	BCLib::Utility::CMutex							m_mutex##_KEYNAME_##Task;																		\
	BCLib::Utility::CObjectPool<C##_KEYNAME_##Task>	m_pool##_KEYNAME_##Task;

//////////////////////////////////////////////////////////////////////////

class CDatabaseTaskMgr : public BCLib::Database::CDatabaseTaskMgr
{
    BCLIB_SINGLETON_DECLARE(CDatabaseTaskMgr);

public:
    typedef BCLib::Utility::CHashMap<std::string, BCLib::uint16*> ServerHashMap;

public:
    CDatabaseTaskMgr();
    virtual ~CDatabaseTaskMgr();

    ServerHashMap&          getServerHmap() {return m_serverHashMap;}

protected:
    virtual void    _processTask(BCLib::Database::CDatabaseTask * pTaskReply);

	CDATABASETASKMGR_TASK_DECLARE(InsertAccountAuth, PTBuf::CAccountAuth);
	CDATABASETASKMGR_TASK_DECLARE(SelectAccountAuth, PTBuf::CAccountAuth);
	CDATABASETASKMGR_TASK_DECLARE(UpdateAccountAuth, PTBuf::CAccountAuth);
	CDATABASETASKMGR_TASK_DECLARE(DeleteAccountAuth, PTBuf::CAccountAuth);

	CDATABASETASKMGR_TASK_DECLARE(VerifyNamePwd, PTBuf::CAccountVerify);
	CDATABASETASKMGR_TASK_DECLARE(InsertNamePwd, PTBuf::CAccountVerify);

public:
	static BCLib::int32     onLoginReply(BCLib::Network::TcpStubID stubID, EchoID echoID, BCLib::Database::EDBTaskResult errCode, PTBuf::CAccountAuth ptBuf);
    static BCLib::int32     onQueryAccountNamePwdReply(BCLib::Network::TcpStubID stubID, EchoID echoID, BCLib::Database::EDBTaskResult, PTBuf::CAccountAuth ptBuf);
    static BCLib::int32     onAccountSetEssOkayReply(BCLib::Network::TcpStubID stubID, EchoID echoID, BCLib::Database::EDBTaskResult, PTBuf::CAccountAuth ptBuf);

    //static BCLib::int32     onVerifyNamePwdReply(BCLib::Network::TcpStubID stubID, EchoID echoID, EDBTaskResult errCode, PTBuf::CAccountAuth ptBuf);

private:
    ServerHashMap       m_serverHashMap;
};

}//AccountServer
}//SSLib

#endif//__SSLIB_ACCOUNTSET_ACCOUNTSERVER_DATABASETASK_DATABASETASKMGR_H__
