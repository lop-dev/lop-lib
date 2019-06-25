//////////////////////////////////////////////////////////////////////
//  created:    2012/02/08
//  filename:   MWLib/subversion/svnInfo.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_SUBVERSION_SVNINFO_H__
#define __MWLIB_SUBVERSION_SVNINFO_H__

#include <BCLib/database/connection.h>
#include <BCLib/database/dataReader.h>

class CSvnInfo
{
public:
    CSvnInfo();
    CSvnInfo(std::string& dbName);
    virtual ~CSvnInfo();

    void setDBname(std::string& dbName);
    BCLib::int32 getSvnLocalRevision();
    std::string& getErrorStr();

private:
    std::string m_dbName;
    std::string m_errStr;
};
#endif//__MWLIB_SUBVERSION_SVNINFO_H__
