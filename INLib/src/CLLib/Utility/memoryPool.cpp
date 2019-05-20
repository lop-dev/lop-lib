//////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   CLLib\Utility\memoryPool.cpp
//    author:     League of Perfect
///   @brief
///
//////////////////////////////////////////////////////////////////////
#include <CLLib/Utility/memoryPool.h>
#include <assert.h>
#include <math.h>

namespace CLLib
{
    namespace Utility
    {
        static const int FREEED_MEMORY_CONTENT = 0xAA;
        static const int NEW_ALLOCATED_MEMORY_CONTENT = 0xFF;

        CMemoryPool::CMemoryPool(const std::size_t &initialMemoryPoolSzie /*= DEFAULT_MEMORY_POOL_SIZE*/,
            const std::size_t &memoryChunkSize /*= DEFAULT_MEMORY_CHUNK_SIZE*/,
            const std::size_t &mininalMemorySizeToAllocate /*= DEFAULT_MEMORY_SIZE_TO_ALLOCATE*/,
            bool setMemoryData /*= false*/)
        {
            m_ptrFirstChunk = NULL;
            m_ptrLastChunk = NULL;
            m_ptrCursorChunk = NULL;

            m_totalMemoryPoolSize = 0;
            m_usedMemoryPoolSize = 0;
            m_freeMemoryPoolSize = 0;

            m_memoryChunkSize = memoryChunkSize;
            m_memoryChunkCount = 0;
            m_objectCount = 0;
            m_setMemoryData = setMemoryData;
            m_minimalMemorySizeToAllocate = mininalMemorySizeToAllocate;

            allocateMemory(initialMemoryPoolSzie);
        }

        CMemoryPool::~CMemoryPool()
        {
            freeAllAllocateMemroy();
            deallocateAllChunks();
            assert((m_objectCount == 0) && "WARNING: Memory Leak : Not all objects have been freed");
        }

        void* CMemoryPool::getMemory(const std::size_t &memoryBlockSzie)
        {
            std::size_t bestMemBlockSize = calculateBestMemoryBlockSize(memoryBlockSzie);
            MemoryChunk* ptrChunk = NULL;
            while (!ptrChunk)
            {
                ptrChunk = findChunkSuitableToHoldMemory(bestMemBlockSize);
                if (!ptrChunk)
                {
                    bestMemBlockSize = maxValue(bestMemBlockSize, calculateBestMemoryBlockSize(m_minimalMemorySizeToAllocate));
                    allocateMemory(bestMemBlockSize);
                }
            }

            m_usedMemoryPoolSize += bestMemBlockSize;
            m_freeMemoryPoolSize -= bestMemBlockSize;
            m_objectCount++;
            setMemoryChunkValues(ptrChunk, bestMemBlockSize);

            return ((void*)ptrChunk->data);
        }

        void CMemoryPool::freeMemory(void *ptrMemoryBlock, const std::size_t &memoryBlockSzie)
        {
            MemoryChunk *ptrChunk = findChunkHoldingPointerTo(ptrMemoryBlock);
            if (ptrChunk)
            {
                freeChunks(ptrChunk);
            }
            else
            {
                assert(false && "ERROR: Pointer not in memory pool");
            }
            assert((m_objectCount > 0) && "ERROR: Delete too many object");
            m_objectCount--;
        }

        bool CMemoryPool::isValidPointer(void *ptrMemoryBlock)
        {
            MemoryChunk *ptrChunk = m_ptrFirstChunk;
            while (ptrChunk)
            {
                if (ptrChunk->data == (TByte*)ptrMemoryBlock)
                {
                    return true;
                }
                ptrChunk = ptrChunk->next;
            }
            return false;
        }

