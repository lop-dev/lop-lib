//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/btNode.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_BTNODE_H__
#define __MWLIB_BEHAVIORTREE_BTNODE_H__

#include <BCLib/utility/baseDef.h>

namespace MWLib
{
namespace BehaviorTree
{
/** @file */ //This line allows Doxygen to see the enums that are defined outside the scope of a class

/*! Status for this node. */
enum ENodeStatus
{
    /*!< The node is ready to start. */
    START       = 0,
    /*!< The node returned failure. */
    FAILURE     = 1,
    /*!< The node returned success.  */
    SUCCESS     = 2,
    /*!< The node is currently executing. */
    EXECUTING   = 3
};

/*! Breakpoint type stated for this node. */
enum EBreakpointMode
{
    /*!< This node has no breakpoint. */
    NO_BREAKPOINT   = 0,
    /*!< The breakpoint will be triggered before executing the node. */
    ENTERING        = 1,
    /*!< The breakpoint will be triggered after executing the node.  */
    LEAVING         = 2,
    /*!< The breakpoint will be triggered before AND after executing the node.  */
    ENTER_AND_LEAVE = 3
};

#ifdef VERBOSE_BT
#include <conio.h>
#endif

class CBTNode;
class CBehaviorTree;

/*! Vector of nodes. Used to represent the data structure that holds the children of this node. */
typedef std::vector<CBTNode*> ChildNodes;


/*! Hashtable of properties. All the node preperties defined in the XML file are stored here,
    as pairs <Key,Value>: <String,String>. */
typedef std::map<std::string, std::string> PropertyTable;

/**
 * The core CBTNode class.
 * This is the basic node for a Behavior Tree. All the nodes in this library
 * have this as the top class of their hierarchy.
 */
class CBTNode
{
    //! Number of children this node has.
    static int m_NodeCount;

protected:

    //! Index of this node in its parent vector of nodes.
    int m_index;

    //! Pointer to the parent of this node.
    CBTNode* m_parent;

    //! Vector of nodes, children of this node.
    ChildNodes m_Children;

    //! Iterator for the children vector of this node.
    ChildNodes::const_iterator m_itChild;

    //! Vector of nodes, children event of this node.
    int m_EventId;

    //! Reference to the Behavior tree this node belongs.
    CBehaviorTree* m_tree;

    //! Indicates if this node has an active breakpoint in a debug session.
    EBreakpointMode m_breakpointMode;

    //! ID of this node, given and used by the BT editor in a debug session.
    int m_id;

    //! Indicates if this node is enabled or not. If the node is disabled, it wont execute.
    bool m_enabled;

    //! Name of this node. It is only used for debug purposes.
    std::string m_nodeName;

    std::string m_typeName;

    /**
     * List of properties defined for this node.
     */
    PropertyTable m_properties;

    /**
     * Status of this node
     */
    ENodeStatus m_nodeStatus;


public:

    //!Constructor initializes member variables.
    CBTNode();

    /**
     * Constructor initializes member variables. Receives and sets a pointer to the parent of this new node.
     * @param a_parent Is the parent of this node.
     */
    CBTNode(CBTNode* a_parent);

    //!Destructor frees member variables.
    virtual ~CBTNode();

    /**
     * This function is called when this node has to be executed.
     */
    virtual void execute()
    {
    }

    /**
     * Initializes the node. Calls initNode() method on children nodes.
     */
    void initNode();

    /**
     * Logs the result of this node
     */
    void logNodeResult();

    /**
     * Resets the node and calls resetNode() method on children nodes.
     */
    void resetNode();

    /**
     * This method is used to initialize the node (suggestion: read leaf node parameters here from property table)
     */
    virtual void init()
    {
    }

    //!Handles what to do if this node is disabled.
    virtual void handleDisabledChild();

    /**
     * Establishes the enable mode of this node AND its children in cascade.
     * @param a_enableMode True if enabled, false if disabled.
     */
    void setEnabledMode(bool a_enableMode);

