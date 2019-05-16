//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/btDecoratorNode.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_BTDECORATORNODE_H__
#define __MWLIB_BEHAVIORTREE_BTDECORATORNODE_H__


#include <MWLib/behaviorTree/btNode.h>

namespace MWLib
{
namespace BehaviorTree
{
/**
 * The CBTDecoratorNode class.
 * Node for Filter nodes. This is a virtual class, must be overridden to create new filters.
 */
class CBTDecoratorNode : public CBTNode
{
protected:

    //!Indicates if the child of this filter has been executed.
    bool m_childLaunched;

public:

    //!Constructor initializes member variables.
    CBTDecoratorNode();

    //!Constructor initializes member variables.  Receives the pointer to the parent of this node.
    CBTDecoratorNode(CBTNode* a_parent);

    //!Function that codifies the behavior of this filter/decorator.
    virtual ENodeStatus decorator()
    {
        return SUCCESS;
    }

    /**
     * This function is called when this node has to be executed.
     * @return The result of the execution
     */
    virtual void execute()
    {
    }
};
}//BehaviorTree
}//MWLib
#endif//__MWLIB_BEHAVIORTREE_BTDECORATORNODE_H__
