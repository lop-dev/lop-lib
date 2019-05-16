//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/thread/main.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <BCLib/utility/thread/thread.h>


BCLib::ResThread testFun(void* param)
{
    int a = (int)param;
    printf("testFun %d\n", a);
    return BCLIB_THREAD_RES(0);
}

class CTest
{
public:
    BCLib::ResThread testFun(void* param)
    {
        int a = (int)param;
        printf("CTest::testFun %d\n", a);
        return BCLIB_THREAD_RES(0);
    }
};

int main(int argc, char* argv[])
{
    CTest test;
    BCLib::Utility::CHndThread thd1 = BCLib::Utility::CHndThread::createThread(testFun, (void*)8);
    BCLib::Utility::CHndThread thd2 = BCLib::Utility::CHndThread::createThread(&CTest::testFun, &test, (void*)9);

    BCLib::Utility::CFunction<BCLib::ResThread, void*> fun1(testFun);
    BCLib::Utility::CHndThread thd3 = BCLib::Utility::CHndThread::createThread(fun1, (void*)10);

    BCLib::Utility::CFunctionObject<BCLib::ResThread, CTest, void*> fun2(&CTest::testFun, &test);
    BCLib::Utility::CHndThread thd4 = BCLib::Utility::CHndThread::createThread(fun2, (void*)11);

    thd1.join();
    thd2.join();
    thd3.join();
    thd4.join();

    return 0;
}