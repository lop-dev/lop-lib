//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SSLib/accountSet/accountServer/databaseTask/task/namePwdTask.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <sstream>
#include <BCLib/utility/logFile.h>
#include <BCLib/utility/convert.h>
#include "namePwdTask.h"
#include "../databaseTaskMgr.h"

namespace SSLib
{
namespace AccountServer
{

CDATABASETASK_SUBCLASS_DEFINE(VerifyNamePwd, PTBuf::CAccountVerify, EDB_TASK_TYPE_VERIFY_NAME_PWD)
{
	CDATABASETASK_SUBCLASS_DEFINE_REPLY_Variable;

	std::string strName = m_ptBuf.accountname();
	if (strName == "")
	{
		m_errCode = BCLib::Database::EDB_TASK_ERROR_ILLEGAL_STRING;
		return false;
	}

	BCLib::Utility::CStringA tmpName = strName;
	HashIndex(tmpName, strDBIndex, strTBIndex);
	strDBName = getDBPrefix() + strDBIndex;
	strTBName = "AccountAuth" + strTBIndex;

	// 打印一下提示日志
	strName = "";
	BCLib::Utility::CConvert::utf8ToGB2312(strName, m_ptBuf.accountname());
	BCLIB_LOG_SYSTEM(BCLib::ELOGMODULE_DEFAULT, "VerifyNamePwd : %s (%s, %s)", strName.c_str(), strDBName.c_str(), strTBName.c_str());

	CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_UseDB;

    sqlSentence.str("");
	sqlSentence << "SELECT * FROM " << strTBName.c_str() <<" WHERE accountName ='" << m_ptBuf.accountname()<<"';";

	CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ExecuteReader;
	CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ExecuteReader_Check(true);

	PTBuf::CAccountVerify ptBuf;
	CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ExecuteReader_Next;
	CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadUint64(ptBuf, accountid);
	CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadString(ptBuf, accountname);
	CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadString(ptBuf, accountpwd);
	delete pDataReader;

	if (m_ptBuf.accountpwd() == ptBuf.accountpwd())
	{
		m_ptBuf.set_accountid(ptBuf.accountid());
		m_ptBuf.set_accountname(ptBuf.accountname());
		m_ptBuf.set_accountpwd(ptBuf.accountpwd());

		BCLib::uint64 uAccountID = m_ptBuf.accountid();
		m_ptBuf.set_accountkey(BCLib::Utility::CConvert::toStringA(uAccountID));
		m_errCode = BCLib::Database::EDB_TASK_ERROR_OK;
	}
	else
	{
		m_errCode = BCLib::Database::EDB_TASK_ERROR_PASSWORD;
	}

    return true;
}

CDATABASETASK_SUBCLASS_DEFINE(InsertNamePwd, PTBuf::CAccountVerify, EDB_TASK_TYPE_INSERT_NAME_PWD)
{
	CDATABASETASK_SUBCLASS_DEFINE_REPLY_Variable;

	std::string strName = m_ptBuf.accountname();
	if (strName == "")
	{
		m_errCode = BCLib::Database::EDB_TASK_ERROR_ILLEGAL_STRING;
		return false;
	}

	BCLib::Utility::CStringA tmpName = strName;
	HashIndex(tmpName, strDBIndex, strTBIndex);
	strDBName = getDBPrefix() + strDBIndex;
	strTBName = "AccountAuth" + strTBIndex;

	// 打印一下提示日志
	strName = "";
	BCLib::Utility::CConvert::utf8ToGB2312(strName, m_ptBuf.accountname());
	BCLIB_LOG_SYSTEM(BCLib::ELOGMODULE_DEFAULT, "InsertNamePwd : %s (%s, %s)", strName.c_str(), strDBName.c_str(), strTBName.c_str());

	CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_UseDB;

	sqlSentence.str("");
	sqlSentence << "SELECT * FROM " << strTBName.c_str() << " WHERE AccountName='" << m_ptBuf.accountname()<<"';";

	CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ExecuteReader;
	CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ExecuteReader_Check(false);

	//
	sqlSentence.str("");
	sqlSentence << "INSERT INTO " << strTBName.c_str() << " ";
	sqlSentence << "( ";
	sqlSentence << "AccountName, AccountPwd, Permission, RegisterIP, RegisterTime, LoginIP, LoginTime ";
	sqlSentence << ") ";
	sqlSentence << "VALUES ( ";
	sqlSentence << "'" + m_ptBuf.accountname() + "', ";
	sqlSentence << "'" + m_ptBuf.accountpwd() + "', ";
	sqlSentence << "'1','127.0.0.1','1753-01-01 00:00:00.0','127.0.0.1','1753-01-01 00:00:00.0'";
	sqlSentence << "); ";

	CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ExecuteNonQuery;

	//
	sqlSentence.str("");
	sqlSentence << "SELECT LAST_INSERT_ID() AS AccountID";

	CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ExecuteReader;
	CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ExecuteReader_Check(true);

	CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ExecuteReader_Next;
	CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadUint64(m_ptBuf, accountid);
	delete pDataReader;

	if (m_ptBuf.accountid() == 0)
	{
		m_errCode = BCLib::Database::EDB_TASK_ERROR_INSERT_RECORD;
		return false;
	}

	BCLib::uint64 uAccountID = m_ptBuf.accountid();
	m_ptBuf.set_accountkey(BCLib::Utility::CConvert::toStringA(uAccountID));

	m_errCode = BCLib::Database::EDB_TASK_ERROR_OK;
	return true;
}

}//AccountServer
}//SSLib