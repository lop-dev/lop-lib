//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/misc/main.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <MWLib/misc/stringFilter.h>
#include <BCLib/Utility/dateTime.h>


//! ≤‚ ‘∆¡±Œ◊÷–‘ƒ‹
int testCase_stringFilterPerformance();
//! √¸¡Ó––∑Ω Ω,≤‚ ‘∆¡±Œ◊÷
int testCase_stringFilterCommand();


int main(int argc, char* argv[])
{
    //return testCase_stringFilterPerformance();
    return testCase_stringFilterCommand();
}


int testCase_stringFilterPerformance()
{
    //////////////////////////////////////////////////////////////////////
    FILE* pkFile = NULL;
    fopen_s(&pkFile, "novel.txt", "rt");
    if(!pkFile)
    {
        return -1;
    }
    fseek(pkFile, 0, SEEK_END);
    size_t uiFileSize = ftell(pkFile);
    char* pkBuf = new char[uiFileSize + 1];
    fseek(pkFile, 0, SEEK_SET);
    fread(pkBuf, uiFileSize, 1, pkFile);
    fclose(pkFile);
    pkBuf[uiFileSize] = 0;
    std::string novel = pkBuf;
    delete [] pkBuf;

    //////////////////////////////////////////////////////////////////////
    std::wstring wNovel;
    MWLib::Misc::ANSI_To_UTF16(novel, wNovel);
    std::vector<std::string> strList;
    strList.reserve(102400);
    size_t idx = 0;
    size_t len = 20;
    wchar_t* novelPtr = (wchar_t*)(wNovel.c_str());
    std::string strUTF8;
    for(; idx < wNovel.length(); idx+=len)
    {
        size_t len1 = idx+len <= uiFileSize ? len : uiFileSize-idx;
        wchar_t c = novelPtr[idx+len1-1];
        novelPtr[idx+len1-1] = 0;
        MWLib::Misc::UTF16_To_UTF8(novelPtr+idx, strUTF8);
        strList.push_back(strUTF8);
        novelPtr[idx+len1-1] = c;
    }
    std::vector<std::string> outStrList = strList;

    //////////////////////////////////////////////////////////////////////
    MWLib::Misc::CMassStringFilter strFilter;
    pkFile = NULL;
    fopen_s(&pkFile, "screenWordList.csv", "rt");
    if(!pkFile)
    {
        return -1;
    }
    fseek(pkFile, 0, SEEK_END);
    uiFileSize = ftell(pkFile);
    pkBuf = new char[uiFileSize];
    fseek(pkFile, 0, SEEK_SET);
    fread(pkBuf, uiFileSize, 1, pkFile);
    fclose(pkFile);
    strFilter.addUnLawKeyWordsInUTF8(pkBuf);
    delete [] pkBuf;

    //////////////////////////////////////////////////////////////////////
    BCLib::uint32 findCnt = 0;
    BCLib::Utility::CRealTime realTime;
    realTime.now();
    BCLib::int64 startTime = realTime.getMilliseconds();

    for(BCLib::uint32 i=0; i<strList.size(); ++i)
    {
        findCnt += strFilter.checkAndReplaceUnLawKeyWordsInUTF8(strList[i], outStrList[i]) ? 1 : 0;
    }

    realTime.now();
    BCLib::int64 endTime = realTime.getMilliseconds();

    printf("totalCnt[%d] findCnt[%d] dura[%lld]ms cntPerMSec[%.2f]\n", strList.size(), findCnt, endTime-startTime, strList.size()/float(endTime-startTime));

    return 0;
}

int testCase_stringFilterCommand()
{
    MWLib::Misc::CMassStringFilter strFilter;

    FILE* pkFile = NULL;
    fopen_s(&pkFile, "screenWordList.csv", "rt");
    if(!pkFile)
    {
        return -1;
    }
    fseek(pkFile, 0, SEEK_END);
    size_t uiFileSize = ftell(pkFile);
    char* pkBuf = new char[uiFileSize];
    fseek(pkFile, 0, SEEK_SET);
    fread(pkBuf, uiFileSize, 1, pkFile);
    fclose(pkFile);
    strFilter.addUnLawKeyWordsInUTF8(pkBuf);
    delete [] pkBuf;

    char inputBuf[4096] = {0};
    std::string inputStr;
    std::string outputStr;
    while(gets_s(inputBuf))
    {
        inputStr = inputBuf;
        outputStr.clear();
        if(inputStr == "quit")
        {
            break;
        }
        printf("Input  : %s\n", inputStr.c_str());
        MWLib::Misc::ANSI_To_UTF8(inputStr);
        strFilter.checkAndReplaceUnLawKeyWordsInUTF8(inputStr, outputStr);
        MWLib::Misc::UTF8_To_ANSI(outputStr);
        printf("Output : %s\n", outputStr.c_str());
    }
    return 0;
}