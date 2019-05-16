//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/btLinkNode.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_BTLINKNODE_H__
#define __MWLIB_BEHAVIORTREE_BTLINKNODE_H__


#include <MWLib/behaviorTree/btNode.h>

namespace MWLib
{
namespace BehaviorTree
{
/**
 * BTLinkNode class.
 * This is the Root Node class. It is the top of the tree and it can only have one child.
 */
class CBTLinkNode : public CBTNode
{
public:
    //!Constructor initializes member variables.
    CBTLinkNode();

    //!Constructor initializes member variables. Receives the parent of this new node, which normally is Null.
    CBTLinkNode(CBTNode* a_parent);

    /**
     * This method is used to reset the values of the node to its initial state.
     * Used when the user hits the Stop button in the editor. Every subclass must overwrite this method
     * to reset its state (filter variables, current child in selectors and conditions, etc...).
     * This may be used too in class leaf nodes to reset user's leaves.
     */
    void reset();

    /**
     * This function is called when this node has to be executed.
     * Executes its children from left to right, bailing out if one of them fails.
     */
    void execute();

    /**
     * Called when my child ends its execution
     * @param a_result Result of the child execution.
     */
    void update(ENodeStatus a_result);

    /**
     * Returns the number of children this node has
     * @return The number of children.
     */
    int getNumChildren()
    {
        return m_Children.size();
    }
};
}//BehaviorTree
}//MWLib
#endif//__MWLIB_BEHAVIORTREE_BTLINKNODE_H__
