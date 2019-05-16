//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/memory/shmMemory/server.cpp
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
    BCLib::Utility::CShareMemory oShareMemory;

    int* buf = NULL;
    const int bufLen = 2000;
    BCLib::Utility::EShareMemoryError eErrorCode = oShareMemory.create(key, bufLen * sizeof(int), (void**)&buf);
    if(eErrorCode == BCLib::Utility::EShareMemoryError::ESME_SUCCESS || eErrorCode == BCLib::Utility::EShareMemoryError::ESME_SUCCESS_EXISTS)
    {
        //
        for(int i = 0; i < bufLen; i++)
        {
            *(buf + i) = i;
        }
    }
    else
    {
        std::cout << "create share memory failed" << std::endl;
    }

    oShareMemory.close();

    BCLib::Utility::CThread::sleep(1);
    system("puase");
    return 0;
}