    /**
     * This method is used to reset the values of the node to its initial state.
     * Used when the user hits the Stop button in the editor. Every subclass must overwrite this method
     * to reset its state (filter variables, current child in selectors and conditions, etc...).
     * This may be used too in class leaf nodes to reset user's leaves.
     */
    virtual void reset()
    {
    }

    /**
     * Adds a new property to the table of node properties.
     * @param a_key Key for the entry.
     * @param a_value Value of the entry.
     */
    void addProperty(std::string a_key, std::string a_value);

    /**
     * Adds a new node as a child of this.
     * @param a_Node The new child.
     */
    void pushBack(CBTNode* a_Node);

    /**
     * Searches a node by its ID. The ID is given by the BT editor in a debug session.
     * @param a_id ID of the node to look for.
     * @return the node if found, null if it was no found.
     */
    CBTNode* searchNode(int a_id);

    /**
     * Manages breakpoint flags and logging, before
     * calling the virtual function execute() to execute this node.
     * @see execute()
     */
    void executeDbg();

    /**
     * Called when my child ends its execution
     * @param a_result Result of the child execution.
     */
    virtual void update(ENodeStatus a_result)
    {
    }

    virtual void executingUpdate();

    /**
     * Function called from the behavior tree so this node contines with the
     * execution from last step position, both executing the same node again
     * or notifying the result to the parent (if applicable).
     */
    void notifyResult();

    // GETTERS //

    /**
     * Returns the behavior tree this node belongs to.
     * @return m_tree, a pointer to the behavior tree this node belongs to.
     */
    CBehaviorTree* getBehaviorTree()
    {
        return m_tree;
    }

    /**
     * Returns if this node has the breakpoint flag set to true in a debug session.
     * @return m_breakpointMode, type of the breakpoint set for this node (@see BreakpointMode).
     */
    EBreakpointMode getBreakpointMode()
    {
        return m_breakpointMode;
    }

    /**
     * Returns the id given by the BT editor.
     * @return m_id, the id given by the BT editor.
     */
    int getId()
    {
        return m_id;
    }

    /**
     * Returns the name of this node. This is used only to help debugging.
     * @return m_name, the name of this node.
     */
    std::string getName()
    {
        return m_nodeName;
    }

    /**
     * Returns the status of this node.
     * @return m_nodeStatus, the status of this node.
     */
    ENodeStatus getNodeStatus()
    {
        return m_nodeStatus;
    }

    // SETTERS //

    /**
     * Assigns the behavior tree this node belongs to.
     * @param a_bt The behavior tree this node belongs to.
     */
    void setBehaviorTree(CBehaviorTree* a_bt)
    {
        m_tree = a_bt;
    }

    /**
     * Sets the breakpoint mode. See BreakpointMode for different values.
     * @param a_bp Value of the breakpoint flag.
     */
    void setDebugMode(EBreakpointMode a_bp)
    {
        m_breakpointMode = a_bp;
    }

    /**
     * Sets an ID for this node. Used by the BT Editor to identify uniquely this node.
     * @param a_id ID for this node.
     */
    void setId(int a_id)
    {
        m_id = a_id;
    }

    /**
     * Sets a name for this node. Used for debug purposes.
     * @param a_nodeName Name for this node.
     */
    void setName(std::string a_nodeName)
    {
        m_nodeName = a_nodeName;
    }

    void setType(std::string a_typeName)
    {
        m_typeName = a_typeName;
    }

    const std::string& GetType()
    {
        return m_typeName;
    }

    void setEvent(int eventId)
    {
        m_EventId = eventId;
    }

    CBTNode* GetBackChild();

    void PopBackChild();

    void searchNode(const std::string& name, std::vector<CBTNode*>& nodes);

    bool IsEnable()
    {
        return m_enabled;
    }

    CBTNode* GetParent()
    {
        return m_parent;
    }

    const std::string& GetProperty(const std::string& prop)
    {
        return m_properties[prop];
    }

    void addEvent(std::string eventName);
    void activeEvents();
    void inActiveEvents();

private:
    std::set<std::string> m_eventSet;
};
}//BehaviorTree
}//MWLib
#endif//__MWLIB_BEHAVIORTREE_BTNODE_H__
