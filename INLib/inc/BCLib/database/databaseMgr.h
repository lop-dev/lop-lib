//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/database/databaseMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_DATABASE_DATABASEMGR_H__
#define __BCLIB_DATABASE_DATABASEMGR_H__

#include <BCLib/utility/singleton.h>
#include <BCLib/database/connection.h>
#include <BCLib/database/dbInfo.h>
#include <BCLib/database/task/databaseTaskMgr.h>

namespace BCLib
{
namespace Database
{

class CDatabaseMgr
{
    BCLIB_SINGLETON_DECLARE(CDatabaseMgr);

protected:
    CDatabaseMgr();
    virtual ~CDatabaseMgr();

public:
	bool			init(CDatabaseTaskMgr* pDatabaseTaskMgr, std::string strDbFile);

	BCLib::uint32	getDBIndex(BCLib::uint64 uHash);
	BCLib::uint32	getTBIndex(BCLib::uint64 uHash);

	std::string		getDBName(BCLib::uint32 dbIndex);  // dbIndex    是从 000 开始的，适应程序员对代码编写的脑回路

protected:
	bool _getInstancesDBName(std::vector<std::string>& vec_reply);

private:
	bool									m_bInit;
	BCLib::uint32							m_dbMaxNum;
	BCLib::uint32							m_tbMaxNum;
	std::vector<BCLib::Database::CDBInfo*>	m_vecDBInfo;
};

}//Database
}//BCLib

#endif//__BCLIB_DATABASE_DATABASEMGR_H__
