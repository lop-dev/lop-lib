//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/composite/btRandomSelNode.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_COMPOSITE_BTRANDOMSELNODE_H__
#define __MWLIB_BEHAVIORTREE_COMPOSITE_BTRANDOMSELNODE_H__

#include <MWLib/behaviorTree/btNode.h>

namespace MWLib
{
namespace BehaviorTree
{
class CBTRandomSelNode : public CBTNode
{
public:

    CBTRandomSelNode();

    CBTRandomSelNode(CBTNode* a_parent);

    void reset();

    void handleDisabledChild();

    void execute();

    void init();

    void update(ENodeStatus a_result);

private:
    ChildNodes m_enableNodes;

    struct RandomRange
    {
        RandomRange()
        :m_min(0)
        ,m_max(0)
        {
        }

        RandomRange(int min, int max)
        :m_min(min)
        ,m_max(max)
        {
        }

        int m_min;
        int m_max;
    };

    typedef std::map<CBTNode*, RandomRange> RandomWeights;
    RandomWeights m_Weights;

    int m_iTotalWeight;
};
}//BehaviorTree
}//MWLib
#endif//__MWLIB_BEHAVIORTREE_COMPOSITE_BTRANDOMSELNODE_H__
