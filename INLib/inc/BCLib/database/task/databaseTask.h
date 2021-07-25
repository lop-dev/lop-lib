//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/database/task/databaseTask.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_DATABASE_TASK_DATABASETASK_H__
#define __BCLIB_DATABASE_TASK_DATABASETASK_H__

#include <BCLib/database/connection.h>
#include <BCLib/framework/serviceID.h>
#include <BCLib/utility/string.h>
#include <BCLib/utility/hashMap.h>
#include <BCLib/utility/spointer.h>

namespace BCLib
{
namespace Database
{

/// @brief ���峣�õ� DBTask ����
/// @brief ������û��ʹ�õ���Ϊ�ϲ�ͳһ����
/// @brief ����ϲ㲻Ը���ã����ж�������ֵ
enum EDBTaskResult
{
	EDB_TASK_ERROR_OK,
	EDB_TASK_ERROR_UNKNOW,

	EDB_TASK_ERROR_NOCONNECTION,
	EDB_TASK_ERROR_UNCONNECTED,
	EDB_TASK_ERROR_EXECUTE_NONQUERY,
	EDB_TASK_ERROR_EXECUTE_READER,
	EDB_TASK_ERROR_READ_NEXT,
	EDB_TASK_ERROR_READ_DATA,

	EDB_TASK_ERROR_PASSWORD,
	EDB_TASK_ERROR_IPADDRESS,
	EDB_TASK_ERROR_PERMISSION,
	EDB_TASK_ERROR_ILLEGAL_STRING,

	EDB_TASK_ERROR_DUPLICATED_RECORD,
	EDB_TASK_ERROR_NO_RECORD,

	EDB_TASK_ERROR_INSERT_RECORD,
	EDB_TASK_ERROR_SELECT_RECORD,
	EDB_TASK_ERROR_UPDATE_RECORD,
	EDB_TASK_ERROR_DELETE_RECORD,

    EDB_TASK_ERROR_EXTEND,   // �����ϲ���չʹ��
    EDB_TASK_ERROR_MAX = 50, // �ϲ���չ��ò�Ҫ�������ֵ����ΪLOP���׹����������Ĵ���ֵ�������ֵ��ʼ
};

class BCLIB_DATABASE_API CDatabaseTask
{
public:
	CDatabaseTask();
	virtual ~CDatabaseTask();

public:
    virtual bool excute(CConnectionMap &) = 0;
    virtual bool reply() = 0;

    BCLib::int32 type() const { return m_taskType; }
    operator BCLib::int32 & () { return m_errCode; }

public:
	//virtual BCLib::uint32 hashDBIndex(BCLib::uint64 uHash);
	//virtual BCLib::uint32 hashDBIndex(const std::string& strHashKey);
	//virtual BCLib::uint32 hashDBIndex(BCLib::Utility::CStringA& strHashKey);

	//virtual BCLib::uint32 hashTBIndex(BCLib::uint64 uHash);
	//virtual BCLib::uint32 hashTBIndex(const std::string& strHashKey);
	//virtual BCLib::uint32 hashTBIndex(BCLib::Utility::CStringA& strHashKey);

	// strDBIndex �Ǵ� 001 ��ʼ�ģ���Ӧ�ǳ���������ļ����Ի�·
	// strTBIndex �Ǵ� 001 ��ʼ�ģ���Ӧ�ǳ���������ļ����Ի�·
	virtual bool hashIndex(BCLib::uint64 uHash, BCLib::Utility::CStringA& strDBIndex, BCLib::Utility::CStringA& strTBIndex);
	virtual bool hashIndex(const std::string& strHashKey, BCLib::Utility::CStringA& strDBIndex, BCLib::Utility::CStringA& strTBIndex);
	virtual bool hashIndex(BCLib::Utility::CStringA& strHashKey, BCLib::Utility::CStringA& strDBIndex, BCLib::Utility::CStringA& strTBIndex);

