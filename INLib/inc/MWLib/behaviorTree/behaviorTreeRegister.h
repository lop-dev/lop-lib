//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/behaviorTreeRegister.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_BEHAVIORTREEREGISTER_H__
#define __MWLIB_BEHAVIORTREE_BEHAVIORTREEREGISTER_H__

#include <BCLib/utility/baseDef.h>

namespace MWLib
{
namespace BehaviorTree
{
/** @file */

class CBehaviorTree;

/**
 * Indicates if the data referred in ConsistencyInfo structure refers to
 * a file in disk or a buffer.
 */
enum ConsistencyType
{
    CON_TYPE_UNDEF  = 0,
    CON_TYPE_FILE   = 1,
    CON_TYPE_STREAM = 2
};


/**
 * ConsistencyInfo struct.
 * It contains information to keep consistency in the system. Allows to
 * delete behavior trees and initialize them again, skipping the communication
 * with the bt editor.
 */
struct SConsistencyInfo
{
    ConsistencyType m_type; /**< It is the type of data about the behavior tree loaded. */
    std::string m_data; /**< It is the data about the loaded behavior tree. It'll be a stream or a filename */
    long long m_startTime; /**< It is the timestamp when the BT was loaded. */
};

/*! Play state of the DEBUG behavior trees in the execution.*/
enum EBT_DebugPlayState
{
    BTPST_ALL_UNDEF,
    /*!< BTs will execute depending on their state.  */
    BTPST_ALL_PLAYING,
    /*!< BTs will execute.  */
    BTPST_ALL_PAUSED,
    /*!< BTs will NOT execute. */
    BTPST_ALL_STOPPED,
    /*!< BTs will NOT execute.  */
    BTPST_ALL_STEPPED,
    /*!< BTs will execute 1 cycle.  */
};

/*! Collection of behavior trees created in the system, identified by an unsigned int. */
typedef std::map<unsigned int, CBehaviorTree*> TreeMap;

/*! Collection of ConsistencyInfo structures, identified by an unsigned int. */
typedef std::map<unsigned int, SConsistencyInfo> TreeConsistency;

/**
 * BTRegister class.
 * This class gathers pointers to the behavior trees created in the system.
 * It is used to maintain consistency and provide a list of BTs to the bt editor.
 */
class CBehaviorTreeRegister
{
public:

    //! Singleton access: Unique access point. Constructor is private.
    static CBehaviorTreeRegister* GetInstance();

    //! Frees member variables. Destructor is private.
    static void Destroy();

    /**
     * Inserts a tree in the tree list.
     * @param a_bt Behavior tree to insert.
     * @return New identifier of this tree.
     */
    unsigned int registerTree(CBehaviorTree* a_bt);

    /**
     * Inserts a tree in the tree list, with a BT ID given
     * @param a_id Identifier of the tree to insert.
     * @param a_bt Behavior tree to insert.
     */
    void registerTree(unsigned int a_id, CBehaviorTree* a_bt);

    /**
     * Deletes a behavior tree from the tree collection.
     * @param a_id Behavior tree ID to delete.
     */
    void unregisterTree(unsigned int a_id);

    /**
     * Returns a behavior tree from the tree list.
     * @param a_id Behavior tree id to get.
     * @return Behavior tree in the collection. Returns null if not found.
     */
    CBehaviorTree* getTree(unsigned int a_id);

    /**
     * Assigns a filename to this tree (where did I read it from)
     * @param a_id Behavior tree id to update.
     * @param a_filename Filename to assign.
     */
    void registerTreeFilename(unsigned int a_id, std::string a_filename);

    /**
     * Assigns a stream to this tree (where did I read it from)
     * @param a_id Behavior tree id to update.
     * @param a_stream Stream to assign.
     */
    void registerTreeStream(unsigned int a_id, std::string a_stream);

    /**
     * Gets the data stored of a tree.
     * @param a_id Behavior tree id which data we want to retrieve.
     * @return Data stored, empty string if nothing found.
     */
    std::string getData(unsigned int a_id);

    /**
     * Gets the consistency type stored for a tree.
     * @param a_id Behavior tree id which consistency type we want to retrieve.
     * @return ConsistencyTpe stored.
     */
    ConsistencyType getConsistencyType(unsigned int a_id);

    /**
     * Assigns a start time for this tree.
     * @param a_id Behavior tree id to update.
     * @param a_time Starting time to assign.
     */
    void registerStartTime(unsigned int a_id, long long a_time);

    /**
     * Gets the starting time of a behavior tree.
     * @param a_id Behavior tree id which time we want to retrieve.
     * @return Starting time, -1 if nothing found.
     */
    long long getStartTime(unsigned int a_id);

    /**
     * Returns a collection of pointers to registered behavior trees
     * @return vector with all the trees' pointers.
     */
    std::vector<CBehaviorTree*> getRegisteredTrees();

    /**
     * Sets the play state of all debug bots registered.
     * @param a_debugPlayState New state. @see BT_DebugPlayState
     */
    void setDebugPlayState(EBT_DebugPlayState a_debugPlayState)
    {
        m_debugPlayState = a_debugPlayState;
    }

    /**
     * Gets the play state of all debug bots registered.
     * @return The BT_DebugPlayState of the debug behavior trees registered.
     */
    EBT_DebugPlayState getDebugPlayState()
    {
        return m_debugPlayState;
    }


private:

    //! Pointer to the unique instance of this class.
    static CBehaviorTreeRegister* m_instance;

    /**
     * Private constructor. To create the leaf factory, use GetInstance() instead.
     * @see GetInstance()
     */
    CBehaviorTreeRegister();

    /**
     * Private destructor. To delete the leaf factory, use Destroy() instead.
     * @see Destroy()
     */
    ~CBehaviorTreeRegister();

    //! State of all debug bots registered.
    EBT_DebugPlayState m_debugPlayState;

    //! Counter of Behavior trees in the system.
    unsigned int m_btCounter;

    //! Behavior trees collection
    TreeMap m_trees;

    //! TreeConsistency struct collection.
    TreeConsistency m_consistency;
};
}//BehaviorTree
}//MWLib
#endif//__MWLIB_BEHAVIORTREE_BEHAVIORTREEREGISTER_H__
