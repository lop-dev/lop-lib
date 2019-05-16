//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/behaviorTree.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_BEHAVIORTREE_H__
#define __MWLIB_BEHAVIORTREE_BEHAVIORTREE_H__

#include <BCLib/utility/baseDec.h>
#include <BCLib/utility/io/xmlFile.h>
#include <BCLib/utility/blackBoard.h>
#include <MWLib/behaviorTree/btNode.h>

namespace MWLib
{
namespace BehaviorTree
{
class CBTLinkNode;
class CBTNode;
class Handler;
class BTTask;
class CBehaviorTreeLibrary;
class CBehaviorTree;

struct CBTEventNode
{
    CBTNode* m_parent;
    CBehaviorTree* m_child;
    std::string m_eventName;
};

/** @file */ //This line allows Doxygen to see the enums that are defined outside the scope of a class

/*! Runtime BT types. */
enum BT_Info
{
    /*!< The type has not been set or is undefined  */
    BT_UNDEF,
    /*!< Configured for reading the BT from a file  */
    BT_RUNTIME,
    /*!< Configured for communication with editor  */
    BT_DEBUGBOT
};

/*! States of the behavior tree. */
enum BT_State
{
    /*!< BT is ready to execute.  */
    BTST_IDLE,
    /*!< BT has been started executing. */
    BTST_EXECUTING,
    /*!< BT is being changed.  */
    BTST_CHANGING,
};

/*! Play state of the behavior tree. The beahvior tree is running, but can be paused or stopped*/
enum BT_PlayState
{
    /*!< BT is ready to execute.  */
    BTPST_PLAYING,
    /*!< BT has been started executing. */
    BTPST_PAUSED,
    /*!< BT is stopped.  */
    BTPST_STOPPED,
    /*!< BT is being stepped (Executes a cycle and gets paused).  */
    BTPST_STEPPED,
};


/*! Stop protocols for the behavior tree. Specifies whether the bt must stop its cycle execution
    in the leaf nodes or not.*/
enum BT_StopProtocol
{
    /*!< BT will not stop until reaching back the root.  */
    BTSTOPPROT_NON,
    /*!< BT will stop in all the leaf nodes. */
    BTSTOPPROT_LEAFNODES,
    /*!< BT will stop in all the actions.  */
    BTSTOPPROT_ACTIONS,
    /*!< BT will stop in all the conditions.  */
    BTSTOPPROT_CONDITIONS,
};

enum ENodeStatus;
enum EBreakpointMode;

/**
 * The core BehaviorTree class.
 * It is responsible for holding the collection of nodes that comprises the BT (Behavior Tree)
 * and the functionality for executing an individual BT.
 */
class CBehaviorTree
{
public:
    typedef std::map<int, CBTNode*> EventNodes;
    typedef std::vector<CBTNode*> Nodes;
    typedef std::map<std::string, CBTEventNode*> EventList;

    typedef std::queue<std::string> EventQueue;
    typedef std::vector<EventQueue*> EventQueueBuffer;

private:

    //! The root BTLinkNode from which the rest of the tree branches.
    CBTLinkNode* m_root;
    //! Pointer to value that is used to identify the owner of the BT can be an int, string, unsigned int, etc.
    Handler* m_hndOwner;
    //! Enum that describe the runtime type of the BT.
    BT_Info m_info;
    //! BT_State of the BT.
    BT_State m_btState;
    //! Blackboard for this BT.
    BCLib::Utility::CBlackBoard* m_blackboard;
    //! Indicates if the PlayState of the behavior tree (Play, Pause or Stop)
    BT_PlayState m_playState;
    //! Indicates the stop protocol of the behavior tree.
    BT_StopProtocol m_stopProtocol;
    //! The current node being executed.
    CBTNode* m_currentNode;

    //! A unique identifier for the BT.
    unsigned int m_id;
    //! Indicates the exact time when this BT started its execution
    long long m_startTime;
    //! Flag that indicates whether the BT has started executing
    bool m_started;
    //! Flag that indicates whether the BT has had the initTree() method called.
    bool m_initialized;

    //!Initializes the nodes of the tree. Calls init() method on those nodes that implement that function.
    void initTree();

    /**
     * Checks the BT Play state of the tree, and the BT state of all the registered ones.
     * @return true if the BT can execute
     */
    bool checkBTPlayState();

    /**
     * This function cleans the behavior tree blackboard.
     */
    void cleanBlackboard();

public:

    //!Constructor initializes member variables.
    CBehaviorTree(CBehaviorTreeLibrary* pBTLib);

    //!Destructor frees member variables.
    ~CBehaviorTree();

    /**
     * Executes the behavior tree.
     * @return The result of the execution.
     */
    ENodeStatus execute();

    /**
     * Loads a behavior tree.
     * @param a_configId It is the ID of the type of behavior to load. The correspondence between IDs and
     * behavior tree types must be written in the file ai_config.xml
     * @param a_hnd Handler pointer to the owner of this behavior tree.
     * @return true if load was OK.
     */
    //bool load(int a_configId, Handler* a_hnd);

