//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/msgExecMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_MSGEXECMGR_H__
#define __BCLIB_FRAMEWORK_MSGEXECMGR_H__

#include <BCLib/utility/hashMap.h>
#include <BCLib/framework/msgExec.h>

namespace BCLib
{
namespace Framework
{
class BCLIB_FRAMEWORK_API CMsgExecMgr
{
public:
    CMsgExecMgr();
    virtual ~CMsgExecMgr();

    bool executeMessage(SThdMsgLabel* msgLabel, SMessage* msg);

    bool addMsgExecPtr(uint16 type, uint16 id, CMsgExecPtr& msgExecPtr);
    void delMsgExecPtr(uint16 type, uint16 id);

protected:
    virtual bool _createMsgExecPtr(uint16 type, uint16 id, CMsgExecPtr& msgExecPtr) = 0;
    virtual bool _createMsgExecPtr(uint16 type, CMsgExecPtr& msgExecPtr) = 0;

private:
    Utility::CHashMap<uint32, CMsgExecPtr> m_msgExecHashMap;
    Utility::CHashMap<uint16, CMsgExecPtr> m_typeMsgExecHashMap;
};
}//Framework
}//BCLib

#define BCLIB_MSGEXEC_DEFINE_BEGIN(type_p) \
    switch(type_p) {

#define BCLIB_MSGEXEC_DEFINE_TYPE(type, fun_p, fun)\
        case type: {\
            fun_p = fun; \
            if (fun_p != NULL) {\
                return true;\
            }\
            break;\
        }

#define BCLIB_MSGEXEC_DEFINE_TYPE_BEGIN(type, id_p) \
        case type: {\
            switch(id_p) {

#define BCLIB_MSGEXEC_DEFINE_ID(fun_p, id, fun) \
                case id: {\
                    fun_p = fun;\
                    if (fun_p != NULL)    { \
                        return true;\
                    }\
                    break;\
                }

#define BCLIB_MSGEXEC_DEFINE_TYPE_END \
                default:{\
                    break;\
                }\
            }\
            break;\
        }

#define BCLIB_MSGEXEC_DEFINE_END \
        default:{\
            break;\
        }\
    }

#endif//__BCLIB_FRAMEWORK_MSGEXECMGR_H__
