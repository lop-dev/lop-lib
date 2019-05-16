//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/function/main.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <BCLib/utility/logFile.h>
#include <BCLib/utility/function.h>
#include <BCLib/utility/functionList.h>

void testFun1()
{
    printf("testFun1\n");
}

int testFun2()
{
    printf("testFun2\n");
    return 0;
}

int testFun3(int a)
{
    printf("testFun3 a=[%d]\n", a);
    return a;
}

class CTest
{
public:
    int a;
    CTest()
    :a(0)
    {
    }

    void show()
    {
        printf("CTest::show %d\n", a);
    }

    int show(int b)
    {
        printf("CTest::show %d\n", a + b);
        return a + b;
    }
};

void testFun4(CTest& test)
{
    printf("testFun4 : ");
    test.show();
}

int main(int argc, char* argv[])
{
    CTest test;
    test.a = 12;

    printf("------------fun1-----------\n");
    BCLib::Utility::CFunction<void> fun1(testFun1);
    fun1();

    printf("\n------------fun2--------------\n");
    BCLib::Utility::CFunction<int> fun2(testFun2);
    printf("fun2=[%d]\n", fun2());

    printf("\n------------fun3--------------\n");
    BCLib::Utility::CFunction<int, int> fun3(testFun3);
    printf("fun3=[%d]\n", fun3(2));

    printf("\n------------fun4--------------\n");
    BCLib::Utility::CFunction<void, CTest&> fun4(testFun4);
    fun4(test);

    printf("\n------------funObj1-----------\n");
    BCLib::Utility::CFunctionObject<void, CTest> funObj1(&CTest::show, &test);
    funObj1();

    printf("\n------------funObj2-----------\n");
    BCLib::Utility::CFunctionObject<int, CTest, int> funObj2(&CTest::show, &test);
    printf("funObj2=[%d]\n", funObj2(4));

    printf("\n------------pfun1-------------\n");
    BCLib::Utility::CFunction<void>* pfun1 = &funObj1;
    (*pfun1)();

    printf("\n------------pfun2-------------\n");
    BCLib::Utility::CFunction<int, int>* pfun2 = &funObj2;
    printf("pfun2=[%d]\n", (*pfun2)(4));

    printf("\n------------funList1----------\n");
    BCLib::Utility::CFunctionList<void> funList1;
    funList1.addFunction(&fun1);
    funList1.addFunction(&funObj1);
    funList1();

    printf("\n------------funList2----------\n");
    BCLib::Utility::CFunctionList<int, int> funList2;
    funList2.addFunction(&fun3);
    funList2.addFunction(&funObj2);
    printf("funList2=[%d]\n", funList2(4));

    printf("\n------------funList1----------\n");
    funList1.delFunction(&fun1);
    funList1();

    printf("\n------------funList2----------\n");
    funList2.delFunction(&funObj2);
    printf("funList2=[%d]\n", funList2(4));
    return 0;
}