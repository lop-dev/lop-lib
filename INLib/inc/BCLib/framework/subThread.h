//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/subThread.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_SUBTHREAD_H__
#define __BCLIB_FRAMEWORK_SUBTHREAD_H__

#include <BCLib/utility/thread/thread.h>
#include <BCLib/framework/threadFrame.h>

namespace BCLib
{
namespace Framework
{
class BCLIB_FRAMEWORK_API CSubThread : public CThreadFrame, public Utility::CThread
{
public:
    CSubThread();
    virtual ~CSubThread();

protected:
    virtual void _run();
};
}//Framework
}//BCLib


#endif//__BCLIB_FRAMEWORK_SUBTHREAD_H__
