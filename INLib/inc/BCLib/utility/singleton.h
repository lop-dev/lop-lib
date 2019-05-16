//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/singleton.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_SINGLETON_H__
#define __BCLIB_UTILITY_SINGLETON_H__

#include <BCLib/utility/baseDef.h>
#include <BCLib/utility/thread/mutex.h>
#include <vector>


namespace BCLib
{
namespace Utility
{
#define BCLIB_SINGLETON_DECLARE(classname)                      \
public:                                                         \
    static classname& singleton();                              \
private:                                                        \
    template<class>                                             \
    friend void BCLib::Utility::deleteSingleton(void*)

#define BCLIB_SINGLETON_DEFINE(classname)                       \
    BCLIB_SINGLETON_DEFINE_WITH_LEVELID(classname, 100000)

#define BCLIB_SINGLETON_DEFINE_WITH_LEVELID(classname, levelID) \
    classname& classname::singleton()                           \
    {                                                           \
        static classname* s_singleton = NULL;                   \
        if(s_singleton)                                         \
            return *s_singleton;                                \
        BCLib::Utility::CSingletonMgr::singleton().lock();      \
        if(s_singleton)                                         \
        {                                                       \
            BCLib::Utility::CSingletonMgr::singleton().unLock();\
            return *s_singleton;                                \
        }                                                       \
        s_singleton = new classname();                          \
        if(NULL == s_singleton)                                 \
        {                                                       \
            BCLib::Utility::CSingletonMgr::singleton().unLock();\
            BCLib::Utility::abend4Singleton();                  \
        }                                                       \
        BCLib::Utility::CSingletonMgr::singleton().             \
        registerSingleton((void**)&s_singleton, levelID,        \
        BCLib::Utility::deleteSingleton<classname>);            \
        BCLib::Utility::CSingletonMgr::singleton().unLock();    \
        return *s_singleton;                                    \
    }


template<class CLASS>
void deleteSingleton(void* obj)
{
    delete (CLASS*)obj;
}

class BCLIB_UTILITY_API CSingletonMgr
{
public:
    static CSingletonMgr& singleton();
    void lock();
    void unLock();
    typedef void(*DeleteFunc)(void*);
    void registerSingleton(void** obj, BCLib::uint32 levelID, DeleteFunc func);

private:
    CSingletonMgr();
    ~CSingletonMgr();

private:
    BCLib::Utility::CMutex m_mutex;
    struct item
    {
        void** m_obj;
        BCLib::uint32 m_levelID;
        DeleteFunc m_deleteFunc;

        item(void** obj, BCLib::uint32 levelID, DeleteFunc func)
        {
            m_obj = obj;
            m_levelID = levelID;
            m_deleteFunc = func;
        }
    };
    typedef std::vector<item> DestroyFuncList;
    DestroyFuncList m_funcList;
    bool m_bInDestroy;
};

void BCLIB_UTILITY_API abend4Singleton();
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_SINGLETON_H__
