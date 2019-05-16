//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/memory/memMallocMgr.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_MEMORY_MEMMALLOCMGR_H__
#define __BCLIB_UTILITY_MEMORY_MEMMALLOCMGR_H__

#include <BCLib/utility/singleton.h>
#include <BCLib/utility/thread/mutex.h>

namespace BCLib
{
namespace Utility
{
class BCLIB_UTILITY_API CMemMallocMgr
{
    BCLIB_SINGLETON_DECLARE(CMemMallocMgr);
public:
    static const BCLib::uint32 DEF_MEM_GRANULARITY_SIZE = 1024*1024;
    //! 大块内存的颗粒度,必须为2的幂,最小为64k
    static void setMemGranularity(BCLib::uint32 memGranularity = DEF_MEM_GRANULARITY_SIZE);
    static BCLib::uint32 getMemGranularity();

    void* malloc(BCLib::uint32 size);
    void free(void* p);

private:
    CMemMallocMgr();
    ~CMemMallocMgr();

    void* _openMem(size_t size);
    void _closeMem(void* p);

private:
    BCLib::Utility::CMutex m_mutex;
    static BCLib::uint32 m_memGranularity;
};
}//Utility
}//BCLib
#endif//__BCLIB_UTILITY_MEMORY_MEMMALLOCMGR_H__
