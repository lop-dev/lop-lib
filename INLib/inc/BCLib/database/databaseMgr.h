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
    /// @brief 初始化数据库
    /// @return bool
    /// @param pDatabaseTaskMgr
    /// @param strDbFile
    /// @param uGroupCount 如果 uGroupCount 传的是0，会尝试从数据库的配置文件中读取线程数
	bool			init(CDatabaseTaskMgr* pDatabaseTaskMgr, std::string strDbFile, BCLib::uint32 uGroupCount = 0);

	BCLib::uint32	getDBMaxNum() { return m_dbMaxNum; }
	BCLib::uint32	getTBMaxNum() { return m_tbMaxNum; }

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
