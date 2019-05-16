//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SSLib/accountSet/accountServer/databaseMgr/databaseMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SSLIB_ACCOUNTSET_ACCOUNTSERVER_DATABASEMGR_DATABASEMGR_H__
#define __SSLIB_ACCOUNTSET_ACCOUNTSERVER_DATABASEMGR_DATABASEMGR_H__

#include <BCLib/utility/singleton.h>
#include <BCLib/database/connection.h>
#include <BCLib/database/dbInfo.h>
#include <BCLib/utility/thread/mutex.h>
#include <SSLib/message/message.h>

namespace SSLib
{
namespace AccountServer
{

class CDatabaseMgr
{
    BCLIB_SINGLETON_DECLARE(CDatabaseMgr);

public:
    CDatabaseMgr();
    virtual ~CDatabaseMgr();

public:
	bool			init(std::string strDbFile);
    BCLib::uint32	getDBIndex(BCLib::uint64 uHash){ return (BCLib::uint32)(uHash%(m_dbMaxNum * m_tbMaxNum)/m_tbMaxNum);}
    BCLib::uint32	getTBIndex(BCLib::uint64 uHash){ return (BCLib::uint32)(uHash%(m_dbMaxNum * m_tbMaxNum)%m_tbMaxNum);}

private:
	bool									m_bInit;
	BCLib::uint32							m_dbMaxNum;
	BCLib::uint32							m_tbMaxNum;
	std::vector<BCLib::Database::CDBInfo*>	m_vecDBInfo;
};

}//AccountServer
}//SSLib

#endif//__SSLIB_ACCOUNTSET_ACCOUNTSERVER_DATABASEMGR_DATABASEMGR_H__
