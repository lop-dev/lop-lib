//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/memory/memPoolMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_MEMORY_MEMPOOLMGR_H__
#define __BCLIB_UTILITY_MEMORY_MEMPOOLMGR_H__

#include <BCLib/utility/objectPool.h>
#include <BCLib/utility/thread/mutex.h>
#include <BCLib/utility/singleton.h>

#if defined(BCLIB_USE_MEMPOLL)
#define BCLIB_MEMPOLL_DEFINE()                                          \
public:                                                                 \
    static void* operator new(size_t size)                              \
    {                                                                   \
        return BCLib::Utility::CMemPoolMgr::singleton().                \
            allocMem(size);                                             \
    }                                                                   \
    static void operator delete(void* p)                                \
    {                                                                   \
        BCLib::Utility::CMemPoolMgr::singleton().freeMem(p);            \
    }                                                                   \
protected:                                                              \
    static void* operator new [](size_t) throw() { return NULL; }       \
    static void operator delete [](void*) throw() {}                    \
    static void* operator new(size_t, void*) throw() { return NULL; }   \
    static void operator delete(void*, void*) throw() {}                \
    static void* operator new [](size_t, void*) throw() { return NULL; }\
    static void operator delete [](void*, void*) throw() {}
#else
#define BCLIB_MEMPOLL_DEFINE()                                          \
public:                                                                 \
    static void* operator new(size_t size)                              \
    {                                                                   \
        return malloc(size);                                            \
    }                                                                   \
    static void operator delete(void* p)                                \
    {                                                                   \
        free(p);                                                        \
    }                                                                   \
protected:                                                              \
    static void* operator new [](size_t) throw() { return NULL; }       \
    static void operator delete [](void*) throw() {}                    \
    static void* operator new(size_t, void*) throw() { return NULL; }   \
    static void operator delete(void*, void*) throw() {}                \
    static void* operator new [](size_t, void*) throw() { return NULL; }\
    static void operator delete [](void*, void*) throw() {}
#endif


namespace BCLib
{
namespace Utility
{
class BCLIB_UTILITY_API CMemPool
{
public:
    CMemPool(BCLib::uint32 size);
    ~CMemPool();

    BCLib::uint32 getMemSize() const;
    char* allocMem();
    void freeMem(void* p);

protected:
    BCLib::uint32 m_size;
    BCLib::Utility::CMutex m_mutex;
    BCLib::Utility::CUnitPool m_unitPool;
};

class BCLIB_UTILITY_API CMemPoolMgr
{
    BCLIB_SINGLETON_DECLARE(CMemPoolMgr);
public:
    char* allocMem(BCLib::uint32 size);
    void freeMem(void* p);

protected:
    CMemPoolMgr();
    ~CMemPoolMgr();

    CMemPool* _findAndCreateMemPool(BCLib::uint32 size);
    CMemPool* _findMemPool(BCLib::uint32 size);

protected:
    BCLib::Utility::CMutex m_mutex;
    BCLib::uint32 m_memPoolCnt;
    CMemPool** m_ppMemPools;
};
}//Utility
}//BCLib
#endif//__BCLIB_UTILITY_MEMORY_MEMPOOLMGR_H__
