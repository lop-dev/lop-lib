//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/composite/btSelectorNode.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_COMPOSITE_BTSELECTORNODE_H__
#define __MWLIB_BEHAVIORTREE_COMPOSITE_BTSELECTORNODE_H__

#include <MWLib/behaviorTree/btNode.h>

namespace MWLib
{
namespace BehaviorTree
{
/**
 * CBTSelecNode class.
 * This is the Selector Node class. Executes its children from left to right,
 * bailing out if one of them suceeds.
 */
class CBTSelecNode : public CBTNode
{
public:

    //!Constructor initializes member variables.
    CBTSelecNode();

    //!Constructor initializes member variables. Receives the parent of this new node.
    CBTSelecNode(CBTNode* a_parent);

    /**
     * This method is used to reset the values of the node to its initial state.
     * Used when the user hits the Stop button in the editor. Every subclass must overwrite this method
     * to reset its state (filter variables, current child in selectors and conditions, etc...).
     * This may be used too in class leaf nodes to reset user's leaves.
     */
    void reset();

    //!Handles what to do if this node is disabled.
    void handleDisabledChild();

    /**
     * This function is called when this node has to be executed.
     * Executes its children from left to right, bailing out if one of them fails.
     */
    void execute();

    /**
     * This method is used to initialize the node (suggestion: read leaf node parameters here from property table)
     */
    void init();

    /**
     * Called when my child ends its execution
     * @param a_result Result of the child execution.
     */
    void update(ENodeStatus a_result);
};
}//BehaviorTree
}//MWLib
#endif//__MWLIB_BEHAVIORTREE_COMPOSITE_BTSELECTORNODE_H__
