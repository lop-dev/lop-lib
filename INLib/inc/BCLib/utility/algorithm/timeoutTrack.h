//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/algorithm/timeoutTrack.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_IO_TIMEOUTTRACK_H__
#define __BCLIB_UTILITY_IO_TIMEOUTTRACK_H__

#include <BCLib/utility/baseDef.h>
#include <BCLib/utility/hashMap.h>

namespace BCLib
{
namespace Utility
{

// ³¬Ê±¹ìµÀ
template<class CINFOPTR>
class CTimeoutTrack
{
public:
	CTimeoutTrack(){}
	virtual ~CTimeoutTrack(){}

	void update(float t)
	{
		typename TimeoutHashMap::iterator it = m_TimeoutHashMap.begin();
		for(; it != m_TimeoutHashMap.end(); )
		{
			if (it->second > t)
			{
				it->second -= t;
				++it;
			}
			else
			{
				_onTimeout(it->first);
				it = m_TimeoutHashMap.erase(it);
			}
		}
	}

	void addTimeout(CINFOPTR infoPtr, BCLib::int64 uSecond)
	{
		if (infoPtr && (uSecond > 0))
		{
			m_TimeoutHashMap.setValue(infoPtr, uSecond);
		}
	}

protected:
	virtual void _onTimeout(CINFOPTR infoPtr) = 0;

private:
	typedef BCLib::Utility::CHashMap<CINFOPTR, BCLib::int64> TimeoutHashMap;
	TimeoutHashMap m_TimeoutHashMap;
};

}//Utility
}//BCLib


#endif//__BCLIB_UTILITY_IO_TIMEOUTTRACK_H__
