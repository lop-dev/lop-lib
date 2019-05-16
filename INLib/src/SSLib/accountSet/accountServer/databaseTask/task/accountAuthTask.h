//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SSLib/accountSet/accountServer/databaseTask/task/accountAuthTask.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SSLIB_ACCOUNTSET_ACCOUNTSERVER_DATABASETASK_TASK_ACCOUNTAUTHTASK_H__
#define __SSLIB_ACCOUNTSET_ACCOUNTSERVER_DATABASETASK_TASK_ACCOUNTAUTHTASK_H__

#include "../databaseTask.h"
#include <SSLib/accountSet/accountServer/protobuf/accountAuth.pb.h>

namespace SSLib
{
namespace AccountServer
{

CDATABASETASK_SUBCLASS_DECLARE(InsertAccountAuth, PTBuf::CAccountAuth);
CDATABASETASK_SUBCLASS_DECLARE(SelectAccountAuth, PTBuf::CAccountAuth);
CDATABASETASK_SUBCLASS_DECLARE(UpdateAccountAuth, PTBuf::CAccountAuth);
CDATABASETASK_SUBCLASS_DECLARE(DeleteAccountAuth, PTBuf::CAccountAuth);

}//AccountServer
}//SSLib
#endif//__SSLIB_ACCOUNTSET_ACCOUNTSERVER_DATABASETASK_TASK_ACCOUNTAUTHTASK_H__
