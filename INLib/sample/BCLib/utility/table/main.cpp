//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/table/main.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <BCLib/utility/string.h>
#include <BCLib/utility/table.h>

#define COLUMN_MAX 10
#define ROW_MAX 20

/*
class Test
{
public:
    Test();
    int operator ()(int i, int j)
    {
         return 0;
    }
};
*/
int main(int argc, char* argv[])
{
    char columnName[][10] =
    {
        "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9"
    };
    BCLib::Utility::CTable<std::string> m_table(10);
    for(int i = 0; i < COLUMN_MAX; ++i)
    {
        m_table.setColumnName(i, columnName[i]);
    }
    for(int j = 0; j < ROW_MAX; ++j)
    {
        m_table.appendRow();
        for(int i = 0; i < COLUMN_MAX; ++i)
        {
            BCLib::Utility::CStringA str;
            str.format("R%dC%d", j, i);
            m_table[j][i] = str;
        }
    }
    int m = 5, n = 5;
    printf("Table[%d][%d] = %s\n", m, n, m_table[m]["A5"].getValue().c_str());

    return 0;
}