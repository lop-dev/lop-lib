//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/decorator/counterDecorator.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_DECORATOR_COUNTERDECORATOR_H__
#define __MWLIB_BEHAVIORTREE_DECORATOR_COUNTERDECORATOR_H__

#include <MWLib/behaviorTree/btDecoratorNode.h>

namespace MWLib
{
namespace BehaviorTree
{
/**
 * CounterFilter class.
 * Limits the number of executions that this filter's child can afford.
 */
class CCounterDecorator : public CBTDecoratorNode
{
protected:

    //! Number of times that this filter's child can afford.
    int m_maxTimes;

    //! Number of this filter's child executions.
    int m_times;

public:

    //!Constructor initializes member variables.
    CCounterDecorator();

    /**
     * Constructor initializes member variables. Receives the number of times that this filter's child can afford.
     * @param a_times Number of times that this filter's child can afford.
     */
    CCounterDecorator(int a_times);

    /**
     * Constructor initializes member variables. Receives this node's parent and the number of times
     * that this filter's child can afford.
     * @param a_parent The parent of this node.
     * @param a_times Number of times that this filter's child can afford.
     */
    CCounterDecorator(CBTNode* a_parent, int a_times);

    /**
     * This method is used to reset the values of the node to its initial state.
     * Used when the user hits the Stop button in the editor. Every subclass must overwrite this method
     * to reset its state (filter variables, current child in selectors and conditions, etc...).
     * This may be used too in class leaf nodes to reset user's leaves.
     */
    void reset();

    //!Function that codifies the behavior of this filter.
    ENodeStatus decorator();

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
#endif//__MWLIB_BEHAVIORTREE_DECORATOR_COUNTERDECORATOR_H__
