//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/string.h
//  author:     League of Perfect
/// @brief      字符串处理
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_STRING_H__
#define __BCLIB_UTILITY_STRING_H__

#include <algorithm>
#include <ctype.h>
#include <BCLib/utility/baseDef.h>

namespace BCLib
{
namespace Utility
{
class BCLIB_UTILITY_API CSpecialCharA
{
protected:
    static const std::string::value_type m_A;
    static const std::string::value_type m_Z;
    static const std::string::value_type m_a;
    static const std::string::value_type m_z;
    static const std::string::value_type m_percent;
};

class BCLIB_UTILITY_API CSpecialCharW
{
protected:
    static const std::wstring::value_type m_A;
    static const std::wstring::value_type m_Z;
    static const std::wstring::value_type m_a;
    static const std::wstring::value_type m_z;
    static const std::wstring::value_type m_percent;
};

template<class BaseStringType, class SpecialCharType>
class CBasicString : public BaseStringType, public SpecialCharType
{
public:
    typedef typename BaseStringType::value_type ValueType;
    typedef typename BaseStringType::size_type SizeType;

public:
    /// @brief 转换成小写
    /// @return void
    void toLower()
    {
        ValueType fixedValue = SpecialCharType::m_a - SpecialCharType::m_A;
        typename BaseStringType::iterator it = BaseStringType::begin();
        for(; it != BaseStringType::end(); ++it)
        {
            if(*it >= SpecialCharType::m_A && *it <= SpecialCharType::m_Z)
            {
                *it += fixedValue;
            }
        }
    }

    /// @brief 转换成大写
    /// @return void
    void toUpper()
    {
        ValueType fixedValue = SpecialCharType::m_a - SpecialCharType::m_A;
        typename BaseStringType::iterator it = BaseStringType::begin();
        for(; it != BaseStringType::end(); ++it)
        {
            if(*it >= SpecialCharType::m_a && *it <= SpecialCharType::m_z)
            {
                *it -= fixedValue;
            }
        }
    }

    /// @brief 不分大小写，比较
    /// @return bool true = 相等
    /// @param str
    bool compareNoCase(const BaseStringType& str)
    {
        SizeType strSize = BaseStringType::size();
        if(str.size() != strSize)
        {
            return false;
        }
        int fixedValue1 = SpecialCharType::m_a - SpecialCharType::m_A;
        int fixedValue2 = -fixedValue1;

        for(uint32 i = 0; i < strSize; ++i)
        {
            int tmp = str[i] - BaseStringType::operator[](i);
            if(tmp == 0 || tmp == fixedValue1 || tmp == fixedValue2)
            {
                continue;
            }
            return false;
        }
        return true;
    }

    /// @brief 替换所有strOld字符串
    /// @return unsigned int 替换的次数
    /// @param strOld
    /// @param strNew
    uint32 replace(const BaseStringType& strOld, const BaseStringType& strNew)
    {
        uint32 nCount = 0;
        typename BaseStringType::iterator itBegin = std::search(BaseStringType::begin(), BaseStringType::end(), strOld.begin(), strOld.end());
        while(itBegin != BaseStringType::end())
        {
            typename BaseStringType::iterator itEnd = itBegin + strOld.size();
            BaseStringType::replace(itBegin, itEnd, strNew);
            itBegin = std::search(BaseStringType::begin() + strNew.size(), BaseStringType::end(), strOld.begin(), strOld.end());
            ++nCount;
        }
        return nCount;
    }
    uint32 replace(const ValueType chOld, const ValueType chNew)
    {
        uint32 nCount = 0;
        typename BaseStringType::iterator it = BaseStringType::begin();
        for(; it != BaseStringType::end(); ++it)
        {
            if(*it == chOld)
            {
                *it = chNew;
                ++nCount;
            }
        }
        return nCount;
    }

