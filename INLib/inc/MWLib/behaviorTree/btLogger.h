//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/btLogger.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_BTLOGGER_H__
#define __MWLIB_BEHAVIORTREE_BTLOGGER_H__

#include <MWLib/behaviorTree/btNode.h>
#include <BCLib/utility/thread/mutex.h>


namespace MWLib
{
namespace BehaviorTree
{
/** @file */ //This line allows Doxygen to see the enums that are defined outside the scope of a class

/*! Vector of strings. Used to store the parameters for each log. */
typedef std::vector<std::string> LogParams;

/**
 * NodeLogEntry struct.
 * It contains the log information sent to the BT Editor.
 */
struct SNodeLogEntry
{
    std::string m_timestamp;    /**< It is the timestamp when the log was recorded. */
    unsigned long m_milisec;    /**< Indicates in miliseconds since execution start when this log was recorded. */
    int m_nodeId;        /**< ID of the node that produces the log. */
    ENodeStatus m_status;        /**< Result of the last execution of the node that produces the log. */
    LogParams m_params;        /**< List of parameters sent to the BT editor in the log. */
};

/*! Queue of entries to log. Used to store log entries. */
typedef std::queue<SNodeLogEntry> Log;


class CBTLogger
{
public:

    //!Constructor initializes member variables.
    CBTLogger();

    //!Destructor frees member variables.
    ~CBTLogger();


    //!Constructor initializes member variables.
    bool hasLogs();

    /**
     * Gets the next log in the queue.
     * @return The next entry to send.
     */
    SNodeLogEntry nextLog();

    /**
     * Pops the next log in the queue
     */
    void advance();

    /**
     * Creates a new log and insertes it in the queue.
     * @param a_nodeId ID of the node that triggers the log entry creation.
     * @param a_nodeStatus Last execution result of the node that produces the log.
     * @param a_params List of params that can be sent to the BT editor.
     * @param a_paramsCount Number of params to include in the log.
     */
    void newNodeLog(int a_nodeId, ENodeStatus a_nodeStatus, std::string* a_params = 0, int a_paramsCount = 0);

private:

    //! Queue of logs entry to send to the BT Editor.
    Log m_log;        // Watch out: This is a shared resource by different threads

    //! Critical section for multi-thread access to the log queue.
    BCLib::Utility::CMutex m_mutex;
};
}//BehaviorTree
}//MWLib

#endif//__MWLIB_BEHAVIORTREE_BTLOGGER_H__
