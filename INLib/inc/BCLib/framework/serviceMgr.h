//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/serviceMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_SERVICEMGR_H__
#define __BCLIB_FRAMEWORK_SERVICEMGR_H__

#include <BCLib/utility/singleton.h>
#include <BCLib/utility/hashMap.h>
#include <BCLib/utility/io/dllFile.h>
#include <BCLib/utility/thread/mutex.h>
#include <BCLib/framework/baseDef.h>
#include <BCLib/framework/service.h>

namespace BCLib
{
namespace Framework
{
class BCLIB_FRAMEWORK_API CServiceMgr
{
    BCLIB_SINGLETON_DECLARE(CServiceMgr);

public:
    CServiceMgr();
    virtual ~CServiceMgr();

    bool openDllFile(const std::string& fileName);
    void closeDllFile(const std::string& fileName);
    void closeAllDllFile();

    bool createService(CThreadFrame* threadFrame, const std::string& serviceName, const std::string& serviceNewName);

private:
    BCLib::Utility::CHashMap<std::string, BCLib::Utility::CDllFile*> m_serviceDllFileHash;
    BCLib::Utility::CMutex m_mutex;
};
}//Framework
}//BCLib

#endif//__BCLIB_FRAMEWORK_SERVICEMGR_H__