    /// @brief 返回一个从右边边起始nCount个字符的子字符串，[length() - nCount，nCount]，类似于std::substr(length() - nCount，nCount)
    /// @return BaseStringType
    /// @param nCount
    BaseStringType right(SizeType nCount)
    {
        if(nCount >= BaseStringType::size())
        {
            return *this;
        }
        return BaseStringType::substr(BaseStringType::size() - nCount, nCount);
    }
    /// @brief 返回一个从左边起始nCount个字符的子字符串，类似于std::substr(0，nCount)
    /// @return BaseStringType
    /// @param nCount 启始位置
    BaseStringType left(SizeType nCount)
    {
        if(nCount >= BaseStringType::size())
        {
            return *this;
        }
        return BaseStringType::substr(0, nCount);
    }

    /// @brief 类似于std::substr(iFirst，nCount)
    /// @return BaseStringType
    /// @param iFirst
    /// @param nCount
    BaseStringType mid(SizeType iFirst, SizeType nCount)
    {
        if(iFirst > BaseStringType::size())
        {
            iFirst = BaseStringType::size();
        }
        if(nCount + iFirst > BaseStringType::size())
        {
            nCount = BaseStringType::size() - iFirst;
        }
        return BaseStringType::substr(iFirst, nCount);
    }


    /// @brief 按照strKeyword拆分字符串
    /// @return int
    /// @param subStringList
    /// @param strKeyword
    template<class SubStringType>
    int split(std::list<SubStringType>& subStringList, BaseStringType& strKeyword)
    {
        SizeType npos = BaseStringType::find(strKeyword);
        SizeType nlastpos = 0;

        while(npos != BaseStringType::npos)
        {
            subStringList.push_back(mid(nlastpos, npos - nlastpos));
            nlastpos = npos + 1;
            npos = BaseStringType::find(strKeyword, nlastpos);
        }

        if(nlastpos < BaseStringType::length())
        {
            subStringList.push_back(mid(nlastpos, BaseStringType::length() - nlastpos));
        }
        return subStringList.size();
    }

    /// @brief 与split类似,结果保存在subStringList,返回拆分的个数
    /// @return int
    /// @param subStringList
    /// @param separator
    template<class SubStringType>
    int split(std::list<SubStringType>& subStringList, ValueType separator)
    {
        SubStringType tmpStr;

        subStringList.clear();

        typename BaseStringType::iterator it = BaseStringType::begin();
        for(; it != BaseStringType::end(); ++it)
        {
            if(*it == separator)
            {
				// 为了兼容 CSV 格式，形如“1,24,,,,100,6,0,,,0”的解析，否则数据会错位
                //if(tmpStr.empty())
                //{
                //    continue;
                //}
                subStringList.push_back(tmpStr);
                tmpStr.clear();
                continue;
            }
            tmpStr.push_back(*it);
        }
        if(!(tmpStr.empty()))
        {
            subStringList.push_back(tmpStr);
            tmpStr.clear();
        }
        return subStringList.size();
    }

    /// @brief 从左边去掉连续的target
    /// @return void
    /// @param target 字符串数组
	void trimLeft(ValueType target[])
	{
		typename BaseStringType::iterator it = BaseStringType::begin();
		while (it != BaseStringType::end())
		{
#ifdef _UNICODE
			if (wcsrchr(target, *it) != NULL)
#else
			if (strchr(target, *it) != NULL)
#endif
			{
				++it;
				continue;
			}
			break;
		}
		BaseStringType::erase(BaseStringType::begin(), it);
	}

    /// @brief 从右边去掉连续的target
    /// @return void
    /// @param target 字符串数组
	void trimRight(ValueType target[])
	{
		typename BaseStringType::iterator it = BaseStringType::end();
		if (BaseStringType::begin() == it)
		{
			return;
		}
		--it;

		while (it != BaseStringType::begin())
		{
#ifdef _UNICODE
			if (wcsrchr(target, *it) != NULL)
#else
			if (strchr(target, *it) != NULL)
#endif
			{
				--it;
				continue;
			}
			break;
		}
		if (it == BaseStringType::begin())
		{
#ifdef _UNICODE
			if (wcsrchr(target, *it) != NULL)
#else
			if (strchr(target, *it) != NULL)
#endif
			//if (*it == target)
			{
				BaseStringType::erase(it, BaseStringType::end());
				return;
			}
		}
		++it;
		if (it == BaseStringType::end())
		{
			return;
		}
		BaseStringType::erase(it, BaseStringType::end());
	}

