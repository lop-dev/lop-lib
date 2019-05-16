//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/io/binFile/main.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include <BCLib/utility/io/binFile.h>
#include <BCLib/utility/io/fileInfo.h>

//////////////////////////////////////////////////////////////////////////

struct SData
{
public:
    SData(int x, int y, int z)
    :m_x(x)
    ,m_y(y)
    ,m_z(z)
    {
    }
public:
    int m_x;
    int m_y;
    int m_z;
    void show()
    {
        printf("(%d, %d, %d)\n", m_x, m_y, m_z);
    }
};

//////////////////////////////////////////////////////////////////////////
class CData
{
public:
    CData(int num, char* data)
    {
        m_num = num;
        m_data = data;
    }

    ~CData()
    {
    }

    void show()
    {
        printf("%d, %s\n", m_num, m_data.c_str());
    }

    void readFromBinFile(BCLib::Utility::CBinFile& binFile)
    {
        binFile >> m_num;
        binFile >> m_data;;
    }

    void writeToBinFile(BCLib::Utility::CBinFile& binFile) const
    {
        binFile << m_num;
        binFile << m_data;
    }

private:
    int m_num;
    std::string m_data;
};

const BCLib::Utility::CBinFile& operator >>(BCLib::Utility::CBinFile& binFile, CData& testString)
{
    testString.readFromBinFile(binFile);
    return binFile;
}

BCLib::Utility::CBinFile& operator <<(BCLib::Utility::CBinFile& binFile, const CData& testString)
{
    testString.writeToBinFile(binFile);
    return binFile;
}

//////////////////////////////////////////////////////////////////////////

void writeFile()
{
    std::string file = "./testFile";
    BCLib::Utility::CBinFile binFile;
    if(!binFile.create(file))
    {
        if(!binFile.open(file))
        {
            printf("打开文件[%s]失败\n", file.c_str());
            return ;
        }
    }
    binFile << 1;
    binFile << 2.1;
    binFile << 'a';
    binFile << "haha";
    binFile << std::string("hello");
    binFile << CData(100, "welcome to here");
    binFile << SData(3, 4, 5);
    binFile.close();
}

void readFile()
{
    std::string file = "./testFile";
    BCLib::Utility::CBinFile binFile;
    if(!binFile.open(file))
    {
        printf("打开文件[%s]失败\n", file.c_str());
        return ;
    }

    int a1;
    double b1;
    char c1;
    char d1[5];
    std::string e1;
    CData f1(0, "");
    SData g1(0, 0, 0);

    binFile >> a1;
    binFile >> b1;
    binFile >> c1;
    binFile.read(d1, sizeof(d1));
    binFile >> e1;
    binFile >> f1;
    //printf("-------%d--------\n", binFile.tellCurrent());
    binFile >> g1;

    printf("%d\n", a1);
    printf("%f\n", b1);
    printf("%c\n", c1);
    printf("%s\n", d1);
    printf("%s\n", e1.c_str());
    f1.show();
    g1.show();

    printf("-------%d--------\n", binFile.tellCurrent());

    binFile.seekEnd(-sizeof(SData));
    //printf("-------%d--------\n", binFile.tellCurrent());
    SData f2(1, 1, 1);
    binFile >> f2;
    f2.show();

    binFile.seekHead(sizeof(int) + sizeof(double));
    char c2;
    binFile >> c2;
    printf("%c\n", c2);

    binFile.seekCurrent(5);
    std::string e2;
    binFile >> e2;
    printf("%s\n", e2.c_str());

    //     if (binFile.seekCurrent(100)) {
    //         binFile << "OK";
    //     }

    if(binFile.isEnd())
    {
        printf("文件结束\n");
    }

    binFile.close();
}

int main(int argc, char* argv[])
{
    writeFile();
    readFile();

    return 0;
}