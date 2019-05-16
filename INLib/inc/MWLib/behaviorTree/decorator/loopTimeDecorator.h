//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/decorator/loopTimeDecorator.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_DECORATOR_LOOPTIMEDECORATOR_H__
#define __MWLIB_BEHAVIORTREE_DECORATOR_LOOPTIMEDECORATOR_H__

#include <MWLib/behaviorTree/btDecoratorNode.h>

namespace MWLib
{
namespace BehaviorTree
{
class CLoopTimeDecorator : public CBTDecoratorNode
{
protected:

    // in millisecond
    int m_timer;
    int m_startTime;
    bool m_bExcuting;

public:

    CLoopTimeDecorator();

    CLoopTimeDecorator(float a_timeInSecond);

    CLoopTimeDecorator(CBTNode* a_parent, float a_timeInSecond);

    void reset();

    ENodeStatus decorator();

    void execute();

    void init();

    void update(ENodeStatus a_result);

    void executingUpdate();
};
}//BehaviorTree
}//MWLib
#endif//__MWLIB_BEHAVIORTREE_DECORATOR_LOOPTIMEDECORATOR_H__