        bool CMemoryPool::allocateMemory(const std::size_t &memorySzie)
        {
            unsigned int neededChunks = calculateNeededChunks(memorySzie);
            std::size_t bestMemBlockSize = calculateBestMemoryBlockSize(memorySzie);
            TByte* ptrNewMemBlock = (TByte*)malloc(bestMemBlockSize);
            MemoryChunk* ptrNewChunks = (MemoryChunk*)malloc(neededChunks * sizeof(MemoryChunk));
            assert(((ptrNewMemBlock) && ptrNewChunks) && "Error: not enough memory");

            m_totalMemoryPoolSize += bestMemBlockSize;
            m_freeMemoryPoolSize += bestMemBlockSize;
            m_memoryChunkCount += neededChunks;

            if (m_setMemoryData)
            {
                memset(((void *)ptrNewMemBlock), NEW_ALLOCATED_MEMORY_CONTENT, bestMemBlockSize);
            }
            return linkChunksToData(ptrNewChunks, neededChunks, ptrNewMemBlock);
        }

        void CMemoryPool::freeAllAllocateMemroy()
        {
            MemoryChunk *ptrChunk = m_ptrFirstChunk;
            while (ptrChunk)
            {
                if (ptrChunk->isAllocationChunk)
                {
                    free(((void *)(ptrChunk->data)));
                }
                ptrChunk = ptrChunk->next;
            }
        }

        unsigned int CMemoryPool::calculateNeededChunks(const std::size_t &memorySzie)
        {
            float f = (float)(((float)memorySzie) / ((float)m_memoryChunkSize));
            return ((unsigned int)ceil(f));
        }

        std::size_t CMemoryPool::calculateBestMemoryBlockSize(const std::size_t &requestMemoryBlockSzie)
        {
            unsigned int neededChunks = calculateNeededChunks(requestMemoryBlockSzie);
            return std::size_t((neededChunks*m_memoryChunkSize));
        }

        MemoryChunk* CMemoryPool::findChunkSuitableToHoldMemory(const std::size_t &memorySzie)
        {
            unsigned int chunkSToSkip = 0;
            MemoryChunk *ptrChunk = m_ptrCursorChunk;
            for (unsigned int i = 0; i < m_memoryChunkCount; ++i)
            {
                if (ptrChunk)
                {
                    if (ptrChunk == m_ptrLastChunk)
                    {
                        ptrChunk = m_ptrFirstChunk;
                    }
                    if (ptrChunk->dataSize >= memorySzie)
                    {
                        if (ptrChunk->usedSize == 0)
                        {
                            m_ptrCursorChunk = ptrChunk;
                            return ptrChunk;
                        }
                    }
                    chunkSToSkip = calculateNeededChunks(ptrChunk->usedSize);
                    if (chunkSToSkip == 0)
                    {
                        chunkSToSkip = 1;
                    }
                    ptrChunk = skipChunks(ptrChunk, chunkSToSkip);
                }
                else
                {
                    return NULL;
                }
            }
            return NULL;
        }

        MemoryChunk* CMemoryPool::findChunkHoldingPointerTo(void *ptrMemoryBlock)
        {
            MemoryChunk *ptrTempChunk = m_ptrFirstChunk;
            while (ptrTempChunk)
            {
                if (ptrTempChunk->data == ((TByte*)ptrMemoryBlock))
                {
                    break;
                }
                ptrTempChunk = ptrTempChunk->next;
            }
            return ptrTempChunk;
        }

        MemoryChunk* CMemoryPool::skipChunks(MemoryChunk *ptrStartChunk, unsigned int chunksToSkip)
        {
            MemoryChunk *ptrCurrentChunk = ptrStartChunk;
            for (unsigned int i = 0; i < chunksToSkip; ++i)
            {
                if (ptrCurrentChunk)
                {
                    ptrCurrentChunk = ptrCurrentChunk->next;
                }
                else
                {
                    assert(false && "Error : Chunk pointer is NULL");
                    break;
                }
            }
            return ptrCurrentChunk;
        }

        MemoryChunk* CMemoryPool::setChunkDefaults(MemoryChunk *ptrChunk)
        {
            if (ptrChunk)
            {
                ptrChunk->data = NULL;
                ptrChunk->dataSize = 0;
                ptrChunk->usedSize = 0;
                ptrChunk->isAllocationChunk = false;
                ptrChunk->next = NULL;
            }
            return ptrChunk;
        }

