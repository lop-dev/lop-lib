//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/memory/shmMemory/client.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/memory/shareMemory.h>
#include <BCLib/utility/thread/thread.h>
#include <iostream>

const int key = 100;
int main(int agrc, char* argv[])
{
    BCLib::Utility::CShareMemory shareMemory;

    int* buf = NULL;
    const int bufLen = 2000;
    BCLib::Utility::EShareMemoryError errorCode = shareMemory.create(key, bufLen * sizeof(int), (void**)&buf);
    if(errorCode == BCLib::Utility::EShareMemoryError::ESME_SUCCESS || errorCode == BCLib::Utility::EShareMemoryError::ESME_SUCCESS_EXISTS)
    {
        //
        for(int i = 0; i < bufLen; i++)
        {
            std::cout << *(buf + i) << " ";
            if(0 == (i % 10))
            {
                std::cout << std::endl;
            }
        }
    }
    else
    {
        std::cout << "create share memory failed" << std::endl;
    }
    BCLib::Utility::CThread::sleep(10);
    std::cout << *buf << std::endl;
    system("puase");
    return 0;
}