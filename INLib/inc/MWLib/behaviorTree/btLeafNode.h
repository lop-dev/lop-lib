//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/btLeafNode.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_BTLEAFNODE_H__
#define __MWLIB_BEHAVIORTREE_BTLEAFNODE_H__

#include <MWLib/behaviorTree/btNode.h>

namespace MWLib
{
namespace BehaviorTree
{
/**
 * The BTLeafNode class.
 * Node for Leaf nodes. This is a virtual class, must be overridden.
 */
class CBTLeafNode : public CBTNode
{
public:
    //!Constructor initializes member variables.
    CBTLeafNode()
    :CBTNode(0)
    {
    }

    //!Constructor initializes member variables. Receives the pointer to the parent of this node.
    CBTLeafNode(CBTNode* a_parent)
    :CBTNode(a_parent)
    {
    }

    //!Destructor frees member variables.
    virtual ~CBTLeafNode()
    {
    };

    /**
     * This method must be overridden by the leaf node classes, so the actual behavior of the
     * action or condition is triggered.
     * @return The result of the execution.
     */
    virtual ENodeStatus step()
    {
        return START;
    }
};
}//BehaviorTree
}//MWLib
#endif//__MWLIB_BEHAVIORTREE_BTLEAFNODE_H__
