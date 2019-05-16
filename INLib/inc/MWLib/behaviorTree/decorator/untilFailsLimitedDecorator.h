//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/decorator/untilFailsLimitedDecorator.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_DECORATOR_UNTILFAILSLIMITEDDECORATOR_H__
#define __MWLIB_BEHAVIORTREE_DECORATOR_UNTILFAILSLIMITEDDECORATOR_H__

#include <MWLib/behaviorTree/btDecoratorNode.h>

namespace MWLib
{
namespace BehaviorTree
{
/**
 * UntilFailsLimitedFilter class.
 * Executes this node's child repeatedly until it receives a FAILULRE, or
 * an execution counter reaches a limit.
 */
class CUntilFailsLimitedDecorator : public CBTDecoratorNode
{
private:

    //! Limit of executions that this filter will 'bounce' (min value: 1)
    int m_limitUsage;

    // Number of consecutve executions
    int m_currentUsage;

public:

    //!Constructor initializes member variables.
    CUntilFailsLimitedDecorator();

    /**
     * Constructor initializes member variables.
     * @param a_parent The parent of this node.
     */
    CUntilFailsLimitedDecorator(CBTNode* a_parent);

    /**
     * Constructor initializes member variables.
     * @param a_parent The parent of this node.
     * @param a_limitUsage Limit of consecutive executions.
     */
    CUntilFailsLimitedDecorator(CBTNode* a_parent, int a_limitUsage);

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
#endif//__MWLIB_BEHAVIORTREE_DECORATOR_UNTILFAILSLIMITEDDECORATOR_H__
