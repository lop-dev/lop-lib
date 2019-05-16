//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/decorator/nonDecorator.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_DECORATOR_NONDECORATOR_H__
#define __MWLIB_BEHAVIORTREE_DECORATOR_NONDECORATOR_H__

#include <MWLib/behaviorTree/btDecoratorNode.h>

namespace MWLib
{
namespace BehaviorTree
{
/**
 * NonFilter class.
 * Inverts the result of this node's child result.
 */
class CNonDecorator : public CBTDecoratorNode
{
public:

    //!Constructor initializes member variables.
    CNonDecorator();

    /**
     * Constructor initializes member variables.
     * @param a_parent The parent of this node.
     */
    CNonDecorator(CBTNode* a_parent);

    //!Function that codifies the behavior of this filter.
    ENodeStatus decorator(ENodeStatus a_status);

    /**
     * This method is used to reset the values of the node to its initial state.
     * Used when the user hits the Stop button in the editor. Every subclass must overwrite this method
     * to reset its state (filter variables, current child in selectors and conditions, etc...).
     * This may be used too in class leaf nodes to reset user's leaves.
     */
    void reset();

    /**
     * This function is called when this node has to be executed.
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
#endif//__MWLIB_BEHAVIORTREE_DECORATOR_NONDECORATOR_H__
