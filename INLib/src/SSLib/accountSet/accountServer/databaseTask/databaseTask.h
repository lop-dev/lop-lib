//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SSLib/accountSet/accountServer/databaseTask/databaseTask.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SSLIB_ACCOUNTSET_ACCOUNTSERVER_DATABASETASK_DATABASETASK_H__
#define __SSLIB_ACCOUNTSET_ACCOUNTSERVER_DATABASETASK_DATABASETASK_H__

#include <BCLib/database/task/databaseTask.h>
#include <BCLib/database/connection.h>
#include <BCLib/framework/serviceID.h>
#include <BCLib/utility/string.h>
#include <BCLib/utility/hashMap.h>
#include <BCLib/network/baseDef.h>

namespace SSLib
{
namespace AccountServer
{

enum EDBTaskType
{
    EDB_TASK_TYPE_INVALID_VALUE,

	EDB_TASK_TYPE_INSERT_ACCOUNT_AUTH,
    EDB_TASK_TYPE_SELECT_ACCOUNT_AUTH,
    EDB_TASK_TYPE_UPDATE_ACCOUNT_AUTH,
    EDB_TASK_TYPE_DELETE_ACCOUNT_AUTH,

	EDB_TASK_TYPE_VERIFY_NAME_PWD,
	EDB_TASK_TYPE_INSERT_NAME_PWD,
};

typedef BCLib::uint16 EchoID;

class CTcpStub;

class CDatabaseTask : public BCLib::Database::CDatabaseTask
{
public:
    CDatabaseTask();
    virtual ~CDatabaseTask();

	std::string getDBPrefix(){ return "AccountServer"; }

	bool HashIndex(BCLib::uint64 uHash, BCLib::Utility::CStringA& strDBIndex, BCLib::Utility::CStringA& strTBIndex);
	bool HashIndex(BCLib::Utility::CStringA& strHashKey, BCLib::Utility::CStringA& strDBIndex, BCLib::Utility::CStringA& strTBIndex);

	BCLib::Database::CConnection* GetConnection(BCLib::Database::CConnectionMap & mapConnections, std::string strDBIndex, std::string& strDBName);

public:
    void setStubID(BCLib::Network::TcpStubID  stubID){m_stubID = stubID;}

    void setEchoID(EchoID echoID){m_echoID = echoID;}
    EchoID getEchoID(){return m_echoID;}

protected:
    BCLib::Network::TcpStubID   m_stubID;
    EchoID                      m_echoID;
};

//////////////////////////////////////////////////////////////////////////

#define CDATABASETASK_SUBCLASS_DECLARE(_KEYNAME_, _PTBUF_)																\
typedef BCLib::int32 (*_KEYNAME_##Reply)(BCLib::Network::TcpStubID, EchoID, BCLib::Database::EDBTaskResult, _PTBUF_);	\
class C##_KEYNAME_##Task : public CDatabaseTask																			\
{																														\
public:																													\
	C##_KEYNAME_##Task(_PTBUF_ ptBuf, _KEYNAME_##Reply);																\
	virtual ~C##_KEYNAME_##Task();																						\
																														\
	virtual bool excute(BCLib::Database::CConnectionMap &);																\
	virtual bool reply();																								\
																														\
private:																												\
	_PTBUF_ m_ptBuf;																									\
	_KEYNAME_##Reply  m_funcReply;																						\
};

//////////////////////////////////////////////////////////////////////////

#define CDATABASETASK_SUBCLASS_DEFINE(_KEYNAME_, _PTBUF_, _TYPE_)										\
C##_KEYNAME_##Task::C##_KEYNAME_##Task(_PTBUF_ ptBuf, _KEYNAME_##Reply reply)							\
:m_ptBuf(ptBuf)																							\
,m_funcReply(reply)																						\
{																										\
	m_taskType = _TYPE_;																				\
}																										\
																										\
C##_KEYNAME_##Task::~C##_KEYNAME_##Task()																\
{																										\
}																										\
																										\
bool C##_KEYNAME_##Task::reply()																		\
{																										\
	BCLib::int32	i32Ret = -1;																		\
	if (m_funcReply)																					\
	{																									\
		i32Ret = m_funcReply(m_stubID, m_echoID, (BCLib::Database::EDBTaskResult)m_errCode, m_ptBuf);	\
	}																									\
	return i32Ret > 0;																					\
}																										\
																										\
bool C##_KEYNAME_##Task::excute(BCLib::Database::CConnectionMap& mapConnections)

//////////////////////////////////////////////////////////////////////////

#define CDATABASETASK_SUBCLASS_DEFINE_REPLY_Variable	\
	std::ostringstream              sqlSentence;		\
	BCLib::Utility::CStringA		strDBIndex = "";	\
	BCLib::Utility::CStringA		strTBIndex = "";	\
	std::string                     strDBName = "";		\
	std::string                     strTBName = "";		\
	BCLib::Database::CConnection *	pConnection = NULL;	\
	BCLib::Database::CDataReader *	pDataReader = NULL;	\
	double							dValue = 0;			\
	BCLib::int32					i32Value = 0;		\
	BCLib::int64					i64Value = 0;		\
	BCLib::uint32					u32Value = 0;		\
	BCLib::uint64					u64Value = 0;		\
	std::string						strValue = "";

#define CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_UseDB																\
	pConnection = GetConnection(mapConnections, strDBIndex, strDBName);												\
	if (pConnection == NULL)																						\
	{																												\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_NOCONNECTION;													\
		return false;																								\
	}																												\
																													\
	if (!pConnection->isConnected())																				\
	{																												\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_UNCONNECTED;													\
		return false;																								\
	}																												\
																													\
	sqlSentence.str("");																							\
	sqlSentence << "USE " << strDBName.c_str();																		\
	if (!pConnection->executeNonQuery(sqlSentence.str().c_str()))													\
	{																												\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "executeNonQuery : %s", pConnection->getErrorString().c_str());	\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_EXECUTE_NONQUERY;												\
		return false;																								\
	}

#define CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ExecuteNonQuery														\
	if (!pConnection->executeNonQuery(sqlSentence.str().c_str()))													\
	{																												\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "executeNonQuery : %s", pConnection->getErrorString().c_str());	\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_EXECUTE_NONQUERY;												\
		return false;																								\
	}

#define CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ExecuteReader														\
    pDataReader = pConnection->executeReader(sqlSentence.str().c_str());											\
    if (!pDataReader)																								\
	{																												\
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "executeReader : %s", pConnection->getErrorString().c_str());	\
        m_errCode = BCLib::Database::EDB_TASK_ERROR_EXECUTE_READER;													\
        return false;																								\
    }

