//////////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   CLLib\Utility\nativeBufferHelper.h
//    author:     League of Perfect
///   @brief 
///
//////////////////////////////////////////////////////////////////////////
#ifndef __CLLIB_UTILITY_NATIVEBUFFERHELPER_H__
#define __CLLIB_UTILITY_NATIVEBUFFERHELPER_H__

#include <CLLib/Utility/memoryPool.h>
using namespace System::Threading;

namespace CLLib
{
    namespace Utility
    {
        public ref class CNativeBufferHelper
        {
        public:
            static CNativeBufferHelper^ singleton()
            {
                return %m_Instance;
            }

            CNativeBufferHelper() :m_NativeMemoryPool(NULL), m_RefCount(0)
            {
                m_MemoryPoolEvent = gcnew AutoResetEvent(true);
            }

            void Init()
            {
                m_RefCount++;
                if (m_NativeMemoryPool == NULL)
                {
                    m_NativeMemoryPool = new CMemoryPool(65536);
                }
            }

            void Release()
            {
                m_RefCount--;
                if (m_RefCount == 0 && m_NativeMemoryPool != NULL)
                {
                    delete m_NativeMemoryPool;
                    m_NativeMemoryPool = NULL;
                }
            }

            void* GetMemory(unsigned int memorySize)
            {
                if (m_NativeMemoryPool != NULL)
                {
                    return m_NativeMemoryPool->getMemory(memorySize);
                }
                return NULL;
            }

            void FreeMemory(void* ptrMem, unsigned int memorySize)
            {
                if (m_NativeMemoryPool != NULL)
                {
                    m_NativeMemoryPool->freeMemory(ptrMem, memorySize);
                }
            }

            void* ConvertManageByteArray(array<System::Byte>^ msgBuff, System::UInt32 buffSize)
            {
                m_MemoryPoolEvent->WaitOne();
                char* nativeBuffer = (char*)GetMemory(buffSize);
                m_MemoryPoolEvent->Set();
                if (nativeBuffer != NULL)
                {
                    for (System::UInt32 i = 0; i < buffSize; ++i)
                    {
                        nativeBuffer[i] = msgBuff[i];
                    }
                }
                return nativeBuffer;
            }

            void ReleaseNativeByteArray(void* nativeBuffer, System::UInt32 buffSize)
            {
                FreeMemory(nativeBuffer, buffSize);
            }

        private:
            CMemoryPool * m_NativeMemoryPool;
            static CNativeBufferHelper m_Instance;
            int m_RefCount;
            AutoResetEvent^ m_MemoryPoolEvent;
        };
    }//Utility
}//CLLib

#endif//__CLLIB_UTILITY_NATIVEBUFFERHELPER_H__