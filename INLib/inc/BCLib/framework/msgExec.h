//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/msgExec.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_MSGEXEC_H__
#define __BCLIB_FRAMEWORK_MSGEXEC_H__

#include <BCLib/utility/pointer.h>
#include <BCLib/utility/function.h>
#include <BCLib/framework/thdMsgLabel.h>

namespace BCLib
{
namespace Framework
{
class BCLIB_FRAMEWORK_API CMsgExec
{
public:
    typedef void (*MessageExecFun)(SThdMsgLabel*, SMessage*);

public:
    CMsgExec(MessageExecFun execFun);
    CMsgExec(const BCLib::Utility::CFunction<void, SThdMsgLabel*, SMessage*>& execFun);

    template<class NAME>
    CMsgExec(void (NAME::* execFun)(SThdMsgLabel*, SMessage*), NAME* object)
    :m_execFun(NULL)
    {
        m_execFun = new BCLib::Utility::CFunctionObject<void, NAME, SThdMsgLabel*, SMessage*>(execFun, object);
    }

    virtual ~CMsgExec();
    virtual void executeMessage(SThdMsgLabel* msgLabel, SMessage* msg);

private:
    BCLib::Utility::CFunction<void, SThdMsgLabel*, SMessage*>* m_execFun;
};
typedef Utility::CPointer<CMsgExec> CMsgExecPtr;
}//Framework
}//BCLib


#endif//__BCLIB_FRAMEWORK_MSGEXEC_H__
