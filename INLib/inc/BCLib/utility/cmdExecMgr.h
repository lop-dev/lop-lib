//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/cmdExecMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_CMDEXECMGR_H__
#define __BCLIB_UTILITY_CMDEXECMGR_H__

#include <string>
#include <BCLib/utility/hashMap.h>
#include <BCLib/utility/command.h>
//#include <BCLib/utility/cmdExec.h>

namespace BCLib
{
namespace Utility
{
class BCLIB_UTILITY_API CCmdExecMgr : protected CCommand
{
public:
    typedef int(*FCmdExecCb)(CProperties&, void*);
    typedef CFunction<int, CProperties&, void*> TCmdExecFun;

public:
    CCmdExecMgr();
    virtual ~CCmdExecMgr();

    virtual int executeCommand(const std::string& cmdLine, void* tempPara);

    bool addCmdExecPtr(const std::string& name, FCmdExecCb funCmdExec);
    bool addCmdExecPtr(const std::string& name, const TCmdExecFun& funCmdExec);

    template<class NAME>
    bool addCmdExecPtr(const std::string& name, int(NAME::* funCmdExec)(CProperties&, void*), NAME* object)
    {
        return addCmdExecPtr(name, CFunctionObject<int, NAME, CProperties&, void*>(funCmdExec, object));
    }

    void delCmdExecPtr(const std::string& name);

protected:
    virtual bool _createCmdExecPtr(const std::string& name, TCmdExecFun*& cmdExecPtr);

private:
    virtual int _execCmd(const std::string& command, CProperties& properties, void* tempPara);
    void _remove();

private:
    typedef Utility::CHashMap<std::string, TCmdExecFun*> THashMapCmdExec;
    typedef THashMapCmdExec::iterator THashMapCmdExecIter;

    THashMapCmdExec m_cmdExecHashMap;
};
}//Utility
}//BCLib

#define BCLIB_CMDEXEC_DEFINE_BEGIN(name, funPtr)\
    const std::string& cmdName = name;\
    TCmdExecFun*& cmdFunPtr = funPtr;

#define BCLIB_CMDEXEC_GLOBAL_FUN(name, fun)\
    if (cmdName == name) {\
        cmdFunPtr = new BCLib::Utility::CFunction<int, BCLib::Utility::CProperties&, void*>(fun);\
        return true;\
    }

#define BCLIB_CMDEXEC_OBJECT_FUN(name, className, fun, object)\
    if (cmdName == name) {\
        cmdFunPtr = new BCLib::Utility::CFunctionObject<int, className, BCLib::Utility::CProperties&, void*>(fun, object);\
        return true;\
    }

#define BCLIB_CMDEXEC_DEFINE_END()


#endif//__BCLIB_UTILITY_CMDEXECMGR_H__
