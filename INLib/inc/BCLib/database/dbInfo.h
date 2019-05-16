//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/database/dbInfo.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_DATABASE_DBINFO_H__
#define __BCLIB_DATABASE_DBINFO_H__

#include <BCLib/database/baseDef.h>
#include <BCLib/utility/pointer.h>
#include <string>

namespace BCLib
{
namespace Database
{
class BCLIB_DATABASE_API CDBInfo
{
public:
    CDBInfo();
    virtual ~CDBInfo();

    void reset();

    void setDBType(std::string strType)
    {
        m_strDBType = strType;
    }
    std::string getDBType()
    {
        return m_strDBType;
    }

    void setDBServer(std::string strServer)
    {
        m_strDBServer = strServer;
    }
    std::string getDBServer()
    {
        return m_strDBServer;
    }

    void setDBInstance(std::string strInstance)
    {
        m_strDBInstance = strInstance;
    }
    std::string getDBInstance()
    {
        return m_strDBInstance;
    }

    void setDBName(std::string strName)
    {
        m_strDBName = strName;
    }
    std::string getDBName()
    {
        return m_strDBName;
    }

    void setUserName(std::string strUser)
    {
        m_strUserName = strUser;
    }
    std::string getUserName()
    {
        return m_strUserName;
    }

    void setPassword(std::string strPassword)
    {
        m_strPassword = strPassword;
    }
    std::string getPassword()
    {
        return m_strPassword;
    }

private:
    std::string m_strDBType;
    std::string m_strDBServer;
    std::string m_strDBInstance;
    std::string m_strDBName;
    std::string m_strUserName;
    std::string m_strPassword;
};

}//Database
}//BCLib

#endif//__BCLIB_DATABASE_DBINFO_H__
