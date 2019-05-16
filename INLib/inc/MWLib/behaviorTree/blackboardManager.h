//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/blackboardManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_BLACKBOARDMANAGER_H__
#define __MWLIB_BEHAVIORTREE_BLACKBOARDMANAGER_H__
/** @file */

#include <BCLib/utility/baseDef.h>

namespace MWLib
{
namespace BehaviorTree
{
class CBlackboard;
/*! Collection of blackboard pointer, identified by an unsigned int. */
typedef std::map<unsigned int, CBlackboard*> BlackboardCollection;

/**
 * This class manages a collection of blackboards that can be shared by more than one unit.
 * Implements a Singleton pattern.
 */
class CBlackboardManager
{
private:

    //! Pointer to the unique instance of this class.
    static CBlackboardManager* m_instance;

    /**
     * Private constructor. To create the leaf factory, use GetInstance() instead.
     * @see GetInstance()
     */
    CBlackboardManager();

    /**
     * Private destructor. To delete the leaf factory, use Destroy() instead.
     * @see Destroy()
     */
    ~CBlackboardManager();

    //! Collection of blackboards.
    BlackboardCollection m_blackboards;

public:
    //! Singleton access: Unique access point. Constructor is private.
    static CBlackboardManager* GetInstance() ;

    //! Frees member variables. Destructor is private.
    static void Destroy();

    /**
     * Gets the blackboard with the given name. If it does not exists, creates it.
     * @param a_bboardId ID for the blackboard to get (or create).
     * @return The blackboard found in the container, or created if it did not exist.
     */
    CBlackboard* getBlackboard(unsigned int a_bboardId);
};
}//BehaviorTree
}//MWLib

#endif//__MWLIB_BEHAVIORTREE_BLACKBOARDMANAGER_H__
