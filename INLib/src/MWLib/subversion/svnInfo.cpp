//////////////////////////////////////////////////////////////////////
//  created:    2012/02/08
//  filename:   MWLib/subversion/svnInfo.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <MWLib/subversion/svnInfo.h>

CSvnInfo::CSvnInfo()
{
    m_dbName = "";
}

CSvnInfo::CSvnInfo(std::string& dbName)
{
    m_dbName = dbName;
    m_errStr = "";
}

CSvnInfo::~CSvnInfo()
{
    ;
}

void CSvnInfo::setDBname(std::string& dbName)
{
    m_dbName = dbName;
    m_errStr = "";
}

std::string& CSvnInfo::getErrorStr()
{
    return m_errStr;
}

BCLib::int32 CSvnInfo::getSvnLocalRevision()
{
    BCLib::Database::CConnection* pconnection;
    BCLib::int32 revision = 0;
    pconnection = new (std::nothrow)BCLib::Database::CConnection(BCLib::Database::SQLITECONNECTION);
    if (pconnection == NULL)
    {
        m_errStr = "creatconnection failed!";
        return 0;
    }
    if (!pconnection->connectDB(NULL, NULL, m_dbName.c_str(), NULL, NULL))
    {
        m_errStr = "connectDB failed!";
        return 0;
    }

    BCLib::Database::CDataReader* pDataReader = pconnection->executeReader("select * from NODES LIMIT 1;");
    if (pDataReader)
    {

        pDataReader->next();
        pDataReader->getInt32("revision", revision);
        delete pDataReader;
    }
    else
    {
        m_errStr = "read data failed";
    }
    delete pconnection;
    return revision;
}