	// dbIndex    �Ǵ� 000 ��ʼ�ģ���Ӧ����Ա�Դ����д���Ի�·
	// strDBIndex �Ǵ� 001 ��ʼ�ģ���Ӧ�ǳ���������ļ����Ի�·
	virtual BCLib::Database::CConnection* getConnection(BCLib::Database::CConnectionMap & mapConnections, BCLib::uint32 dbIndex, std::string& strDBName);
	virtual BCLib::Database::CConnection* getConnection(BCLib::Database::CConnectionMap & mapConnections, std::string strDBIndex, std::string& strDBName);

	// dbIndex    �Ǵ� 000 ��ʼ�ģ���Ӧ����Ա�Դ����д���Ի�·
	// strDBIndex �Ǵ� 001 ��ʼ�ģ���Ӧ�ǳ���������ļ����Ի�·
	std::string		getDBName(BCLib::uint32 dbIndex);
	std::string		getDBName(std::string strDBIndex); 

protected:
    BCLib::int32    m_taskType;
    BCLib::int32    m_errCode;
};

//////////////////////////////////////////////////////////////////////////

#define BCLIB_CDATABASETASK_SUBCLASS_DEFINE_REPLY_Variable					\
	std::ostringstream              sqlSentence;(void)sqlSentence;			\
	BCLib::Utility::CStringA		strDBIndex = "";(void)strDBIndex;		\
	BCLib::Utility::CStringA		strTBIndex = "";(void)strTBIndex;		\
	std::string                     strDBName = "";(void)strDBName;			\
	std::string                     strTBName = "";(void)strTBName;			\
	BCLib::Database::CConnection *	pConnection = NULL;(void)pConnection;	\
	BCLib::Database::CDataReader *	pDataReader = NULL;(void)pDataReader;	\
	double							dValue = 0;(void)dValue;				\
	BCLib::int32					i32Value = 0;(void)i32Value;			\
	BCLib::int64					i64Value = 0;(void)i64Value;			\
	BCLib::uint32					u32Value = 0;(void)u32Value;			\
	BCLib::uint64					u64Value = 0;(void)u64Value;			\
	std::string						strValue = "";(void)strValue;			\
	BCLib::Utility::SPointerGuard<BCLib::Database::CDataReader> readerGuard(pDataReader);

#define BCLIB_CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_GetConnection													\
	pConnection = getConnection(mapConnections, strDBIndex, strDBName);												\
	if (pConnection == NULL)																						\
	{																												\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "pConnection == NULL");											\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_NOCONNECTION;													\
		return false;																								\
	}																												\
																													\
	if (!pConnection->isConnected())																				\
	{																												\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "!pConnection->isConnected()");									\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_UNCONNECTED;													\
		return false;																								\
	}

#define BCLIB_CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_UseDB															\
	pConnection = getConnection(mapConnections, strDBIndex, strDBName);												\
	if (pConnection == NULL)																						\
	{																												\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "pConnection == NULL");											\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_NOCONNECTION;													\
		return false;																								\
	}																												\
																													\
	if (!pConnection->isConnected())																				\
	{																												\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "!pConnection->isConnected()");									\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_UNCONNECTED;													\
		return false;																								\
	}																												\
																													\
	sqlSentence.str("");																							\
	sqlSentence << "USE " << strDBName.c_str();																		\
	if (!pConnection->executeNonQuery(sqlSentence.str().c_str()))													\
	{																												\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "executeNonQuery : %s", pConnection->getErrorString().c_str());	\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "sqlSentence : [%s:%s][%s]", strDBName.c_str(), strTBName.c_str(), sqlSentence.str().c_str());	\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_EXECUTE_NONQUERY;												\
		return false;																								\
	}

