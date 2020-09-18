//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/logger/loggerManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_LOGGER_LOGGERMANAGER_H__
#define __MWLIB_BEHAVIORTREE_LOGGER_LOGGERMANAGER_H__

/** @file */

#include <BCLib/utility/thread/mutex.h>
#include <BCLib/utility/singleton.h>

namespace MWLib
{
namespace BehaviorTree
{
class CBTLogger;

/*! Vector of BTLogger pointers, identified by its behavior tree id. */
typedef std::map<unsigned int, CBTLogger*> BTLoggers;

/**
 * CLoggerManager class. This class gathers the loggers of all the behavior trees
 * created in the system.
 */
class CLoggerManager
{
    BCLIB_SINGLETON_DECLARE(CLoggerManager);

public:
    //! Frees member variables. Destructor is private.
    static void Destroy();

    /**
     * Returns the BTLogger from a behavior tree. If there is no BTLogger, it is created.
     * @param a_btId Behavior tree identifier which BTLogger we want to retrieve.
     * @return The behavior tree BTLogger.
     */
    CBTLogger* getBTLogger(unsigned int a_btId);

private:


    //! Pointer to the unique instance of this class.
    static CLoggerManager* m_instance;

    /**
     * Private constructor. To create the leaf factory, use GetInstance() instead.
     * @see GetInstance()
     */
    CLoggerManager();

    /**
     * Private destructor. To delete the leaf factory, use Destroy() instead.
     * @see Destroy()
     */
    ~CLoggerManager();

    //! Collection of BTLogger pointers.
    BTLoggers m_btLoggers;

    //! Critical section to allow multi-thread access to this class collection.
    BCLib::Utility::CMutex m_mutex;
};
}//BehaviorTree
}//MWLib
#endif//__MWLIB_BEHAVIORTREE_LOGGER_LOGGERMANAGER_H__
