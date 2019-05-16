//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/io/xmlExcel/main.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string>
#include <BCLib/utility/string.h>
#include <BCLib/utility/io/xmlExcel.h>

#define COLUMN_MAX 10
#define ROW_MAX 20

void readExcelFile(BCLib::Utility::CXmlExcel& xmlExcel)
{
    char columnName[][10] =
    {
        "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9"
    };

    xmlExcel["Test"].appendRow();
    int lastNum = xmlExcel["Test"].countRows() - 1;
    for(int i = 0; i < COLUMN_MAX; ++i)
    {
        BCLib::Utility::CStringA str;
        str.format("R%dC%d", lastNum, i);
        xmlExcel["Test"][lastNum][i] = str;
    }

    for(int j = 0; j < xmlExcel["Test"].countRows(); ++j)
    {
        for(int i = 0; i < xmlExcel["Test"].countColumns(); ++i)
        {
            printf("%s\t", xmlExcel["Test"][j][columnName[i]].getString().c_str());
        }
        printf("\n");
    }
}

void writeExcelFile(BCLib::Utility::CXmlExcel& xmlExcel)
{
    char columnName[][10] =
    {
        "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9"
    };
    for(int i = 0; i < COLUMN_MAX; ++i)
    {
        xmlExcel["Test"].setColumnName(i, columnName[i]);
    }
    for(int j = 0; j < ROW_MAX; ++j)
    {
        xmlExcel["Test"].appendRow();
        for(int i = 0; i < COLUMN_MAX; ++i)
        {
            BCLib::Utility::CStringA str;
            str.format("R%dC%d", j, i);
            xmlExcel["Test"][j][i] = str;
        }
    }
}

int main(int argc, char* argv[])
{
    BCLib::Utility::CXmlExcel xmlExcel;
    //std::string fileName = "test.xml";
    std::string fileName = "PerformSkillTable.xml";
    if(!xmlExcel.loadFile(fileName))
    {
        printf("读取文件[%s]失败\n", fileName.c_str());
        if(!xmlExcel.createFile(fileName, "Test", 10))
        {
            printf("创建文件[%s]失败\n", fileName.c_str());
            return 0;
        }
        printf("创建文件[%s]成功\n", fileName.c_str());

        //writeExcelFile(xmlExcel);
    }
    //readExcelFile(xmlExcel);

    if(!xmlExcel.save())
    {
        printf("保存文件[%s]失败\n", fileName.c_str());
    }
    else
    {
        printf("保存文件[%s]成功\n", fileName.c_str());
    }
    return 0;
}
/*
#include <BCLib/utility/noncopyable.h>
class TestA : public BCLib::Utility::CNoncopyable
{
public:
    TestA(int a)
    {
        m_a = new int;
        *m_a = a;
        printf("构造TestA\n");
    }

    ~TestA()
    {
        if (m_a) {
            delete m_a;
            m_a = NULL;
        }
        printf("释放TestA\n");
    }

    void show() const
    {
        printf("A=[%d]\n", *m_a);
    }

private:
    int* m_a;
};

class TestB
{
public:
    TestB()
    {
        m_testa = new TestA(5);
    }

    ~TestB()
    {
        if (m_testa != NULL) {
            delete m_testa;
            m_testa = NULL;
        }
    }

    TestA* get()
    {
        return m_testa;
    }

    const TestA& operator[](const std::string& tableName)
    {
        return *m_testa;
    }

private:
    TestA* m_testa;
};

void testFun(TestB& testb)
{
    TestA& tesa = (TestA&)testb[""];
    tesa.show();
}

int main(int argc, char* argv[])
{
    TestB testb;
    testFun(testb);
    testb[""].show();
}*/