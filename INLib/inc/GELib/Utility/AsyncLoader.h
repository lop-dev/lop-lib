//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/AsyncLoader.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_ASYNCLOADER_H__
#define __GELIB_UTILITY_ASYNCLOADER_H__

#include <map>
#include "SystemUtility.h"
#include "LoadPriority.h"

namespace GELib
{

class CAsyncLoader;

class CLoadTask
{
public:
    CLoadTask();
    virtual ~CLoadTask();

    CAsyncLoader* GetLoader() { return m_pLoader; }

    bool Load();        // run in IO thread
    bool Process();     // run in main thread, if Load() is true
    void Cleanup();     // run in main thread, if Load() is false && not canceled
	void Wait();		// wait for Load() or Process() or Cleanup()

    std::string m_Name;
    int m_iSize;                           // value must be correct after Load()
    ELoadPriority m_ePriority;             // higher priority load first, FIFO if priority is equal
    CAsyncLoader *m_pLoader;               // assigned by loader when added into loader

protected:

    virtual bool DoLoad() { return true; }
    virtual bool DoProcess() { return true; }
    virtual void DoCleanup() {}

	CCriticalSectionObject m_CS;
};


class CTaskQueue;
class CLoadThread;

class CAsyncLoader
{
public:
    CAsyncLoader();
    virtual ~CAsyncLoader();

    void Initialize(bool enableThread=true);
    void Uninitialize();

    void SetLoadThreadProcessor(int processNo);

    int  GetLoadedSize() const;

    bool AddTask(CLoadTask *task);
    void CancelTask(CLoadTask *task);
    bool IsTaskExist(CLoadTask *task);
    bool ChangeTaskPriority(CLoadTask *task, ELoadPriority priority);

    void Process(int timeInMs);

    void WaitAllTasks();
	bool IsAllTaskFinished();

    friend class CLoadThread;

private:

    enum ETaskState
    {
        TASK_STATE_INVALID = 0,
        TASK_STATE_CANCELED,
        TASK_STATE_WAIT_LOAD,
        TASK_STATE_LOADING,
        TASK_STATE_WAIT_PROCESS,
        TASK_STATE_PROCESSING,
        TASK_STATE_WAIT_CLEANUP,
        TASK_STATE_CLEANUP,
    };

    void RunLoadThread();

    void SetTaskState(CLoadTask *task, ETaskState state);
    ETaskState GetTaskState(CLoadTask *task);
    void DeleteTask(CLoadTask *task);

    CCriticalSectionObject m_CS;
    CLoadThread *m_pLoadThread;
    CTaskQueue *m_pLoadTasks[3];
    CTaskQueue *m_pProcessTasks;
    CTaskQueue *m_pCleanupTasks;

    typedef std::map<CLoadTask*, ETaskState> TaskStateMap;
    TaskStateMap m_TaskStates;
    int m_iLoadedSize;
    bool m_bEnableThread;
};

void SetAsyncLoader(CAsyncLoader *loader);
CAsyncLoader * GetAsyncLoader();


}//GELib

#endif//__GELIB_UTILITY_ASYNCLOADER_H__
