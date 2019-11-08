//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/misc/stringFilter.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/assert.h>
#include <BCLib/utility/convert.h>
#include <BCLib/utility/dateTime.h>
#include <BCLib/utility/random.h>
#include <MWLib/misc/stringFilter.h>

namespace MWLib
{
namespace Misc
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void ANSI_To_UTF8(std::string& kStr)
{
    std::string kTmp = kStr;
    BCLib::Utility::CConvert::gb2312ToUTF8(kStr, kTmp);
}

void UTF8_To_ANSI(std::string& kStr)
{
    std::string kTmp = kStr;
    BCLib::Utility::CConvert::utf8ToGB2312(kStr, kTmp);
}

void ANSI_To_UTF16(const std::string& kSrc, std::wstring& kDes)
{
    BCLib::Utility::CConvert::gb2312ToUnicode(kDes, kSrc);
}

void UTF16_To_ANSI(const std::wstring& kSrc, std::string& kDes)
{
    BCLib::Utility::CConvert::unicodeToGB2312(kDes, kSrc);
}

void UTF8_To_UTF16(const std::string& kSrc, std::wstring& kDes)
{
    BCLib::Utility::CConvert::utf8ToUnicode(kDes, kSrc);
}

void UTF16_To_UTF8(const std::wstring& kSrc, std::string& kDes)
{
    BCLib::Utility::CConvert::unicodeToUTF8(kDes, kSrc);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
CStringFilter::CStringFilter()
:m_cMark('*')
{
    m_kUnLawKeyWords.reserve(8);
}

CStringFilter::~CStringFilter()
{
    ;
}

void CStringFilter::setMark(char cMark)
{
    m_cMark = cMark;
}

void CStringFilter::addUnLawKeyWordInUTF8(const std::string& kUTF8Word)
{
    if(kUTF8Word.empty() || isBlankString(kUTF8Word.c_str()))
    {
        return;
    }
    BCLib::uint32 wordCnt = calcWordCnt(kUTF8Word, 1, 1);
    std::string kMarkWord;
    kMarkWord.resize(wordCnt, m_cMark);
    m_kUnLawKeyWords.push_back(SUnLawKeyWord(kUTF8Word, kMarkWord));
}

void CStringFilter::addUnLawKeyWordInANSI(const std::string& kANSIWord)
{
    std::string kTmp(kANSIWord);
    ANSI_To_UTF8(kTmp);
    addUnLawKeyWordInUTF8(kTmp);
}

void CStringFilter::addUnLawKeyWordsInUTF8(const std::string& kUTF8WordsStream)
{
    std::string::size_type uiPos = 0;
    std::string::size_type uiPosNext = 0;
    while((uiPosNext = kUTF8WordsStream.find("\n", uiPos)) != std::string::npos)
    {
        addUnLawKeyWordInUTF8(kUTF8WordsStream.substr(uiPos, uiPosNext-uiPos));
        uiPos = uiPosNext +1;
    }
}

void CStringFilter::addUnLawKeyWordsInANSI(const std::string& kANSIWordsStream)
{
    std::string::size_type uiPos = 0;
    std::string::size_type uiPosNext = 0;
    while((uiPosNext = kANSIWordsStream.find("\n", uiPos)) != std::string::npos)
    {
        addUnLawKeyWordInANSI(kANSIWordsStream.substr(uiPos, uiPosNext-uiPos));
        uiPos = uiPosNext +1;
    }
}

bool CStringFilter::hasUnLawKeyWordInUTF8(const std::string& kUTF8Str) const
{
    if(kUTF8Str.empty() || m_kUnLawKeyWords.empty())
    {
        return false;
    }
    const SUnLawKeyWord* pkIdx = &m_kUnLawKeyWords[0];
    const SUnLawKeyWord* pkEnd = pkIdx + m_kUnLawKeyWords.size();
    for(; pkIdx!=pkEnd; ++pkIdx)
    {
        //! TODO: 换成KMP算法,会不会更快?
        if(kUTF8Str.find(pkIdx->m_unLawKeyWord) != std::string::npos)
        {
            return true;
        }
    }
    return false;
}

bool CStringFilter::checkAndReplaceUnLawKeyWordsInUTF8(const std::string& kUTF8Src, std::string& kUTF8Des) const
{
    if(kUTF8Src.empty() || m_kUnLawKeyWords.empty())
    {
        return false;
    }
    kUTF8Des = kUTF8Src;
    bool bFind = false;
    const SUnLawKeyWord* pkIdx = &m_kUnLawKeyWords[0];
    const SUnLawKeyWord* pkEnd = pkIdx + m_kUnLawKeyWords.size();
    for(; pkIdx!=pkEnd; ++pkIdx)
    {
        //! TODO: 换成KMP算法,会不会更快?
        std::string::size_type uiSrcPos = 0;
        std::string::size_type uiDstPosDiff = 0;
        while((uiSrcPos = kUTF8Src.find(pkIdx->m_unLawKeyWord, uiSrcPos)) != std::string::npos)
        {
            bFind = true;
            std::string::size_type uiUnLawKeyWordLen = pkIdx->m_unLawKeyWord.length();
            std::string::size_type uiMarkWordLen = pkIdx->m_markWord.length();
            kUTF8Des = kUTF8Des.substr(0, uiSrcPos-uiDstPosDiff) + pkIdx->m_markWord + kUTF8Src.substr(uiSrcPos+uiUnLawKeyWordLen, kUTF8Src.length()-(uiSrcPos+uiUnLawKeyWordLen));
            uiSrcPos += uiUnLawKeyWordLen;
            uiDstPosDiff += uiUnLawKeyWordLen - uiMarkWordLen;
        }
    }
    return bFind;
}

CStringFilter::SUnLawKeyWord::SUnLawKeyWord(const std::string& unLawKeyWord, const std::string& markWord)
:m_unLawKeyWord(unLawKeyWord)
,m_markWord(markWord)
{
    ;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
CMassStringFilter::CMassStringFilter()
:m_uiUnLawKeyWordsPairArraySize(0)
,m_pkUnLawKeyWordsPairArray(NULL)
,m_cMark('*')
{
    ;
}

CMassStringFilter::~CMassStringFilter()
{
    if(m_pkUnLawKeyWordsPairArray)
    {
        for(BCLib::uint32 i=0; i<m_uiUnLawKeyWordsPairArraySize; ++i)
        {
            delete m_pkUnLawKeyWordsPairArray[i].second;
        }
        delete [] m_pkUnLawKeyWordsPairArray;
    }
}

void CMassStringFilter::_addUnLawKeyWordInUTF8(const std::string& kUTF8Word, UnLawKeyWordMap& kMap)
{
    if(kUTF8Word.empty() || isBlankString(kUTF8Word.c_str()))
    {
        return;
    }
    BCLib::uint64 kFirstWord;
    if(getUTF8Word(kUTF8Word.c_str(), 0, kFirstWord) == size_t(-1))
    {
        return;
    }
    BCLib::uint32 wordCnt = calcWordCnt(kUTF8Word, 1, 1);
    std::string kMarkWord;
    kMarkWord.resize(wordCnt, m_cMark);
    UnLawKeyWords* pkUnLawKeyWords = NULL;
    UnLawKeyWordMap::iterator itr = kMap.find(kFirstWord);
    if(itr == kMap.end())
    {
        pkUnLawKeyWords = new UnLawKeyWords();
        kMap.insert(std::make_pair(kFirstWord, pkUnLawKeyWords));
        pkUnLawKeyWords->reserve(8);
        pkUnLawKeyWords->push_back(SUnLawKeyWord(kUTF8Word, kMarkWord));
    }
    else
    {
        pkUnLawKeyWords = itr->second;
        SUnLawKeyWord* pkIdx = &(*pkUnLawKeyWords)[0];
        SUnLawKeyWord* pkEnd = pkIdx + pkUnLawKeyWords->size();
        for(; pkIdx!=pkEnd; ++pkIdx)
        {
            if(kUTF8Word == pkIdx->m_unLawKeyWord)
            {
                return;
            }
        }
        pkUnLawKeyWords->push_back(SUnLawKeyWord(kUTF8Word, kMarkWord));
    }
}

void CMassStringFilter::_addUnLawKeyWordInANSI(const std::string& kANSIWord, UnLawKeyWordMap& kMap)
{
    std::string kTmp(kANSIWord);
    ANSI_To_UTF8(kTmp);
    _addUnLawKeyWordInUTF8(kTmp, kMap);
}

void CMassStringFilter::setMark(char cMark)
{
    m_cMark = cMark;
}

void CMassStringFilter::addUnLawKeyWordsInUTF8(const std::string& kUTF8WordsStream)
{
    UnLawKeyWordMap kUnLawKeyWordMap;
    std::string::size_type uiPos = 0;
    std::string::size_type uiPosNext = 0;
    while((uiPosNext = kUTF8WordsStream.find("\n", uiPos)) != std::string::npos)
    {
        _addUnLawKeyWordInUTF8(kUTF8WordsStream.substr(uiPos, uiPosNext-uiPos), kUnLawKeyWordMap);
        uiPos = uiPosNext +1;
    }
    m_uiUnLawKeyWordsPairArraySize = BCLib::uint32(kUnLawKeyWordMap.size());
    if(!m_uiUnLawKeyWordsPairArraySize)
    {
        return;
    }
    m_pkUnLawKeyWordsPairArray = new UnLawKeyWordsPair[m_uiUnLawKeyWordsPairArraySize];
    UnLawKeyWordsPair* pkIdx = m_pkUnLawKeyWordsPairArray;
    UnLawKeyWordMap::iterator itrB = kUnLawKeyWordMap.begin();
    UnLawKeyWordMap::iterator itrE = kUnLawKeyWordMap.end();
    for(; itrB!=itrE; ++itrB, ++pkIdx)
    {
        pkIdx->first = itrB->first;
        pkIdx->second = itrB->second;
    }
}

void CMassStringFilter::addUnLawKeyWordsInANSI(const std::string& kANSIWordsStream)
{
    UnLawKeyWordMap kUnLawKeyWordMap;
    std::string::size_type uiPos = 0;
    std::string::size_type uiPosNext = 0;
    while((uiPosNext = kANSIWordsStream.find("\n", uiPos)) != std::string::npos)
    {
        _addUnLawKeyWordInANSI(kANSIWordsStream.substr(uiPos, uiPosNext-uiPos), kUnLawKeyWordMap);
        uiPos = uiPosNext +1;
    }
    m_uiUnLawKeyWordsPairArraySize = BCLib::uint32(kUnLawKeyWordMap.size());
    if(!m_uiUnLawKeyWordsPairArraySize)
    {
        return;
    }
    m_pkUnLawKeyWordsPairArray = new UnLawKeyWordsPair[m_uiUnLawKeyWordsPairArraySize];
    UnLawKeyWordsPair* pkIdx = m_pkUnLawKeyWordsPairArray;
    UnLawKeyWordMap::iterator itrB = kUnLawKeyWordMap.begin();
    UnLawKeyWordMap::iterator itrE = kUnLawKeyWordMap.end();
    for(; itrB!=itrE; ++itrB, ++pkIdx)
    {
        pkIdx->first = itrB->first;
        pkIdx->second = itrB->second;
    }
}

bool CMassStringFilter::hasUnLawKeyWordInUTF8(const std::string& kUTF8Str) const
{
    if(kUTF8Str.empty() || !m_uiUnLawKeyWordsPairArraySize)
    {
        return false;
    }
    size_t uiOffset = 0;
    size_t uiOffsetNext = 0;
    BCLib::uint64 kWord;
    while((uiOffsetNext = getUTF8Word(kUTF8Str.c_str(), uiOffset, kWord)) != size_t(-1))
    {
        UnLawKeyWords* pkUnLawKeyWords = _findKeyWords(kWord);
        if(!pkUnLawKeyWords)
        {
            uiOffset = uiOffsetNext;
            continue;
        }
        const UnLawKeyWords& kUnLawKeyWords = *pkUnLawKeyWords;
        BCLIB_ASSERT(kUnLawKeyWords.size());
        const SUnLawKeyWord* pkIdx = &kUnLawKeyWords[0];
        const SUnLawKeyWord* pkEnd = pkIdx + kUnLawKeyWords.size();
        for(; pkIdx!=pkEnd; ++pkIdx)
        {
            //! TODO: 换成KMP算法,会不会更快?
            if(kUTF8Str.find(pkIdx->m_unLawKeyWord, uiOffset) != std::string::npos)
            {
                return true;
            }
        }
        uiOffset = uiOffsetNext;
    }
    return false;
}

bool CMassStringFilter::checkAndReplaceUnLawKeyWordsInUTF8(const std::string& kUTF8Src, std::string& kUTF8Des) const
{
    if(kUTF8Src.empty() || !m_uiUnLawKeyWordsPairArraySize)
    {
        return false;
    }
    kUTF8Des = kUTF8Src;
    bool bFind = false;
    size_t uiOffset = 0;
    size_t uiOffsetNext = 0;
    BCLib::uint64 kWord;
    UnLawKeyWordMap::const_iterator itr;
    while((uiOffsetNext = getUTF8Word(kUTF8Des.c_str(), uiOffset, kWord)) != size_t(-1))
    {
        if(kWord == (BCLib::uint64)m_cMark)
        {
            uiOffset = uiOffsetNext;
            continue;
        }
        UnLawKeyWords* pkUnLawKeyWords = _findKeyWords(kWord);
        if(!pkUnLawKeyWords)
        {
            uiOffset = uiOffsetNext;
            continue;
        }
        std::string::size_type uiDesResLen = kUTF8Des.length() - uiOffset;
        const UnLawKeyWords& kUnLawKeyWords = *pkUnLawKeyWords;
        BCLIB_ASSERT(kUnLawKeyWords.size());
        const SUnLawKeyWord* pkIdx = &kUnLawKeyWords[0];
        const SUnLawKeyWord* pkEnd = pkIdx + kUnLawKeyWords.size();
        for(; pkIdx!=pkEnd; ++pkIdx)
        {
            //! TODO: 换成KMP算法,会不会更快?
            std::string::size_type uiUnLawKeyWordLen = pkIdx->m_unLawKeyWord.length();
            if(uiDesResLen >= uiUnLawKeyWordLen && kUTF8Des.compare(uiOffset, uiUnLawKeyWordLen, pkIdx->m_unLawKeyWord) == 0)
            {
                bFind = true;
                kUTF8Des = kUTF8Des.substr(0, uiOffset) + pkIdx->m_markWord + kUTF8Des.substr(uiOffset+uiUnLawKeyWordLen, uiDesResLen-uiUnLawKeyWordLen);
                uiOffset += pkIdx->m_markWord.length();
                break;
            }
        }
        if(pkIdx == pkEnd)
        {
            uiOffset = uiOffsetNext;
        }
    }
    return bFind;
}

CMassStringFilter::SUnLawKeyWord::SUnLawKeyWord(const std::string& unLawKeyWord, const std::string& markWord)
:m_unLawKeyWord(unLawKeyWord)
,m_markWord(markWord)
{
    ;
}

CMassStringFilter::UnLawKeyWords* CMassStringFilter::_findKeyWords(BCLib::uint64 uiWord) const
{
    if(!m_uiUnLawKeyWordsPairArraySize ||
        uiWord < m_pkUnLawKeyWordsPairArray[0].first ||
        m_pkUnLawKeyWordsPairArray[m_uiUnLawKeyWordsPairArraySize -1].first < uiWord)
    {
        return NULL;
    }
    BCLib::uint32 uiFirst = 0;
    BCLib::uint32 uiLast = m_uiUnLawKeyWordsPairArraySize -1;
    BCLib::uint32 uiMid = 0;
    do
    {
        uiMid = (uiFirst + uiLast) >> 1;
        if(m_pkUnLawKeyWordsPairArray[uiMid].first < uiWord)
        {
            uiFirst = uiMid +1;
        }
        else if(uiWord < m_pkUnLawKeyWordsPairArray[uiMid].first)
        {
            uiLast = uiMid -1;
        }
        else
        {
            return m_pkUnLawKeyWordsPairArray[uiMid].second;
        }
    }
    while(uiFirst <= uiLast);
    return NULL;
}

size_t getUTF8Word(const char* pcUTF8Str, size_t uiOffset, BCLib::uint64& kUTF8Word)
{
    pcUTF8Str += uiOffset;
    BCLib::uint8 cTmp = *(BCLib::uint8*)pcUTF8Str;
    if(!cTmp)
    {
        return size_t(-1);
    }
    if((cTmp & 0x80) == 0)//! 0XXX XXXX
    {
        kUTF8Word = cTmp;
        return uiOffset+1;
    }
    else if((cTmp & 0xE0) == 0xC0)//! 110X XXXX
    {
        kUTF8Word = *(BCLib::uint16*)pcUTF8Str;
        return uiOffset+2;
    }
    else if((cTmp & 0xF0) == 0xE0)//! 1110 XXXX
    {
        kUTF8Word = (cTmp<<16) + (*(BCLib::uint16*)(pcUTF8Str+1));
        return uiOffset+3;
    }
    else if((cTmp & 0xF8) == 0xF0)//! 1111 0XXX
    {
        kUTF8Word = *(BCLib::uint32*)pcUTF8Str;
        return uiOffset+4;
    }
    else if((cTmp & 0xFC) == 0xF8)//! 1111 10XX
    {
        kUTF8Word = ((BCLib::uint64)cTmp<<32) + (*(BCLib::uint32*)(pcUTF8Str+1));
        return uiOffset+5;
    }
    else if((cTmp & 0xFE) == 0xFC)//! 1111 110X
    {
        kUTF8Word = ((BCLib::uint64)(*(BCLib::uint16*)pcUTF8Str)<<32) + (*(BCLib::uint32*)(pcUTF8Str+2));
        return uiOffset+6;
    }
    else
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "UTF-8 Mark Err");
        return size_t(-1);
    }
}

bool isBlankString(const char* str)
{
    if(!str || !str[0])
    {
        return true;
    }
    while(*str)
    {
        if(*str != '\t' && *str != ' ')
        {
            return false;
        }
        ++str;
    }
    return true;
}

int calcWordCnt(const std::string& kUTF8Str, int iAsciiCoef, int iNotAsciiCoef)
{
    if(kUTF8Str.empty())
    {
        return 0;
    }
    size_t uiOffset = 0;
    size_t uiOffsetNext = 0;
    BCLib::uint64 kWord;
    int iCnt = 0;
    while((uiOffsetNext = getUTF8Word(kUTF8Str.c_str(), uiOffset, kWord)) != size_t(-1))
    {
        if(kWord < 0x80)
        {
            iCnt += iAsciiCoef;
        }
        else
        {
            iCnt += iNotAsciiCoef;
        }
        uiOffset = uiOffsetNext;
    }
    return iCnt;
}

BCLib::uint16 UTF8_TO_UNICODE(const char* pcUTF8Str, wchar_t* pwcUnicodeStr, BCLib::uint8 uiUnicodeLen)
{
    if(uiUnicodeLen <= 0)
    {
        return BCLib::uint16(-1);
    }
    BCLib::uint16 usUnicodeCount = 0;
    while(*pcUTF8Str)
    {
        if((*pcUTF8Str & 0x80) == 0)
        {
            ++usUnicodeCount;
            if(usUnicodeCount > uiUnicodeLen)
            {
                return BCLib::uint16(-1);
            }
            *pwcUnicodeStr = (wchar_t)(*pcUTF8Str);
            pcUTF8Str++;
            pwcUnicodeStr++;
        }
        else if((*pcUTF8Str & 0xE0) == 0xC0)
        {
            ++usUnicodeCount;
            if(usUnicodeCount > uiUnicodeLen)
            {
                return BCLib::uint16(-1);
            }
            char uiTemp1 = (*pcUTF8Str & (0xff >> 3));
            pcUTF8Str++;
            char uiTemp2 = (*pcUTF8Str & (0xff >> 2));
            pcUTF8Str++;
            wchar_t wcTemp1 = ((wchar_t)(uiTemp1 >> 2)) << 13;
            wchar_t wcTemp2 = (wchar_t)(((uiTemp1 & (0xff >> 6)) << 6) | uiTemp2);
            *pwcUnicodeStr = wcTemp1 | wcTemp2;
            pwcUnicodeStr++;
        }
        else if((*pcUTF8Str & 0xF0) == 0xE0)
        {
            ++usUnicodeCount;
            if(usUnicodeCount > uiUnicodeLen)
            {
                return BCLib::uint16(-1);
            }
            char uiTemp1 = (*pcUTF8Str & (0xff >> 3));
            pcUTF8Str++;
            char uiTemp2 = (*pcUTF8Str & (0xff >> 2));
            pcUTF8Str++;
            char uiTemp3 = (*pcUTF8Str & (0xff >> 2));
            pcUTF8Str++;
            wchar_t wcTemp1 = ((wchar_t)((uiTemp1 << 4) | (uiTemp2 >> 2))) << 8;
            wchar_t wcTemp2 = (wchar_t)(((uiTemp2 & (0xff >> 6)) << 6) | uiTemp3);
            *pwcUnicodeStr = wcTemp1 | wcTemp2;
            pwcUnicodeStr++;
        }
        else
        {
            return BCLib::uint16(-1);
        }
    }
    return usUnicodeCount;
}
}//Misc
}//MWLib