    /// @brief 从左边去掉连续的target，直到第一个字符不为 target
    ///        如原字符串aaabbaa,target为a,处理完就是bbaa
    /// @return void
    /// @param target 单个字符
    void trimLeft(ValueType target = ' ')
    {
        ValueType targets[] = { target };
        trimLeft(targets);
    }

    /// @brief 从右边去掉连续的target，直到第一个字符不为 target
    ///        如原字符串aaabbaa,target为a,处理完就是aaabb
    /// @return void
    /// @param target 单个字符
    void trimRight(ValueType target = ' ')
    {
        ValueType targets[] = { target };
        trimRight(targets);
    }

    /// @brief 从左边和右边去掉连续的target，直到第一个字符不为 target
    ///        如原字符串aaabbaa,target为a,处理完就是bb
    /// @return void
    /// @param target 单个字符
    void trim(ValueType target = ' ')
    {
		ValueType targets[] = { target };
        trimLeft(targets);
        trimRight(targets);
    }

    /// @brief 加强版本，去掉空格、换行等一切不可见字符
    /// @return void
    void trimSpace()
    {
        ValueType target[] = " \n\r\t\f\v";
        trimLeft(target);
        trimRight(target);
    }

protected:
    bool _formatSafeCheck(uint32 uParamCnt, const ValueType* pszFormat)
    {
        if(pszFormat == NULL)
        {
            return false;
        }
        uint32 nCnt = 0;
        int i = 0;
        for(; pszFormat[i] != 0; ++i)
        {
            if(pszFormat[i] == SpecialCharType::m_percent)
            {
                if(pszFormat[i + 1] == SpecialCharType::m_percent)
                {
                    ++i;
                    continue;
                }
                ++nCnt;
            }
        }
        if(i == 0)
        {
            return true;
        }
        if(pszFormat[i - 1] == SpecialCharType::m_percent)
        {
            if(i - 2 < 0)
            {
                return false;
            }
            if(pszFormat[i - 2] != SpecialCharType::m_percent)
            {
                return false;
            }
        }
        return nCnt == uParamCnt;
    }
};

class CStringA;
class CStringW;

class BCLIB_UTILITY_API CStringA : public CBasicString<std::string, CSpecialCharA>
{
public:
    CStringA()
    {
    }

    template<class Type>
    CStringA(const Type& data)
    {
        operator = (data);
    }

    /// @brief 转换成宽字符
    /// @return CStringW
    CStringW toWideString() const;

    /// @brief
    /// @return void
    /// @param uParamCnt
    /// @param pszFormat
    /// @param
    void format(uint32 uParamCnt, const ValueType* pszFormat, ...);
    void format(const ValueType* pszFormat, ...);

public:
    CStringA& operator =(const char data[]);
    CStringA& operator =(const wchar_t data[]);
    CStringA& operator =(const CStringA& data);
    CStringA& operator =(const CStringW& data);
    CStringA& operator =(const std::string& data);
    CStringA& operator =(const std::wstring& data);
    CStringA& operator =(const int8& data);
    CStringA& operator =(const uint8& data);
    CStringA& operator =(const int16& data);
    CStringA& operator =(const uint16& data);
    CStringA& operator =(const int32& data);
    CStringA& operator =(const uint32& data);
    CStringA& operator =(const int64& data);
    CStringA& operator =(const uint64& data);
    CStringA& operator =(const float& data);
    CStringA& operator =(const double& data);

    template<class Type>
    CStringA& operator =(const Type& data);