    /**
     * Sets the debug breakpoint mode in the node given by the ID received
     * @param a_id It is the node which breakpoint mode must be modified.
     * @param a_on It is the value of the breakpoint mode to set.
     */
    void setBreakpoint(int a_id, EBreakpointMode a_on);

    /**
     * Enables/Disables the branch under the received node.
     * @param a_nodeId It is the node, top of the branch to enable/disable.
     * @param a_enabled True is branch must be enabled, false otherwise.
     */
    void enableBranch(int a_nodeId, bool a_enabled);

    /**
     * This function cleans the internal state of the tree, so another behavior tree can be loaded.
     */
    void cleanTreeForReplacing();

    /**
     * Loads a behavior tree from a XML file.
     * @param a_file It is the XML file with the behavior tree to load.
     * @param a_hnd Handler pointer to the owner of this behavior tree.
     * @return true if the load was ok.
     */
    bool load(const char* a_file, Handler* hndBTOwner);

    // 为 Event 定制的函数
    bool load(const BCLib::Utility::HXmlNode rootElement, Handler* hndBTOwner);

    /**
     * Loads a behavior tree from a binary stream.
     * @param a_data It is the binary stream with the behavior tree to load.
     * @param a_hnd Handler pointer to the owner of this behavior tree.
     * @return true if the load was ok.
     */
    //bool loadStream(const char* a_data, Handler* a_hnd);

    /**
     * Pauses the behavior tree. Next execution will start from the current nodes.
     */
    void pause();

    /**
     * Stops the behavior tree. Next execution will start from the root.
     */
    void stop();

    /**
     * Plays the behavior tree.
     */
    void play();

    /**
     * Steps the behavior tree. Runs the tree one cycle
     */
    void step();

    // GETTERS //
    /**
     * Returns the ID of this behavior tree.
     * @return The ID of the behavior tree.
     */
    unsigned int getId()
    {
        return m_id;
    }

    /**
     * Returns the type of this behavior tree.
     * @return The type of behavior tree.
     */
    BT_Info getBTClassInfo()
    {
        return m_info;
    }

    /**
     * Returns the handler pointer of this behavior tree owner.
     * @return The owner handler pointer of this tree.
     */
    Handler* getOwner()
    {
        return m_hndOwner;
    }

    /**
     * Returns the private blackboard of this tree.
     * @return The blackboard pointer.
     */
    BCLib::Utility::CBlackBoard* getBlackboard()
    {
        return m_blackboard;
    }

    /**
     * Returns the stop protocol of this tree.
     * @return The stop protocol.
     */
    BT_StopProtocol getStopProtocol()
    {
        return m_stopProtocol;
    }

    // SETTERS //

    /**
     * Sets the identifier of the behavior tree
     * @param a_id The new identifier.
     */
    void setId(unsigned int a_id)
    {
        m_id = a_id;
    }

    /**
     * Sets it the tree has been initialized.
     * @param bInit Init value.
     */
    void setInitialized(bool bInit)
    {
        m_initialized = bInit;
    }

    /**
     * Sets the node that is being executed right now.
     * @param a_curNode The current node.
     */
    void setCurrentNode(CBTNode* a_curNode);

    /**
     * Sets the stop protocol for the node
     * @param a_stopProtocol The new stop protocol.
     */
    void setStopProtocol(BT_StopProtocol a_stopProtocol)
    {
        m_stopProtocol = a_stopProtocol;
    }

    bool FireEvent(int iEvent);

    void AddEventNode(int iEvent, CBTNode* pNode);

    CBTNode* GetRoot()
    {
        return (CBTNode *)m_root;
    }

    CBTNode* GetCurNode()
    {
        return m_currentNode;
    }

    const EventNodes& GetEventNodes()
    {
        return m_EventNodes;
    }

    CBehaviorTreeLibrary* GetBTLib()
    {
        return m_pBTLib;
    }

    void initEvent();

    void pushbackEvent(const std::string& eventName, CBTEventNode* eventNode);

    void singleEvent(const std::string& eventName);

    void activeEvent(const std::string& eventName);

    void inActiveEvent(const std::string& eventName);

    bool executeEvent();

    bool executeFinish();

    std::string File() const
    {
        return m_file;
    }
    void File(std::string val)
    {
        m_file = val;
    }
    std::string Folder() const
    {
        return m_folder;
    }
    void Folder(std::string val)
    {
        m_folder = val;
    }

private:
    CBehaviorTreeLibrary* m_pBTLib;

    // Behavior Event
    EventNodes m_EventNodes;
    CBTNode* m_ActiveEventNode;
    int m_iCurrentEvent;

    std::string m_file;
    std::string m_folder;

    //event
    EventList* m_EventList;

    EventQueueBuffer m_eventQueueBuffer;
    int m_eventQueueIndex;

    std::set<std::string> m_eventSet;
    std::set<std::string> m_activeEventList;

    CBTEventNode* m_currentEventNode;

#ifdef _DEBUG
public:
    const Nodes& GetDebugNodes()
    {
        return m_DebugNodes;
    }

private:
    Nodes m_DebugNodes;
#endif
};
}//BehaviorTree
}//MWLib

#endif//__MWLIB_BEHAVIORTREE_BEHAVIORTREE_H__
