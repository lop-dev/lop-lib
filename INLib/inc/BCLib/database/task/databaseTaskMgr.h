//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/database/task/databaseTaskMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_DATABASE_TASK_DATABASETASKMGR_H__
#define __BCLIB_DATABASE_TASK_DATABASETASKMGR_H__

#include <BCLib/utility/objectPool.h>
#include <BCLib/utility/hashMap.h>
#include <BCLib/utility/timer.h>
#include <BCLib/utility/thread/thread.h>
#include <BCLib/database/dbInfo.h>
#include <BCLib/database/task/databaseTask.h>

namespace BCLib
{
namespace Database
{

class CDatabaseTaskGroup;

class BCLIB_DATABASE_API CDatabaseTaskMgr
{
public:
    typedef BCLib::Utility::CHashMap<std::string, BCLib::uint32> KeyHashMap;

public:
    CDatabaseTaskMgr();
    virtual ~CDatabaseTaskMgr();

    bool            init(std::vector<BCLib::Database::CDBInfo*> & vecDBInfo, BCLib::uint32 uGroupCount = 8);
    void            release();

    bool            addTask(BCLib::uint32 u32HashID, CDatabaseTask * pTask);
    bool            addTask(CDatabaseTask * pTask);

    BCLib::int32    processTaskResults();

    BCLib::uint32   getGroupCount() {return m_vecGroup.size();}

    BCLib::uint32	hash(BCLib::uint32 u32Hash) { return u32Hash % m_vecGroup.size(); }
    KeyHashMap&     getKeyHmap() {return m_keyHashMap;}

	BCLib::int32    getReqQueueSize();
	BCLib::int32    getResQueueSize();

protected:
    virtual void    _processTask(CDatabaseTask * pTaskReply) = 0;

private:
	std::vector<CDatabaseTaskGroup*> m_vecGroup;
    KeyHashMap          m_keyHashMap;
};

}//Database
}//BCLib

#endif//__BCLIB_DATABASE_TASK_DATABASETASKMGR_H__