#define CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ExecuteReader_Check(_NEED_HAVE_)				\
																								\
	if (_NEED_HAVE_)																			\
	{																							\
		if (pDataReader->getRecordCount() < 1)													\
		{																						\
			BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "pDataReader->getRecordCount() < 1");	\
			m_errCode = BCLib::Database::EDB_TASK_ERROR_NO_RECORD;								\
			delete pDataReader;																	\
			return false;																		\
		}																						\
	}																							\
	else																						\
	{																							\
		if (pDataReader->getRecordCount() != 0)													\
		{																						\
			m_errCode = BCLib::Database::EDB_TASK_ERROR_DUPLICATED_RECORD;						\
			delete pDataReader;																	\
			return false;																		\
		}																						\
	}

#define CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ExecuteReader_Next								\
	if (!pDataReader->next())																	\
	{																							\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "!pDataReader->next()");						\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_READ_NEXT;									\
		delete pDataReader;																		\
		return false;																			\
	}

//////////////////////////////////////////////////////////////////////////

#define CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadFloat(_PTBUF_, _NAME_)	\
	if (!pDataReader->getDouble(#_NAME_, dValue))							\
	{																		\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, #_NAME_);				\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_READ_DATA;				\
		delete pDataReader;													\
		return false;														\
	}																		\
	_PTBUF_.set_##_NAME_((float)dValue);

#define CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadInt32(_PTBUF_, _NAME_)	\
	if (!pDataReader->getInt32(#_NAME_, i32Value))							\
	{																		\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, #_NAME_);				\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_READ_DATA;				\
		delete pDataReader;													\
		return false;														\
	}																		\
	_PTBUF_.set_##_NAME_(i32Value);

#define CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadInt64(_PTBUF_, _NAME_)	\
	if (!pDataReader->getInt64(#_NAME_, i64Value))							\
	{																		\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, #_NAME_);				\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_READ_DATA;				\
		delete pDataReader;													\
		return false;														\
	}																		\
	_PTBUF_.set_##_NAME_(i64Value);

#define CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadUint32(_PTBUF_, _NAME_)	\
	if (!pDataReader->getUint32(#_NAME_, u32Value))							\
	{																		\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, #_NAME_);				\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_READ_DATA;				\
		delete pDataReader;													\
		return false;														\
	}																		\
	_PTBUF_.set_##_NAME_(u32Value);

#define CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadUint64(_PTBUF_, _NAME_)	\
	if (!pDataReader->getUint64(#_NAME_, u64Value))							\
	{																		\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, #_NAME_);				\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_READ_DATA;				\
		delete pDataReader;													\
		return false;														\
	}																		\
	_PTBUF_.set_##_NAME_(u64Value);

#define CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadString(_PTBUF_, _NAME_)	\
	if (!pDataReader->getString(#_NAME_, strValue))							\
	{																		\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, #_NAME_);				\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_READ_DATA;				\
		delete pDataReader;													\
		return false;														\
	}																		\
	_PTBUF_.set_##_NAME_(strValue);

//////////////////////////////////////////////////////////////////////////

}//AccountServer
}//SSLib

#endif//__SSLIB_ACCOUNTSET_ACCOUNTSERVER_DATABASETASK_DATABASETASK_H__
