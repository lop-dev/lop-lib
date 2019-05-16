//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/database/smpDatabase.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <BCLib/database/connection.h>


void BlobSample();
void ProcedureSample();

int _tmain(int argc, _TCHAR* argv[])
{
    //BCLib::Database::CConnection::Sample();
    BCLib::Database::CConnection tmpConnection(BCLib::Database::MYSQLCONNECTION);
    tmpConnection.connectDB("tcp://10.253.17.28:3306", "", "bugzilla", "username", "password");

    //BCLib::Database::CConnection tmpConnection(BCLib::Database::MSSQLCONNECTION);
    //tmpConnection.connectDB("tcp://10.253.17.28:1433","","bugzilla","username", "password");

    tmpConnection.executeNonQuery("DROP TABLE IF EXISTS test");
    tmpConnection.executeNonQuery("CREATE TABLE test(id INT, label CHAR(1))");
    tmpConnection.executeNonQuery("INSERT INTO test(id, label) VALUES (1, 'a')");

    std::string strCommand = "SELECT id, label FROM test ORDER BY id ASC";
    BCLib::Database::CDataReader* pDataReader = tmpConnection.executeReader(strCommand.c_str());
    if(pDataReader)
    {
        while(pDataReader->next())
        {
            // You can use either numeric offsets...
            int iValue;
            if(!pDataReader->getInt32(1, iValue))
            {
                // getInt(1) returns the first column
                // ... or column names for accessing results.
                //error
            }
            // The latter is recommended.
            std::string strValue;
            if(!pDataReader->getString("label", strValue))
            {
                //error
            }
        }
        pDataReader->release();
    }
    return 0;
}

void BlobSample()
{
    BCLib::Database::CConnection tmpConnection(BCLib::Database::MYSQLCONNECTION);

    if(!tmpConnection.connectDB("tcp://192.168.4.141:3306", "", "wmge", "root", "123456"))
    {
        //error
        return;
    }

    if(!tmpConnection.executeNonQuery("CREATE TABLE blobtest(`id` INT UNSIGNED NOT NULL, `data` blob);"))
    {
        //error
        return;
    }

    BCLib::Database::DB_PARAM_TYPE idParam;
    BCLib::Database::DB_PARAM_TYPE dataParam;
    char aTempBuf[1024];

    memset(aTempBuf, '3', sizeof(aTempBuf));
    aTempBuf[sizeof(aTempBuf) - 1] = 0;

    idParam.type = BCLib::Database::DB_PARAM_TYPE_INT64;
    idParam.value.i64Value = 4;

    dataParam.type = BCLib::Database::DB_PARAM_TYPE_BLOB;
    dataParam.value.pBlobData = aTempBuf;
    dataParam.value.nBlobLen = 256;

    if(!tmpConnection.executeInsertData("blobtest", "id, data", &idParam, &dataParam))
    {
        //error
        return;
    }

    memset(aTempBuf, '4', sizeof(aTempBuf));
    dataParam.value.nBlobLen = 128;

    if(!tmpConnection.executeUpdateData("blobtest", "id = 4", "id, data", &idParam, &dataParam))
    {
        //error
        return;
    }
}

void ProcedureSample()
{
    BCLib::Database::CConnection tmpConnection(BCLib::Database::MYSQLCONNECTION);

    //if (!tmpConnection.connectDB("tcp://192.168.4.141:3306","","wmge","root", "123456")){
    //    //error
    //    return;
    //}

    if(!tmpConnection.connectDB("tcp://127.0.0.1:3306", "", "wmge", "root", "123456"))
    {
        //error
        return;
    }

    if(!tmpConnection.executeNonQuery("DROP PROCEDURE IF EXISTS testprocedure"))
    {
        //error
        return;
    }

    if(!tmpConnection.executeNonQuery("CREATE PROCEDURE testprocedure(in paramin int, inout result int) \t\
        begin\t\
            set result = paramin + result;\t\
        end"))
    {
        //error
        return;
    }

    BCLib::Database::DB_PROCEDURE_PARAM_TYPE param1, param2;
    BCLib::int32 i32In = 100;
    BCLib::int32 i32Inout = 10;

    param1.value(i32In);
    param2.value(i32Inout, BCLib::Database::DB_PROCEDURE_PARAM_DIR_BOTH);

    BCLib::int32 i32Result = tmpConnection.executeProcedure("testprocedure", 2, NULL, &param1, &param2);
    if(i32Result < 0)
    {
        //error
    }
    //i32Inout = i32In + i32Inout;
}
