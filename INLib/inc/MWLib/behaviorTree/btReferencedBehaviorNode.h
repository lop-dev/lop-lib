//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/btReferencedBehaviorNode.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_BTREFERENCEDBEHAVIORNODE_H__
#define __MWLIB_BEHAVIORTREE_BTREFERENCEDBEHAVIORNODE_H__

#include <MWLib/behaviorTree/btLinkNode.h>

namespace MWLib
{
namespace BehaviorTree
{
class Handler;

class CBTReferencedBehaviorNode : public CBTLinkNode
{
public:
    CBTReferencedBehaviorNode(CBTNode* a_parent, const std::string& filename, const std::string& foldername);
    ~CBTReferencedBehaviorNode();

    void Load(Handler* hndBTOwner);

    virtual    void reset();

    virtual void update(ENodeStatus a_result);

    virtual void execute();

private:
    CBTReferencedBehaviorNode();

private:
    std::string m_strFileName;
    std::string m_strFolderName;
};
}//BehaviorTree
}//MWLib
#endif//__MWLIB_BEHAVIORTREE_BTREFERENCEDBEHAVIORNODE_H__
