//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/Allocator.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_ALLOCATOR_H__
#define __GELIB_UTILITY_ALLOCATOR_H__

#include <vector>
#include <cstddef>

namespace GELib
{


class CChunkAllocator
{
public:

    CChunkAllocator(int chunkSize=2048);
    virtual ~CChunkAllocator();

    void SetChunkSize(int chunkSize);     // set chunk size before any Alloc()

    class ChunkHeader
    {
    public:
        char *m_pNext;
        int m_iChunkSize;
    };

    void* Alloc(int size);
    void FreeAll();
    void Purge();

private:

    ChunkHeader* Header(char *chunk)
    {
        return (ChunkHeader*)chunk;
    }

    int m_iChunkSize;
    int m_iFreeSize;
	int m_iTotalSize;
	int m_iTotalChunkSize;
    char *m_pChunkList;
    char *m_pFreeChunkList;
};


template<typename T> class CObjectPool
{
public:
    CObjectPool() :
        m_Allocator(sizeof(T) * 256)
    {
        m_pFreeList = 0;
    }

    virtual ~CObjectPool()
    {
        Uninitialize();
    }

    void Initialize(int chunkSize)
    {
        m_Allocator.SetChunkSize(sizeof(T) * chunkSize);
    }

	void Uninitialize()
    {
        // destructor are not called
        m_Allocator.Purge();
        m_pFreeList = 0;
    }

	T* Alloc()
    {
        if (m_pFreeList)
        {
            T *p = m_pFreeList;
            m_pFreeList = Next(m_pFreeList);
            return p;
        }
        else
        {
            return (T*)m_Allocator.Alloc(sizeof(T));
        }
    }

    void Free(T *p)
    {
        Next(p) = m_pFreeList;
        m_pFreeList = p;
    }

    void FreeAll()
    {
        m_Allocator.FreeAll();
        m_pFreeList = 0;
    }

    T* New()
    {
        void *p = Alloc();
        if (p == 0)
            return 0;
        return new(p) T;
    };

    void Delete(T *p)
    {
        if (p)
        {
            p->~T();
            Free(p);
        }
    }

private:

    T*& Next(T *p)
    {
        return *((T**)p);
    }

    CChunkAllocator m_Allocator;
    T *m_pFreeList;
};

template<typename T> class CChunkObjectAllocator
{
public:
    CChunkObjectAllocator()
    {
        m_iChunkSize = 256;
        m_iFreeIndex = 0;
        m_iFreeChunkIndex = 0;
    }

	virtual ~CChunkObjectAllocator()
    {
        Uninitialize();
    }

	void Initialize(int chunkSize)
    {
        Uninitialize();

        m_iChunkSize = chunkSize;
        T *chunk = new T[m_iChunkSize];
        m_Pool.push_back(chunk);
        m_iFreeIndex = 0;
        m_iFreeChunkIndex = 0;
    }

	void Uninitialize()
    {
        size_t s = m_Pool.size();
        for (size_t i=0; i<s; ++i)
            delete[] m_Pool[i];
        m_Pool.clear();
        m_iFreeIndex = 0;
        m_iFreeChunkIndex = 0;
    }

	void Reset()
    {
        m_iFreeIndex = 0;
        m_iFreeChunkIndex = 0;
    }

    // warning! constructor is not called if there are free pool
	T* Alloc()
    {
        if (m_iFreeIndex >= m_iChunkSize)
        {
            m_iFreeIndex = 0;
            ++m_iFreeChunkIndex;
            if (m_iFreeChunkIndex >= (int)m_Pool.size())
            {
                T *chunk = new T[m_iChunkSize];
                m_Pool.push_back(chunk);
            }
        }

        return m_Pool[m_iFreeChunkIndex] + m_iFreeIndex++;
    }

    T* New()
    {
        return new(Alloc()) T;
    }

private:
    std::vector<T*> m_Pool;
    int m_iChunkSize;
	int m_iFreeIndex;
	int m_iFreeChunkIndex;
};



}//GELib

#endif//__GELIB_UTILITY_ALLOCATOR_H__
