//////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   CLLib\Utility\memoryPool.h
//    author:     League of Perfect
///   @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __CLLIB_UTILITY_MEMORYPOOL_H__
#define __CLLIB_UTILITY_MEMORYPOOL_H__

#include <iostream>

namespace CLLib
{
    namespace Utility
    {

        typedef unsigned char TByte;

        static const std::size_t DEFAULT_MEMORY_POOL_SIZE = 1024;
        static const std::size_t DEFAULT_MEMORY_CHUNK_SIZE = 128;
        static const std::size_t DEFAULT_MEMORY_SIZE_TO_ALLOCATE = DEFAULT_MEMORY_CHUNK_SIZE * 2;

        class IMemoryBlock
        {
        public:
            virtual ~IMemoryBlock() {}

            virtual    void* getMemory(const std::size_t &memoryBlockSzie) = 0;
            virtual void freeMemory(void *ptrMemoryBlock, const std::size_t &memoryBlockSzie) = 0;
        };

        typedef struct MemoryChunk
        {
            TByte* data;
            std::size_t dataSize;
            std::size_t usedSize;
            bool isAllocationChunk;
            MemoryChunk *next;
        } MemoryChunk;

        class CMemoryPool : public IMemoryBlock
        {
        public:
            CMemoryPool(const std::size_t &initialMemoryPoolSzie = DEFAULT_MEMORY_POOL_SIZE,
                const std::size_t &memoryChunkSize = DEFAULT_MEMORY_CHUNK_SIZE,
                const std::size_t &mininalMemorySizeToAllocate = DEFAULT_MEMORY_SIZE_TO_ALLOCATE,
                bool setMemoryData = false);
            virtual ~CMemoryPool();

            virtual    void* getMemory(const std::size_t &memoryBlockSzie);
            virtual void freeMemory(void *ptrMemoryBlock, const std::size_t &memoryBlockSzie);

            bool isValidPointer(void *ptrMemoryBlock);

        private:
            bool allocateMemory(const std::size_t &memorySzie);
            void freeAllAllocateMemroy();

            unsigned int calculateNeededChunks(const std::size_t &memorySzie);
            std::size_t calculateBestMemoryBlockSize(const std::size_t &requestMemoryBlockSzie);

            MemoryChunk* findChunkSuitableToHoldMemory(const std::size_t &memorySzie);
            MemoryChunk* findChunkHoldingPointerTo(void *ptrMemoryBlock);

            MemoryChunk* skipChunks(MemoryChunk *ptrStartChunk, unsigned int chunksToSkip);
            MemoryChunk* setChunkDefaults(MemoryChunk *ptrChunk);

            void freeChunks(MemoryChunk *ptrChunk);
            void deallocateAllChunks();

            bool linkChunksToData(MemoryChunk *ptrNewChunks, unsigned int chunkCount, TByte* ptrNewMemBlock);
            void setMemoryChunkValues(MemoryChunk *ptrChunk, const std::size_t memoryBlockSize);
            bool recalcChunkMemorySize(MemoryChunk *ptrChunk, unsigned int chunkCount);

            std::size_t maxValue(const std::size_t &valueA, const std::size_t &valueB) const;

        private:
            MemoryChunk *m_ptrFirstChunk;
            MemoryChunk *m_ptrLastChunk;
            MemoryChunk *m_ptrCursorChunk;

            std::size_t m_totalMemoryPoolSize;
            std::size_t m_usedMemoryPoolSize;
            std::size_t m_freeMemoryPoolSize;

            std::size_t m_memoryChunkSize;
            unsigned int m_memoryChunkCount;
            unsigned int m_objectCount;

            bool m_setMemoryData;
            std::size_t m_minimalMemorySizeToAllocate;
        };
    }//Utility
}//CLLib

#endif//__CLLIB_UTILITY_MEMORYPOOL_H__