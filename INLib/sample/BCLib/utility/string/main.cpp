//////////////////////////////////////////////////////////////////////
//  created:    2012/08/01
//  filename:   BCLib/utility/string/main.cpp
//  author:     aaron
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <BCLib/utility/string.h>

int main(int argc, char* argv[])
{
    BCLib::Utility::CStringA str("How  do  you  do?");
    printf("%s\n", str.c_str());
    str.toLower();
    printf("%s\n", str.c_str());
    str.toUpper();
    printf("%s\n", str.c_str());

    if(str.compareNoCase("How  do  you  do?"))
    {
        printf("compare ok\n");
    }
    str.toLower();
    str.replace("how", "How");
    printf("%s\n", str.c_str());

    BCLib::Utility::CStringA str1 = str.left(3);
    BCLib::Utility::CStringA str2 = str.right(3);
    BCLib::Utility::CStringA str3 = str.mid(9, 3);
    printf("str1=[%s]\n", str1.c_str());
    printf("str2=[%s]\n", str2.c_str());
    printf("str3=[%s]\n", str3.c_str());

    str.trim('?');
    std::list<BCLib::Utility::CStringA> strList;
    str.split(strList, ' ');
    int i = 0;
    for(std::list<BCLib::Utility::CStringA>::iterator it = strList.begin(); it != strList.end(); ++it)
    {
        printf("str[%d]=[%s]\n", i + 1, it->c_str());
        ++i;
    }

    BCLib::Utility::CStringA strNum1(9);
    BCLib::Utility::CStringA strNum2(9.95);

    printf("strNum1=[%s]\n", strNum1.c_str());
    printf("strNum2=[%s]\n", strNum2.c_str());

    int num1 = 0;
    double num2 = 0.0;

    strNum1 >> num1;
    strNum2 >> num2;

    printf("num1=[%d]\n", num1);
    printf("num2=[%g]\n", num2);


    BCLib::Utility::CStringA strFormat;
    strFormat.format(3, "%d + %d = %d", 2, 5, 2 + 5);
    printf("strFormat=[%s]\n", strFormat.c_str());
    BCLib::Utility::CStringW strw = strFormat.toWideString();
    wprintf(L"%s\n", strw.c_str());

    return 0;
}