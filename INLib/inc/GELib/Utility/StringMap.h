//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/StringMap.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_STRINGMAP_H__
#define __GELIB_UTILITY_STRINGMAP_H__

#include <map>
#include <string.h>
#include "VectorMap.h"

namespace GELib
{

// string is NOT copied

class CompareString
{
public:
    bool operator () (const char *s1, const char *s2) const
    {
        return strcmp(s1, s2) < 0;
    }
};

template<typename T> class StringMap : public std::map<const char *, T, CompareString>
{
};


class StringPointer
{
public:

	StringPointer() : m_pString(0)
	{
	}

	StringPointer(const char *s) : m_pString(s)
	{
	}

	bool operator<(const StringPointer &r) const
	{
		return strcmp(m_pString, r.m_pString) < 0;
	}

	operator const char *()
	{
		return m_pString;
	}

private:
	const char *m_pString;
};

template<typename T> class StringVectorMap : public VectorMap<StringPointer, T>
{
};


}//GELib


#endif//__GELIB_UTILITY_STRINGMAP_H__
