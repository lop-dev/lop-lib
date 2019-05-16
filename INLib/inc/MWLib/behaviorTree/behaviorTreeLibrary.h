//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/behaviorTreeLibrary.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_BEHAVIORTREELIBRARY_H__
#define __MWLIB_BEHAVIORTREE_BEHAVIORTREELIBRARY_H__

#include <BCLib/utility/baseDef.h>

/**
 * BehaviorTreeLibrary class.
 * The objects of this class can be used for the following purposes (in this order):
 *  1. Initialize the behavior tree library.
 *  2. Register custom leaf nodes.
 *  3. Finalize the behavior tree library.
 * Usage:
 *  1. Create a subclass of this, and override the method _registerLeafNodes(), including the
 *        macro calls that create the leaf node ccreator functions. (see
 *        BTLeafFactory::_registerLeafNodes() for and example on how to use this macros).
 *  2. When starting the application, call to the function Init();
 *  3. When closing the application, call to Finalize() so the library can free the memory it is usung.
 */


namespace MWLib
{
namespace BehaviorTree
{
class CBehaviorTreeLibrary
{
public:
    CBehaviorTreeLibrary()
    {
    }

    ~CBehaviorTreeLibrary()
    {
    }

    //! Initializes the behavior tree library.
    void Init();

    //! Closes the behavior tree library.
    void Finalize();

    // path
    void SetPath(const std::string& path)
    {
        m_strBTFilePath = path;
    }

    const std::string& GetPath()
    {
        return m_strBTFilePath;
    }

protected:
    //! This virtual function is called by Init() when initializating the Lirary.
    virtual void _registerLeafNodes()
    {
    }

    std::string m_strBTFilePath;
};
}//BehaviorTree
}//MWLib

#endif//__MWLIB_BEHAVIORTREE_BEHAVIORTREELIBRARY_H__
