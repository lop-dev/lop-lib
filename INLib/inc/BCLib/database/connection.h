//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/database/connection.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_DATABASE_CONNECTION_H__
#define __BCLIB_DATABASE_CONNECTION_H__

#include <BCLib/utility/pointer.h>
#include <BCLib/utility/hashMap.h>
#include <BCLib/database/dbInfo.h>
#include <BCLib/database/dataReader.h>
#include <BCLib/database/dbParamDef.h>
#include <string>

namespace BCLib
{
namespace Database
{
enum DBCTYPE
{
    DBCTYPENONE,
    MSSQLCONNECTION,
    MYSQLCONNECTION,
    SQLITECONNECTION
};

class BCLIB_DATABASE_API IConnection
{
public:
    IConnection();
    virtual ~IConnection();

    virtual bool connectDB(const char* pszDBServer, const char* pszDBInstance, const char* pszDBName, const char* pszUserName, const char* pszPassword) = 0;
    virtual bool isConnected() const = 0;

    virtual bool executeNonQuery(const char* pszCommand) = 0;
    virtual CDataReader* executeReader(const char* pszCommand) = 0;
    virtual bool executeInsertData(const char* pszTableName, const char* pszColumnList, va_list vaList) = 0;
    virtual int32 executeUpdateData(const char* pszTableName, const char* pszCondition, const char* pszColumnList, va_list vaList) = 0;
    virtual int32 executeProcedure(const char* pszProcedure, int32 i32ParamNum, CDataReader** pOutDataReader, va_list vaList) = 0;

    std::string errorString() const
    {
        return m_errString;
    }
protected:
    mutable std::string m_errString;
};

//////////////////////////////////////////////////////////////////////////

class BCLIB_DATABASE_API CConnection
{
public:
    CConnection(DBCTYPE dbcType);
    virtual ~CConnection();

	void setDBIndex(BCLib::uint32 dbIndex);
	BCLib::uint32 getDBIndex();

	void setDBInfo(CDBInfo& dbInfo) { m_dbInfo = dbInfo; }
	CDBInfo getDBInfo() { return m_dbInfo; }

    bool connectDB(const char* pszDBServer, const char* pszDBInstance, const char* pszDBName, const char* pszUserName, const char* pszPassword);
    bool isConnected() const;

    bool executeNonQuery(const char* pszCommand);
    CDataReader* executeReader(const char* pszCommand);
    bool executeInsertData(const char* pszTableName, const char* pszColumnList, ...);
    int32 executeUpdateData(const char* pszTableName, const char* pszCondition, const char* pszColumnList, ...);
    int32 executeProcedure(const char* pszProcedure, int32 i32ParamNum, CDataReader** pOutDataReader, ...);

    virtual std::string getErrorString() const;
    DBCTYPE getConnectionType() const;

private:
    IConnection* m_pConnection;
    DBCTYPE m_eComType;

	BCLib::uint32 m_dbIndex;
	CDBInfo m_dbInfo;
};
typedef BCLib::Utility::CHashMap<std::string, CConnection *> CConnectionMap;

}//Database
}//BCLib

#endif//__BCLIB_DATABASE_CONNECTION_H__
