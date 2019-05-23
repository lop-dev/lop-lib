//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/misc/stringFilter.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_MISC_STRINGFILTER_H__
#define __MWLIB_MISC_STRINGFILTER_H__

#include <MWLib/misc/miscDef.h>


namespace MWLib
{
namespace Misc
{
//! StringFilter用于小规模数量关键字的屏蔽
class MWLIB_MISC_API CStringFilter
{
public:
    CStringFilter();
    virtual ~CStringFilter();

    void setMark(char cMark);

    void addUnLawKeyWordInUTF8(const std::string& kUTF8Word);                                             //! kUTF8Word为UTF8格式
    void addUnLawKeyWordInANSI(const std::string& kANSIWord);                                             //! kANSIWord为ANSI格式
    void addUnLawKeyWordsInUTF8(const std::string& kUTF8WordsStream);                                     //! kUTF8WordsStream为UTF8格式,\n分行,一行一个关键字
    void addUnLawKeyWordsInANSI(const std::string& kANSIWordsStream);                                     //! kANSIWordsStream为ANSI格式,\n分行,一行一个关键字

    bool hasUnLawKeyWordInUTF8(const std::string& kUTF8Str) const;                                        //! kUTF8Str为UTF-8格式
    bool checkAndReplaceUnLawKeyWordsInUTF8(const std::string& kUTF8Src, std::string& kUTF8Des) const;    //! kUTF8Src,kUTF8Des为UTF-8格式

protected:
    struct SUnLawKeyWord
    {
        std::string m_unLawKeyWord;
        std::string m_markWord;

        SUnLawKeyWord(const std::string& unLawKeyWord, const std::string& markWord);
    };
    typedef std::vector<SUnLawKeyWord> UnLawKeyWords;
    UnLawKeyWords m_kUnLawKeyWords;
    char m_cMark;
};


//! FvMassStringFilter用于大规模数量关键字的屏蔽
class MWLIB_MISC_API CMassStringFilter
{
public:
    CMassStringFilter();
    ~CMassStringFilter();

    void setMark(char cMark);

    void addUnLawKeyWordsInUTF8(const std::string& kUTF8WordsStream);                                     //! kUTF8WordsStream为UTF8格式,\n分行,一行一个关键字
    void addUnLawKeyWordsInANSI(const std::string& kANSIWordsStream);                                     //! kANSIWordsStream为ANSI格式,\n分行,一行一个关键字

    bool hasUnLawKeyWordInUTF8(const std::string& kUTF8Str) const;                                        //! kUTF8Str为UTF-8格式
    bool checkAndReplaceUnLawKeyWordsInUTF8(const std::string& kUTF8Src, std::string& kUTF8Des) const;    //! kUTF8Src,kUTF8Des为UTF-8格式

protected:
    struct SUnLawKeyWord
    {
        std::string m_unLawKeyWord;
        std::string m_markWord;

