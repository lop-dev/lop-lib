//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/classLeafNode.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_CLASSLEAFNODE_H__
#define __MWLIB_BEHAVIORTREE_CLASSLEAFNODE_H__

#include <MWLib/behaviorTree/btLeafNode.h>

namespace MWLib
{
namespace BehaviorTree
{
class CClassLeafNode;
class Handler;


enum EBTLeafType
{
    BTLEAF_UNDEF        = 0,
    BTLEAF_CONDITION    = 1,
    BTLEAF_ACTION       = 2,
    BTLEAF_EVENT        = 3
};

/*! Callback for leaf node classes creation. */
template<typename T>
CClassLeafNode* createLeafNode(CBTNode* parent, Handler* a_id)
{
    return new T(parent, a_id);
}

/**
 * The core ClassLeafNode class.
 * This is a virtual class, and must be used as the base class for leaf node classes.
 * Actions and condition classes must inherit from this class, overriding the method step()
 * to perform the actual behavior of the node.
 */
class CClassLeafNode : public CBTLeafNode
{
protected:

    /**
      * Handler that identifies the owner of this node. That's it, the unit that
      * will use this node at runtime.
      * @see Handler
      */
    Handler* m_leafOwner;

    /**
     * It indicates if it is an action or a condition
     */
    EBTLeafType m_leafType;

public:

    /**
     * Constructor initializes member variables.
     * @param a_owner Is the handler that identifies the owner of this node.
     */
    CClassLeafNode(Handler* a_owner);

    /**
     * Constructor initializes member variables. Receives and sets a pointer to the parent of this new node.
     * @param a_owner Is the handler that identifies the owner of this node.
     * @param parent Is the parent of this node.
     */
    CClassLeafNode(CBTNode* parent, Handler* a_owner);

    //!Constructor initializes member variables.
    CClassLeafNode();

    //!Destructor frees member variables.
    ~CClassLeafNode();

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
     * This method must be overridden by the leaf node classes, so the actual behavior of the
     * action or condition is triggered.
     * @return The result of the execution.
     */
    virtual ENodeStatus step()
    {
        return START;
    }

    /**
     * Gets the handler of the owner of this node.
     * @return m_leafOwner, The handler of the owner of this node.
     */
    Handler* getHandler()
    {
        return m_leafOwner;
    }

    /**
     * Executes this leaf node. Takes care of logging and calls step() to trigger the
     * leaf node behavior.
     * @return The result of the execution.
     */
    void execute();

    /**
     * Sets the leaf type (action / condition)
     * @param a_leafType Type of the leaf node.
     */
    void setLeafType(EBTLeafType a_leafType)
    {
        m_leafType = a_leafType;
    }
};
}//BehaviorTree
}//MWLib
#endif//__MWLIB_BEHAVIORTREE_CLASSLEAFNODE_H__
