//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/objectPool/main.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/objectPool.h>
#include <BCLib/utility/dateTime.h>

#define TEST_MAX 10000
int g_createCount = 0;
int g_freeCount = 0;
class CTest
{
public:
    CTest()
    :m_a(0)
    ,m_b(0)
    ,m_c(0)
    {
        g_createCount++;
    }
    CTest(int a)
    :m_a(a)
    ,m_b(0)
    ,m_c(0)
    {
        g_createCount++;
    }
    CTest(int a, int b)
    :m_a(a)
    ,m_b(b)
    ,m_c(0)
    {
        g_createCount++;
    }
    CTest(int a, int b, int c)
    :m_a(a)
    ,m_b(b)
    ,m_c(c)
    {
        g_createCount++;
    }

    void show()
    {
        printf("%d, %d, %d\n", m_a, m_b, m_c);
    }

    ~CTest()
    {
        g_freeCount ++;
    }

private:
    int m_a;
    int m_b;
    int m_c;
};

int main(int argc, char* argv[])
{
    int tesNum[10] =
    {
        TEST_MAX / 10, TEST_MAX / 9, TEST_MAX / 8, TEST_MAX / 7, TEST_MAX / 6, TEST_MAX / 5, TEST_MAX / 4, TEST_MAX / 3, TEST_MAX / 2, TEST_MAX
    };
    CTest* testList[TEST_MAX] =
    {
        NULL
    };

    BCLib::Utility::CSurveyTimer surveyTimer;
    surveyTimer.reset();
    for(int j = 0; j < 10; j++)
    {
        for(int i = 0; i < tesNum[j]; ++i)
        {
            testList[i] = new CTest(1, 2, 3);
        }
        for(int i = 0; i < tesNum[j]; ++i)
        {
            BCLIB_SAFE_DELETE(testList[i]);
        }
    }
    printf("%I64d\t%d\t%d\n", surveyTimer.microseconds(), g_createCount, g_freeCount);

    BCLib::Utility::CObjectPool<CTest> objectPool;

    surveyTimer.reset();
    for(int j = 0; j < 10; j++)
    {
        for(int i = 0; i < tesNum[j]; ++i)
        {
            testList[i] = objectPool.construct(1, 2, 3);
        }
        for(int i = 0; i < tesNum[j]; ++i)
        {
            objectPool.destroy(testList[i]);
        }
    }

    printf("%I64d\t%d\t%d\n", surveyTimer.microseconds(), g_createCount, g_freeCount);

    char buff[100];
    surveyTimer.reset();

    for(int j = 0; j < 10; j++)
    {
        for(int i = 0; i < tesNum[j]; ++i)
        {
            testList[i] = new (buff)CTest(1, 2, 3);
        }
        for(int i = 0; i < tesNum[j]; ++i)
        {
            testList[i]->~CTest();
        }
    }
    printf("%I64d\t%d\t%d\n", surveyTimer.microseconds(), g_createCount, g_freeCount);

    return 0;
}
