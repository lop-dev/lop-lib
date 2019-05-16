//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/decorator/loggerDecorator.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_DECORATOR_LOGGERDECORATOR_H__
#define __MWLIB_BEHAVIORTREE_DECORATOR_LOGGERDECORATOR_H__

#include <MWLib/behaviorTree/btDecoratorNode.h>

namespace MWLib
{
namespace BehaviorTree
{
/**
 * LoggerFilter class.
 * Prints on the console that this filter's child is going to be executed.
 */
class CLoggerDecorator : public CBTDecoratorNode
{
protected:

    /**
    * Name of this logger. It is printed in the decorator() function for a quick recognition
    * in the output window.
    */
    std::string m_name;

public:

    //!Constructor initializes member variables.
    CLoggerDecorator();

    /**
     * Constructor initializes member variables. Receives the name of the filter.
     * @param a_name Name of this logger. It is printed in the decorator() function for a quick recognition
     */
    CLoggerDecorator(std::string a_name);

    /**
     * Constructor initializes member variables. Receives this node's parent and the name of the filter.
     * @param a_parent The parent of this node.
     * @param a_name Name of this logger. It is printed in the decorator() function for a quick recognition
     */
    CLoggerDecorator(CBTNode* a_parent, std::string a_name);

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
#endif//__MWLIB_BEHAVIORTREE_DECORATOR_LOGGERDECORATOR_H__