#define BCLIB_CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_UseDB_For_StaticTask(pTask)									\
	if (pTask == NULL)																								\
	{																												\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "pTask == NULL");												\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_NOCONNECTION;													\
		return false;																								\
	}																												\
	pConnection = pTask->getConnection(mapConnections, strDBIndex, strDBName);										\
	if (pConnection == NULL)																						\
	{																												\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "pConnection == NULL");											\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_NOCONNECTION;													\
		return false;																								\
	}																												\
																													\
	if (!pConnection->isConnected())																				\
	{																												\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "!pConnection->isConnected()");									\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_UNCONNECTED;													\
		return false;																								\
	}																												\
																													\
	sqlSentence.str("");																							\
	sqlSentence << "USE " << strDBName.c_str();																		\
	if (!pConnection->executeNonQuery(sqlSentence.str().c_str()))													\
	{																												\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "executeNonQuery : %s", pConnection->getErrorString().c_str());	\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "sqlSentence : [%s:%s][%s]", strDBName.c_str(), strTBName.c_str(), sqlSentence.str().c_str());	\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_EXECUTE_NONQUERY;												\
		return false;																								\
	}
	
#define BCLIB_CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ExecuteNonQuery												\
	BCLIB_LOG_DEBUG(BCLib::ELOGMODULE_DEFAULT, "sqlSentence : [%s:%s][%s]", strDBName.c_str(), strTBName.c_str(), sqlSentence.str().c_str());	\
	if (!pConnection->executeNonQuery(sqlSentence.str().c_str()))													\
	{																												\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "executeNonQuery : %s", pConnection->getErrorString().c_str());	\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "sqlSentence : [%s:%s][%s]", strDBName.c_str(), strTBName.c_str(), sqlSentence.str().c_str());	\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_EXECUTE_NONQUERY;												\
		return false;																								\
	}

#define BCLIB_CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ExecuteReader													\
    BCLIB_SAFE_DELETE(pDataReader);																					\
    pDataReader = pConnection->executeReader(sqlSentence.str().c_str());											\
    if (!pDataReader)																								\
	{																												\
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "executeReader : %s", pConnection->getErrorString().c_str());	\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "sqlSentence : [%s:%s][%s]", strDBName.c_str(), strTBName.c_str(), sqlSentence.str().c_str());	\
        m_errCode = BCLib::Database::EDB_TASK_ERROR_EXECUTE_READER;													\
        return false;																								\
    }

#define BCLIB_CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ExecuteReader_Check(_NEED_HAVE_)								\
																													\
	if (_NEED_HAVE_)																								\
	{																												\
		if (pDataReader->getRecordCount() < 1)																		\
		{																											\
			BCLIB_LOG_WARNING(BCLib::ELOGMODULE_DEFAULT, "pDataReader->getRecordCount() < 1");						\
			BCLIB_LOG_WARNING(BCLib::ELOGMODULE_DEFAULT, "sqlSentence : [%s:%s][%s]", strDBName.c_str(), strTBName.c_str(), sqlSentence.str().c_str());	\
			m_errCode = BCLib::Database::EDB_TASK_ERROR_NO_RECORD;													\
			BCLIB_SAFE_DELETE(pDataReader);																			\
			return false;																							\
		}																											\
	}																												\
	else																											\
	{																												\
		if (pDataReader->getRecordCount() != 0)																		\
		{																											\
			m_errCode = BCLib::Database::EDB_TASK_ERROR_DUPLICATED_RECORD;											\
			BCLIB_SAFE_DELETE(pDataReader);																			\
			return false;																							\
		}																											\
	}

#define BCLIB_CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ExecuteReader_Next											\
	if (!pDataReader->next())																						\
	{																												\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "!pDataReader->next()");											\
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "sqlSentence : [%s:%s][%s]", strDBName.c_str(), strTBName.c_str(), sqlSentence.str().c_str());	\
		m_errCode = BCLib::Database::EDB_TASK_ERROR_READ_NEXT;														\
		BCLIB_SAFE_DELETE(pDataReader);																				\
		return false;																								\
	}

//////////////////////////////////////////////////////////////////////////

