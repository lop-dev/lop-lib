//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/decorator/timerDecorator.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_DECORATOR_TIMERDECORATOR_H__
#define __MWLIB_BEHAVIORTREE_DECORATOR_TIMERDECORATOR_H__

#include <MWLib/behaviorTree/btDecoratorNode.h>

namespace MWLib
{
namespace BehaviorTree
{
/**
 * TimerFilter class.
 * Locks the access to this node's child during a X amount of time since the last execution.
 */
class CTimerDecorator : public CBTDecoratorNode
{
protected:

    //! Time to lock the access after execution.
    BCLib::uint32 m_timer;

    //! Time when the locks expires.
    BCLib::uint32 m_unlockTime;

public:

    //!Constructor initializes member variables.
    CTimerDecorator();

    /**
     * Constructor initializes member variables.
     * @param a_time Time to lock the access after execution.
     */
    CTimerDecorator(float a_time);

    /**
     * This method is used to reset the values of the node to its initial state.
     * Used when the user hits the Stop button in the editor. Every subclass must overwrite this method
     * to reset its state (filter variables, current child in selectors and conditions, etc...).
     * This may be used too in class leaf nodes to reset user's leaves.
     */
    void reset();

    /**
     * Constructor initializes member variables.
     * @param a_parent The parent of this node.
     * @param a_time Time to lock the access after execution.
     */
    CTimerDecorator(CBTNode* a_parent, float a_time);

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
#endif//__MWLIB_BEHAVIORTREE_DECORATOR_TIMERDECORATOR_H__
