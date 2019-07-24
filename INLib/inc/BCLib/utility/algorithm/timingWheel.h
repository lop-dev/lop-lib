//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/algorithm/timingWheel.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_IO_TIMINGWHEEL_H__
#define __BCLIB_UTILITY_IO_TIMINGWHEEL_H__

#include <BCLib/utility/baseDef.h>
#include <BCLib/utility/hashMap.h>
#include <BCLib/utility/thread/mutex.h>

namespace BCLib
{
namespace Utility
{

// 时间轮分两种情况：
// 1、定时要做的事情，比如每隔定时保存数据；
// 2、超时要做的时期，比如关闭无用网络连接；
template<class CINFOPTR>
class CTimingWheel
{
public:
	CTimingWheel(float fInterval = 1)
	:m_uIndex1(0)
	,m_uIndex2(1)
	,m_fInterval(fInterval)
	,m_fDelay(0)
	,m_uInfoNum(0)
	{
		m_pInfoPtrHashMap1 = &m_InfoPtrHashMap[m_uIndex1];
		m_pInfoPtrHashMap2 = &m_InfoPtrHashMap[m_uIndex2];
	}
	virtual ~CTimingWheel(){}

	BCLib::uint32 getInfoNum()
    {
        BCLib::Utility::CMutexFun fun(&m_mutex);
        return m_uInfoNum;
    }

	void update(float t)
	{
        BCLib::Utility::CMutexFun fun(&m_mutex);

		m_fDelay += t;
		if (m_fDelay < m_fInterval) {
			return;
		}
		m_fDelay -= m_fInterval;

		//
		if ((m_uIndex1 + 1) >= MAP_NUM) m_uIndex1 = 0;
		else m_uIndex1++;

		if ((m_uIndex2 + 1) >= MAP_NUM) m_uIndex2 = 0;
		else m_uIndex2++;

		if (m_pInfoPtrHashMap2) {
			for(typename InfoPtrHashMap::iterator it = m_pInfoPtrHashMap2->begin(); it != m_pInfoPtrHashMap2->end(); ++it) {
				if (m_markInfoPtrHashMap.remove(it->first)) {
					m_uInfoNum--;
				}
			}

			m_pInfoPtrHashMap2->clear();
		}

		//
		m_pInfoPtrHashMap1 = &m_InfoPtrHashMap[m_uIndex1];
		m_pInfoPtrHashMap2 = &m_InfoPtrHashMap[m_uIndex2];
	}

	bool haveInfoPtr(BCLib::uint64 uInfoID)
	{
        BCLib::Utility::CMutexFun fun(&m_mutex);

		if (m_markInfoPtrHashMap.find(uInfoID) != m_markInfoPtrHashMap.end()) {
			return true;
		}

		return false;
	}
	
	CINFOPTR getInfoPtr(BCLib::uint64 uInfoID)
	{
        BCLib::Utility::CMutexFun fun(&m_mutex);

		CINFOPTR value = NULL;
		typename BCLib::Utility::CHashMap<BCLib::uint64, InfoPtrHashMap*>::iterator it1 = m_markInfoPtrHashMap.find(uInfoID);
		if (it1 == m_markInfoPtrHashMap.end()) {
			return value;
		}

		InfoPtrHashMap* pMap = it1->second;
		if (pMap != NULL) {
			pMap->getValue(uInfoID, value);
		}
		return value;
	}
	
	void addInfoPtr(BCLib::uint64 uInfoID, CINFOPTR infoPtr)
	{
        BCLib::Utility::CMutexFun fun(&m_mutex);

		if (m_markInfoPtrHashMap.find(uInfoID) != m_markInfoPtrHashMap.end()) {
			return;
		}

		if (m_pInfoPtrHashMap1 == NULL) {
			return;
		}

		m_markInfoPtrHashMap.setValue(uInfoID, m_pInfoPtrHashMap1);
		m_pInfoPtrHashMap1->setValue(uInfoID, infoPtr);
		m_uInfoNum++;
	}

	void delInfoPtr(BCLib::uint64 uInfoID)
	{
        BCLib::Utility::CMutexFun fun(&m_mutex);

		typename BCLib::Utility::CHashMap<BCLib::uint64, InfoPtrHashMap*>::iterator it1 = m_markInfoPtrHashMap.find(uInfoID);
		if (it1 == m_markInfoPtrHashMap.end()) {
			return;
		}

		InfoPtrHashMap* pMap = it1->second;
		if (pMap != NULL) {
			if (pMap->remove(uInfoID)) {
				m_uInfoNum--;
			}
		}

		m_markInfoPtrHashMap.erase(it1);
	}

private:
	static const BCLib::uint8 MAP_NUM = 60;
    BCLib::Utility::CMutex m_mutex;

	typedef BCLib::Utility::CHashMap<BCLib::uint64, CINFOPTR> InfoPtrHashMap;
	InfoPtrHashMap m_InfoPtrHashMap[MAP_NUM];
	InfoPtrHashMap* m_pInfoPtrHashMap1; // 添加的MAP
	InfoPtrHashMap* m_pInfoPtrHashMap2; // 清空的MAP
	BCLib::Utility::CHashMap<BCLib::uint64, InfoPtrHashMap*> m_markInfoPtrHashMap;

	BCLib::uint8 m_uIndex1;
	BCLib::uint8 m_uIndex2;
	float m_fInterval;
	float m_fDelay;

	BCLib::uint32 m_uInfoNum;
};

}//Utility
}//BCLib


#endif//__BCLIB_UTILITY_IO_TIMINGWHEEL_H__