    CStringA& operator <<(const char data[]);
    CStringA& operator <<(const wchar_t data[]);
    CStringA& operator <<(const CStringA& data);
    CStringA& operator <<(const CStringW& data);
    CStringA& operator <<(const std::string& data);
    CStringA& operator <<(const std::wstring& data);
    CStringA& operator <<(const int8& data);
    CStringA& operator <<(const uint8& data);
    CStringA& operator <<(const int16& data);
    CStringA& operator <<(const uint16& data);
    CStringA& operator <<(const int32& data);
    CStringA& operator <<(const uint32& data);
    CStringA& operator <<(const int64& data);
    CStringA& operator <<(const uint64& data);
    CStringA& operator <<(const float& data);
    CStringA& operator <<(const double& data);

    template<class Type>
    CStringA& operator <<(const Type& data);

    const CStringA& operator >>(int8& data) const;
    const CStringA& operator >>(uint8& data) const;
    const CStringA& operator >>(int16& data) const;
    const CStringA& operator >>(uint16& data) const;
    const CStringA& operator >>(int32& data) const;
    const CStringA& operator >>(uint32& data) const;
    const CStringA& operator >>(int64& data) const;
    const CStringA& operator >>(uint64& data) const;
    const CStringA& operator >>(double& data) const;
    const CStringA& operator >>(float& data) const;

    template<class Type>
    const CStringA& operator >>(Type& data) const;
};

class BCLIB_UTILITY_API CStringW : public CBasicString<std::wstring, CSpecialCharW>
{
public:
    CStringW()
    {
    }

    template<class Type>
    CStringW(Type data)
    {
        operator = (data);
    }

    /// @brief  转换成窄字符
    /// @return CStringA&
    CStringA toNarrowString() const;

    /// @brief
    /// @return void
    /// @param uParamCnt
    /// @param pszFormat
    /// @param
    void format(uint32 uParamCnt, const ValueType* pszFormat, ...);
    void format(const ValueType* pszFormat, ...);

public:
    CStringW& operator =(const char data[]);
    CStringW& operator =(const wchar_t data[]);
    CStringW& operator =(const CStringA& data);
    CStringW& operator =(const CStringW& data);
    CStringW& operator =(const std::string& data);
    CStringW& operator =(const std::wstring& data);
    CStringW& operator =(const int8& data);
    CStringW& operator =(const uint8& data);
    CStringW& operator =(const int16& data);
    CStringW& operator =(const uint16& data);
    CStringW& operator =(const int32& data);
    CStringW& operator =(const uint32& data);
    CStringW& operator =(const int64& data);
    CStringW& operator =(const uint64& data);
    CStringW& operator =(const float& data);
    CStringW& operator =(const double& data);

    template<class Type>
    CStringW& operator =(const Type& data);

    CStringW& operator <<(const char data[]);
    CStringW& operator <<(const wchar_t data[]);
    CStringW& operator <<(const CStringA& data);
    CStringW& operator <<(const CStringW& data);
    CStringW& operator <<(const std::string& data);
    CStringW& operator <<(const std::wstring& data);
    CStringW& operator <<(const int8& data);
    CStringW& operator <<(const uint8& data);
    CStringW& operator <<(const int16& data);
    CStringW& operator <<(const uint16& data);
    CStringW& operator <<(const int32& data);
    CStringW& operator <<(const uint32& data);
    CStringW& operator <<(const int64& data);
    CStringW& operator <<(const uint64& data);
    CStringW& operator <<(const float& data);
    CStringW& operator <<(const double& data);

    template<class Type>
    CStringW& operator <<(const Type& data);

    const CStringW& operator >>(int8& data) const;
    const CStringW& operator >>(uint8& data) const;
    const CStringW& operator >>(int16& data) const;
    const CStringW& operator >>(uint16& data) const;
    const CStringW& operator >>(int32& data) const;
    const CStringW& operator >>(uint32& data) const;
    const CStringW& operator >>(int64& data) const;
    const CStringW& operator >>(uint64& data) const;
    const CStringW& operator >>(double& data) const;
    const CStringW& operator >>(float& data) const;

    template<class Type>
    const CStringW& operator >>(Type& data) const;
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_STRING_H__