        void CMemoryPool::freeChunks(MemoryChunk *ptrChunk)
        {
            MemoryChunk * ptrCurrentChunk = ptrChunk;
            unsigned int chunkCount = calculateNeededChunks(ptrChunk->usedSize);
            for (unsigned int i = 0; i < chunkCount; ++i)
            {
                if (ptrCurrentChunk)
                {
                    if (m_setMemoryData)
                    {
                        memset(((void *)ptrCurrentChunk->dataSize), FREEED_MEMORY_CONTENT, m_memoryChunkSize);
                    }
                    ptrCurrentChunk->usedSize = 0;
                    m_usedMemoryPoolSize -= m_memoryChunkSize;
                    ptrCurrentChunk = ptrCurrentChunk->next;
                }

            }
        }

        bool CMemoryPool::linkChunksToData(MemoryChunk *ptrNewChunks, unsigned int chunkCount, TByte* ptrNewMemBlock)
        {
            MemoryChunk *ptrNewChunk = NULL;
            unsigned int memOffset = 0;
            bool allocationChunkAssigned = false;
            for (unsigned int i = 0; i < chunkCount; ++i)
            {
                if (!m_ptrFirstChunk)
                {
                    m_ptrFirstChunk = setChunkDefaults(&(ptrNewChunks[0]));
                    m_ptrLastChunk = m_ptrFirstChunk;
                    m_ptrCursorChunk = m_ptrFirstChunk;
                }
                else
                {
                    ptrNewChunk = setChunkDefaults(&(ptrNewChunks[i]));
                    m_ptrLastChunk->next = ptrNewChunk;
                    m_ptrLastChunk = ptrNewChunk;
                }
                memOffset = (i* ((unsigned int)m_memoryChunkSize));
                m_ptrLastChunk->data = &(ptrNewMemBlock[memOffset]);

                if (!allocationChunkAssigned)
                {
                    m_ptrLastChunk->isAllocationChunk = true;
                    allocationChunkAssigned = true;
                }
            }
            return recalcChunkMemorySize(m_ptrFirstChunk, m_memoryChunkSize);
        }

        void CMemoryPool::deallocateAllChunks()
        {
            MemoryChunk *ptrChunk = m_ptrFirstChunk;
            MemoryChunk *ptrChunkToDelete = NULL;
            while (ptrChunk)
            {
                if (ptrChunk->isAllocationChunk)
                {
                    if (ptrChunkToDelete)
                    {
                        free(((void*)ptrChunkToDelete));
                    }
                    ptrChunkToDelete = ptrChunk;
                }
                ptrChunk = ptrChunk->next;
            }
        }

        void CMemoryPool::setMemoryChunkValues(MemoryChunk *ptrChunk, const std::size_t memoryBlockSize)
        {
            if (ptrChunk)
            {
                ptrChunk->usedSize = memoryBlockSize;
            }
            else
            {
                assert(false && "Error: Invalid NULL Chunk Pointer");
            }
        }

        bool CMemoryPool::recalcChunkMemorySize(MemoryChunk *ptrChunk, unsigned int chunkCount)
        {
            unsigned int memOffset = 0;
            for (unsigned int i = 0; i < chunkCount; ++i)
            {
                if (ptrChunk)
                {
                    memOffset = (i*((unsigned int)m_memoryChunkSize));
                    ptrChunk->dataSize = (((unsigned int)m_totalMemoryPoolSize) - memOffset);
                    ptrChunk = ptrChunk->next;
                }
                else
                {
                    assert(false && "Error: Bad Memory Chunk Pointer");
                    return false;
                }
            }
            return true;
        }

        std::size_t CMemoryPool::maxValue(const std::size_t &valueA, const std::size_t &valueB) const
        {
            return valueA > valueB ? valueA : valueB;
        }
    }//Utility
}//CLLib