        SUnLawKeyWord(const std::string& unLawKeyWord, const std::string& markWord);
    };
    typedef std::vector<SUnLawKeyWord> UnLawKeyWords;
    UnLawKeyWords* _findKeyWords(BCLib::uint64 uiWord) const;
    typedef std::map<BCLib::uint64, UnLawKeyWords*> UnLawKeyWordMap;
    void _addUnLawKeyWordInUTF8(const std::string& kUTF8Word, UnLawKeyWordMap& kMap);
    void _addUnLawKeyWordInANSI(const std::string& kANSIWord, UnLawKeyWordMap& kMap);

protected:
    typedef std::pair<BCLib::uint64, UnLawKeyWords*> UnLawKeyWordsPair;
    BCLib::uint32 m_uiUnLawKeyWordsPairArraySize;
    UnLawKeyWordsPair* m_pkUnLawKeyWordsPairArray;
    char m_cMark;
};


void MWLIB_MISC_API ANSI_To_UTF8(std::string& kStr);
void MWLIB_MISC_API UTF8_To_ANSI(std::string& kStr);
void MWLIB_MISC_API ANSI_To_UTF16(const std::string& kSrc, std::wstring& kDes);
void MWLIB_MISC_API UTF16_To_ANSI(const std::wstring& kSrc, std::string& kDes);
void MWLIB_MISC_API UTF8_To_UTF16(const std::string& kSrc, std::wstring& kDes);
void MWLIB_MISC_API UTF16_To_UTF8(const std::wstring& kSrc, std::string& kDes);

//! 取出下一个UTF8单字
size_t MWLIB_MISC_API getUTF8Word(const char* pcUTF8Str, size_t uiOffset, BCLib::uint64& kUTF8Word);

//! 判断一个字符是否为空白字符串
bool MWLIB_MISC_API isBlankString(const char* str);


//! 统计字符串中的"字数"
//! kStr: UTF8格式字符串
//! iAsciiCoef: Ascii字符代表的"字数"
//! iNotAsciiCoef: 非Ascii字符代表的"字数"
int MWLIB_MISC_API calcWordCnt(const std::string& kUTF8Str, int iAsciiCoef=1, int iNotAsciiCoef=1);

BCLib::uint16 MWLIB_MISC_API UTF8_TO_UNICODE(const char* pcUTF8Str, wchar_t* pwcUnicodeStr, BCLib::uint8 uiUnicodeLen);


namespace StrFilterTest
{
/**
    static void Test()
    {
        //! 统计字符串中的"字数"
        {
            std::string kANSIStr("KMP本质上是构造了DFA并进行了模拟，");
            //! 字符串必须为UTF8格式,转换UTF8格式
            ANSI_To_UTF8(kANSIStr);
            //! 统计字符串中的"字数",ANSI字符算1个,非ANSI字符算2个
            int iCnt = calcWordCnt(kANSIStr, 1, 2);
            printf("word cnt : %d\n", iCnt);
        }

        //! 屏蔽字检测
        {
            CStringFilter kFilter;

            //! 设置屏蔽码,被屏蔽的字显示'#'
            kFilter.setMark('#');

            //! 添加屏蔽字"ABC",ANSI格式
            kFilter.addUnLawKeyWordInANSI("ABC");

            //! 添加屏蔽字列表
            //! 从文件test.csv中读取,一行一个屏蔽字,ANSI格式
            //! test.csv中内容:
            //! DDD
            //! FFF
            {
                FILE* pkFile = fopen("test.csv", "rt");
                if(!pkFile)
                {
                    return;
                }
                fseek(pkFile, 0, SEEK_END);
                size_t uiFileSize = ftell(pkFile);
                char* pkBuf = new char[uiFileSize];
                fseek(pkFile, 0, SEEK_SET);
                fread(pkBuf, uiFileSize, 1, pkFile);
                fclose(pkFile);

                kFilter.addUnLawKeyWordsInANSI(pkBuf);
            }

            //! 检查字符串中是否有屏蔽字
            std::string kANSIStr("AAABC哈哈DDFFFDDE");
            //! 字符串必须为UTF8格式,转换UTF8格式
            ANSI_To_UTF8(kANSIStr);
            if(kFilter.hasUnLawKeyWordInUTF8(kANSIStr))
            {
                //! 包含屏蔽字
                UTF8_To_ANSI(kANSIStr);
                printf("%s\n", kANSIStr.c_str());
            }

            //! 检查并用屏蔽码替换屏蔽字
            ANSI_To_UTF8(kANSIStr);
            std::string kUTF8DesStr;
            if(kFilter.checkAndReplaceUnLawKeyWordsInUTF8(kANSIStr, kUTF8DesStr))
            {
                //! 包含屏蔽字
                UTF8_To_ANSI(kUTF8DesStr);
                printf("%s\n", kUTF8DesStr.c_str());
            }
        }
    }
**/
}//StrFilterTest
}//Misc
}//MWLib

#endif//__MWLIB_MISC_STRINGFILTER_H__