#define BCLIB_CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadFloat(_PTBUF_, _NAME_)	\
	if (!pDataReader->getDouble(#_NAME_, dValue))							        \
	{																		        \
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, #_NAME_);				        \
		m_errCode = BCLib::Database::EDB_TASK_ERROR_READ_DATA;				        \
		BCLIB_SAFE_DELETE(pDataReader);										        \
		return false;														        \
	}																		        \
	_PTBUF_.set_##_NAME_((float)dValue);

#define BCLIB_CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadInt32(_PTBUF_, _NAME_)	\
	if (!pDataReader->getInt32(#_NAME_, i32Value))							        \
	{																		        \
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, #_NAME_);				        \
		m_errCode = BCLib::Database::EDB_TASK_ERROR_READ_DATA;				        \
		BCLIB_SAFE_DELETE(pDataReader);										        \
		return false;														        \
	}																		        \
	_PTBUF_.set_##_NAME_(i32Value);

#define BCLIB_CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadInt64(_PTBUF_, _NAME_)	\
	if (!pDataReader->getInt64(#_NAME_, i64Value))							        \
	{																		        \
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, #_NAME_);				        \
		m_errCode = BCLib::Database::EDB_TASK_ERROR_READ_DATA;				        \
		BCLIB_SAFE_DELETE(pDataReader);										        \
		return false;														        \
	}																		        \
	_PTBUF_.set_##_NAME_(i64Value);

#define BCLIB_CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadUint32(_PTBUF_, _NAME_)	\
	if (!pDataReader->getUint32(#_NAME_, u32Value))							        \
	{																		        \
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, #_NAME_);				        \
		m_errCode = BCLib::Database::EDB_TASK_ERROR_READ_DATA;				        \
		BCLIB_SAFE_DELETE(pDataReader);										        \
		return false;														        \
	}																		        \
	_PTBUF_.set_##_NAME_(u32Value);

#define BCLIB_CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadUint64(_PTBUF_, _NAME_)	\
	if (!pDataReader->getUint64(#_NAME_, u64Value))							        \
	{																		        \
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, #_NAME_);				        \
		m_errCode = BCLib::Database::EDB_TASK_ERROR_READ_DATA;				        \
		BCLIB_SAFE_DELETE(pDataReader);										        \
		return false;														        \
	}																		        \
	_PTBUF_.set_##_NAME_(u64Value);

#define BCLIB_CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadString(_PTBUF_, _NAME_)	\
	if (!pDataReader->getString(#_NAME_, strValue))							        \
	{																		        \
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, #_NAME_);				        \
		m_errCode = BCLib::Database::EDB_TASK_ERROR_READ_DATA;				        \
		BCLIB_SAFE_DELETE(pDataReader);										        \
		return false;														        \
	}																		        \
	_PTBUF_.set_##_NAME_(strValue);

#define BCLIB_CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadBase64(_PTBUF_, _NAME_)	\
	if (!pDataReader->getString(#_NAME_, strValue))							        \
	{																		        \
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, #_NAME_);				        \
		m_errCode = BCLib::Database::EDB_TASK_ERROR_READ_DATA;				        \
		BCLIB_SAFE_DELETE(pDataReader);										        \
		return false;														        \
	}																		        \
	_PTBUF_.set_##_NAME_(BCLib::Security::CBase64::Decode(strValue));

#define BCLIB_CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadPTBuf(_PTBUF_, _NAME_)	\
	if (!pDataReader->getString(#_NAME_, strValue))							        \
	{																		        \
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, #_NAME_);				        \
		m_errCode = BCLib::Database::EDB_TASK_ERROR_READ_DATA;				        \
		BCLIB_SAFE_DELETE(pDataReader);										        \
		return false;														        \
	}																		        \
	_PTBUF_.ParsePartialFromString(BCLib::Security::CBase64::Decode(strValue));

// ���������һ�����ɵĺ���
#define BCLIB_CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadRepeated(_PTBUF_, _NAME_, _CONVERTFUNC_)	\
	if (!pDataReader->getString(#_NAME_, strValue))							                        \
	{																		                        \
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, #_NAME_);				                        \
		m_errCode = BCLib::Database::EDB_TASK_ERROR_READ_DATA;				                        \
		BCLIB_SAFE_DELETE(pDataReader);										                        \
		return false;														                        \
	}																		                        \
	_CONVERTFUNC_(_PTBUF_, strValue);

//////////////////////////////////////////////////////////////////////////

}//Database
}//BCLib

#endif//__BCLIB_DATABASE_TASK_DATABASETASK_H__
