//////////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   CLLib\Utility\managerBufferHelper.h
//    author:     League of Perfect
///   @brief 
///
//////////////////////////////////////////////////////////////////////////
#pragma once
#include <msclr/lock.h>
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Threading;
using namespace msclr;

namespace CLLib
{
    namespace Utility
    {
        public ref class CByteArray
        {
        public:
            CByteArray(unsigned int size)
            {
                m_byteArray = nullptr;
                try
                {
                    m_byteArray = gcnew array<System::Byte>(size);
                    m_size = size;
                }
                catch (System::Exception^ e)
                {
                    throw gcnew System::Exception(e->Message);
                }
            }

            array<System::Byte>^ GetByteArray() 
            { 
                return m_byteArray; 
            }

            unsigned int GetSize() 
            { 
                return m_size; 
            }

            void Reset()
            {
                if (m_byteArray!=nullptr)
                {
                    m_byteArray->Initialize();
                }
            }

        private:
            array<System::Byte>^ m_byteArray;
            unsigned int m_size;

        };

        public ref class CManagerBufferHelper
        {
        public:

            static CManagerBufferHelper^ singleton()
            {
                return %m_Instance;
            }

            CManagerBufferHelper():m_beginningPoolSize(0),m_maxByteArraySize(0)
            {
                m_MemoryPoolEvent = gcnew AutoResetEvent(true);
                m_byteArrayList = gcnew System::Collections::Generic::LinkedList<CByteArray^>();
            }

            void Init(int maxConcurrentThreads,unsigned int maxSize)
            {
                if (m_maxByteArraySize!=0)
                {
                    return;
                }
                m_beginningPoolSize = 0;
                m_maxByteArraySize = maxSize;

                if (maxConcurrentThreads<1)
                {
                    throw gcnew System::Exception("maxConcurrentThreads is less than 1");
                }
                try
                {
                    for (int i=0;i<maxConcurrentThreads;++i)
                    {
                        m_byteArrayList->AddFirst(gcnew CByteArray(maxSize));
                    }
                    m_beginningPoolSize = maxConcurrentThreads;
                }
                catch (System::Exception^ e)
                {
                    throw gcnew System::Exception(e->Message);
                }
            }

            void Release()
            {
                m_byteArrayList->Clear();
            }

            CByteArray^ GetMemory(unsigned int memorySize)
            {
                CByteArray^ byteArray = nullptr;
                m_MemoryPoolEvent->WaitOne();
                if (m_byteArrayList->Count > 0)
                {
                    if (memorySize<=m_maxByteArraySize)
                    {
                        byteArray = m_byteArrayList->First->Value;
                        m_byteArrayList->RemoveFirst();
                    }
                    else
                    {
                        byteArray =  m_byteArrayList->Last->Value;
                        if (byteArray->GetSize()>memorySize)
                        {
                            System::Collections::Generic::LinkedListNode<CByteArray^>^ currentNode = m_byteArrayList->Last;
                            System::Collections::Generic::LinkedListNode<CByteArray^>^ preNode =  currentNode->Previous;                        
                            CByteArray^ preByteArray = preNode==nullptr?nullptr:preNode->Value;

                            while (preByteArray!=nullptr && preByteArray->GetSize()>memorySize)
                            {
                                byteArray = preByteArray;
                                currentNode = preNode;
                                preNode = preNode->Previous;
                                preByteArray = preNode==nullptr?nullptr:preNode->Value;
                            }
                            m_byteArrayList->Remove(currentNode);
                        }
                        else
                        {
                            byteArray = gcnew CByteArray(memorySize);
                        }
                    }                    
                }
                else
                    byteArray = gcnew CByteArray(memorySize);
                m_MemoryPoolEvent->Set();
                return byteArray;
            }

            void FreeMemory(CByteArray^ byteArray)
            {
                m_MemoryPoolEvent->WaitOne();
                if (byteArray->GetSize()==m_maxByteArraySize||m_byteArrayList->Count==0)
                {
                    m_byteArrayList->AddFirst(byteArray);
                }
                else
                {
                    System::Collections::Generic::LinkedListNode<CByteArray^>^ currentNode = m_byteArrayList->Last;
                    CByteArray^ currentByteArray = currentNode->Value;
                    while (currentByteArray!=nullptr&&currentByteArray->GetSize()>byteArray->GetSize())
                    {
                        if(currentNode->Previous!=nullptr)
                        {
                            currentNode = currentNode->Previous;
                            currentByteArray = currentNode==nullptr?nullptr:currentNode->Value;
                        }

                    }
                    m_byteArrayList->AddAfter(currentNode,byteArray);
                }
                m_MemoryPoolEvent->Set();
            }

            CByteArray^ ConvertNavtiveByteArray(const char* msgBuff, unsigned int buffSize, array<System::Byte>^& managedBuffer)
            {
                CByteArray^ byteArray = GetMemory(buffSize);
                if (byteArray!=nullptr)
                {
                    managedBuffer = byteArray->GetByteArray();
                    for (System::UInt32 i = 0; i< buffSize ; ++i)
                    {
                        managedBuffer[i] = msgBuff[i];
                    }
                }            
                return byteArray;
            }

        private:
            static CManagerBufferHelper m_Instance;
            System::Collections::Generic::LinkedList<CByteArray^>^ m_byteArrayList;
            int m_beginningPoolSize;
            unsigned int m_maxByteArraySize;
            AutoResetEvent^ m_MemoryPoolEvent;
        };
    }
}